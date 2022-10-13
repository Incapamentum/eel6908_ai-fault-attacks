#include <torch/torch.h>

#include <cmath>
#include <cstdio>
#include <iostream>

#include "network/generator.h"
#include "network/discriminator.h"

// The size of the noise vector fed to the generator.
const int64_t kNoiseSize = 100;

// The batch size for training.
const int64_t kBatchSize = 64;

// The number of epochs to train.
const int64_t kNumberOfEpochs = 30;

// Where to find the MNIST dataset.
const char* kDataFolder = "./data";

// After how many batches to create a new checkpoint periodically.
const int64_t kCheckpointEvery = 200;

// How many images to sample at every checkpoint.
const int64_t kNumberOfSamplesPerCheckpoint = 10;

// Set to `true` to restore models and optimizers from previously saved
// checkpoints.
const bool kRestoreFromCheckpoint = false;

// After how many batches to log a new update with the loss value.
const int64_t kLogInterval = 10;

using namespace torch;

int main(int argc, const char* argv[])
{
    // Sets the seed for generating random numbers
    torch::manual_seed(1);

    // Create the device we pass around based on whether CUDA is available.
    torch::Device device(torch::kCPU);
    if (torch::cuda::is_available())
    {
        std::cout << "CUDA is available! Training on GPU." << std::endl;
        device = torch::Device(torch::kCUDA);
    }

    // Creates the generator, passing it and the discriminator to the
    // device
    DCGANGenerator generator(kNoiseSize);
    generator->to(device);
    discriminator->to(device);

    // Assume the MNIST dataset is available under `kDataFolder`;
    auto dataset = torch::data::datasets::MNIST(kDataFolder)
            .map(torch::data::transforms::Normalize<>(0.5, 0.5))
            .map(torch::data::transforms::Stack<>());
    const int64_t batches_per_epoch =
            std::ceil(dataset.size().value() / static_cast<double>(kBatchSize));

    // data_loader is a torch::data::StatelessDataLoader<>, which wraps the MNIST data around an iterable
    // for easy access. Specifically, the iterable produces batch requests in sets
    auto data_loader = torch::data::make_data_loader(
            std::move(dataset),
            torch::data::DataLoaderOptions().batch_size(kBatchSize).workers(2));

    // Both generator and discriminator will make use of the Adam algorithm, which is a method for
    // stochastic optimization
    torch::optim::Adam generator_optimizer(
            generator->parameters(), torch::optim::AdamOptions(2e-4).betas(std::make_tuple (0.5, 0.5)));
    torch::optim::Adam discriminator_optimizer(
            discriminator->parameters(), torch::optim::AdamOptions(2e-4).betas(std::make_tuple (0.5, 0.5)));

    // Can be used to restore models and optimizers from a previous saved state
    if (kRestoreFromCheckpoint)
    {
        torch::load(generator, "generator-checkpoint.pt");
        torch::load(generator_optimizer, "generator-optimizer-checkpoint.pt");
        torch::load(discriminator, "discriminator-checkpoint.pt");
        torch::load(
                discriminator_optimizer, "discriminator-optimizer-checkpoint.pt");
    }

    // Training loop
    int64_t checkpoint_counter = 1;
    for (int64_t epoch = 1; epoch <= kNumberOfEpochs; ++epoch)
    {
        int64_t batch_index = 0;
        for (torch::data::Example<>& batch : *data_loader)
        {
            // Train discriminator with real images.
            discriminator->zero_grad();
            torch::Tensor real_images = batch.data.to(device);
            torch::Tensor real_labels =
                    torch::empty(batch.data.size(0), device).uniform_(0.8, 1.0);
            torch::Tensor real_output = discriminator->forward(real_images);
            torch::Tensor d_loss_real =
                    torch::binary_cross_entropy(real_output, real_labels);
            d_loss_real.backward();

            // Train discriminator with fake images.
            torch::Tensor noise =
                    torch::randn({batch.data.size(0), kNoiseSize, 1, 1}, device);
            torch::Tensor fake_images = generator->forward(noise);
            torch::Tensor fake_labels = torch::zeros(batch.data.size(0), device);
            torch::Tensor fake_output = discriminator->forward(fake_images.detach());
            torch::Tensor d_loss_fake =
                    torch::binary_cross_entropy(fake_output, fake_labels);
            d_loss_fake.backward();

            torch::Tensor d_loss = d_loss_real + d_loss_fake;
            discriminator_optimizer.step();

            // Train generator.
            generator->zero_grad();
            fake_labels.fill_(1);
            fake_output = discriminator->forward(fake_images);
            torch::Tensor g_loss =
                    torch::binary_cross_entropy(fake_output, fake_labels);
            g_loss.backward();
            generator_optimizer.step();
            batch_index++;
            if (batch_index % kLogInterval == 0)
            {
                std::printf(
                        "\r[%2ld/%2ld][%3ld/%3ld] D_loss: %.4f | G_loss: %.4f\n",
                        epoch,
                        kNumberOfEpochs,
                        batch_index,
                        batches_per_epoch,
                        d_loss.item<float>(),
                        g_loss.item<float>());
            }

            // Checkpointing
            if (batch_index % kCheckpointEvery == 0)
            {
                // Checkpoint the model and optimizer state.
                torch::save(generator, "generator-checkpoint.pt");
                torch::save(generator_optimizer, "generator-optimizer-checkpoint.pt");
                torch::save(discriminator, "discriminator-checkpoint.pt");
                torch::save(
                        discriminator_optimizer, "discriminator-optimizer-checkpoint.pt");
                // Sample the generator and save the images.
                torch::Tensor samples = generator->forward(torch::randn(
                        {kNumberOfSamplesPerCheckpoint, kNoiseSize, 1, 1}, device));
                torch::save(
                        (samples + 1.0) / 2.0,
                        torch::str("dcgan-sample-", checkpoint_counter, ".pt"));
                std::cout << "\n-> checkpoint " << ++checkpoint_counter << '\n';
            }
        }
    }

    std::cout << "Training complete!" << std::endl;
}