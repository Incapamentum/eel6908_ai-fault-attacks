#include <filesystem>

#include <torch/torch.h>

#include "../net/net.h"

namespace fs = std::filesystem;

// Path to the model
const fs::path g_md { fs::current_path() / "../model" };

int main(void)
{
    // Create a new Net.
    auto net { std::make_shared<Net>() };

    // Create model directory if it doesn't exist
    if (!fs::is_directory(g_md))
        fs::create_directory(g_md);

    // Create a multi-threaded data loader for the MNIST dataset.
    auto data_loader { torch::data::make_data_loader(
            torch::data::datasets::MNIST("../data").map(
                    torch::data::transforms::Stack<>()),
            /*batch_size=*/64) };

    // Instantiate an SGD optimization algorithm to update our Net's parameters.
    torch::optim::SGD optimizer(net->parameters(), /*lr=*/0.01);

    for (size_t epoch = 1; epoch <= 10; ++epoch)
    {
        size_t batch_index{ 0 };
        // Iterate the data loader to yield batches from the dataset.
        for (auto& batch : *data_loader)
        {
            // Reset gradients.
            optimizer.zero_grad();
            // Execute the model on the input data.
            torch::Tensor prediction = net->forward(batch.data);
            // Compute a loss value to judge the prediction of our model.
            torch::Tensor loss = torch::nll_loss(prediction, batch.target);
            // Compute gradients of the loss w.r.t. the parameters of our model.
            loss.backward();
            // Update the parameters based on the calculated gradients.
            optimizer.step();
            // Output the loss and checkpoint every 100 batches.
            if (++batch_index % 100 == 0)
            {
                std::cout << "Epoch: " << epoch << " | Batch: " << batch_index
                          << " | Loss: " << loss.item<float>() << std::endl;
                // Serialize your model periodically as a checkpoint.
                torch::save(net, g_md / "net.pt");
            }
        }
    }

    return 0;
}