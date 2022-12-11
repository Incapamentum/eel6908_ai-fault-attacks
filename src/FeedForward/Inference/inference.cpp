#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

#include <torch/torch.h>

#include "../net/net.h"

namespace fs = std::filesystem;

const fs::path g_md { fs::current_path() / "../model" };
constexpr int g_cutoff { 5000 };
constexpr uint8_t pat{ 0xAA };

void exit_fn(void)
{
    std::cout << "The directory \"model\" doesn't exist!\n";
    std::cout << "Exiting...\n";

    exit(0);
}

uint64_t random_int(size_t max)
{
    std::mt19937_64 gen(std::random_device{ }());
    std::uniform_int_distribution<std::size_t> uni(0, max);

    return uni(gen);
}

double random_prob(void)
{
    std::mt19937_64 gen(std::random_device{ }());
    std::uniform_real_distribution<> uni(0, 1);

    return uni(gen);
}

// Return a random weight from the vector.
torch::Tensor& random_weight(const std::vector<torch::Tensor>& w)
{
    auto r {random_int(w.size() - 1) };

    return const_cast<torch::Tensor &>(w[r]);
}

// Generate a random address, then align.
uint64_t random_aligned(const size_t n_bytes, const size_t alignment)
{
    uint64_t addr {random_int(n_bytes - 1) };

    // Align
    if (addr % alignment != 0)
        addr = (addr < alignment) ? alignment : addr - addr % alignment;

    return addr;
}

// Main entry-point to model inference.
void run_inference(bool toggle = false)
{
    std::vector<torch::Tensor> weights{ };

    unsigned int guess{ 0 };
    unsigned int correct{ 0 };

    auto net{ std::make_shared<Net>() };

    // Load the model
    torch::load(net, g_md / "net.pt");

    // Create a multi-threaded data loader for the MNIST dataset.
    auto data_loader { torch::data::make_data_loader(
            torch::data::datasets::MNIST("../data").map(
                    torch::data::transforms::Stack<>()),
            /*batch_size=*/1) };

    if (toggle)
    {
        for (const auto& p : net->named_parameters())
        {
            if (p.key().find("weight") != std::string::npos)
                weights.push_back(p.value());
        }
    }

    for (auto& batch : *data_loader)
    {
        // Randomly select a weight matrix and change param values
        if (toggle)
        {
            auto rand_weight { random_weight(weights) };

            auto total_bytes { rand_weight.sym_nbytes() };
            auto dims { rand_weight.sizes() };

            auto aligned_addr { random_aligned(total_bytes.expect_int(),
                                               sizeof(float) * dims[1]) };

            float* dp { rand_weight.data_ptr<float>() };
            uint8_t* b_dp { reinterpret_cast<uint8_t*>(dp) };

            // Overwrite data pointed at by the data pointer
            for (int i { 0 }; i < sizeof(float) * dims[1]; i++)
            {
                if (random_prob() < 0.1)
                    b_dp[aligned_addr + i] ^= pat;
            }
        }

        torch::Tensor prediction = net->forward(batch.data);

        auto result{ torch::max(prediction, 1) };
        auto max_index{ std::get<1>(result) };

        if (torch::equal(batch.target, max_index))
            correct++;

        ++guess;

        if (guess > g_cutoff)
            break;
    }

    std::cout << "    Number of images processed: " << g_cutoff << "\n";
    std::cout << "    Prediction accuracy: ";
    std::cout << static_cast<double>(correct) / guess << "\n";
}

int main(void)
{
    // Exit if model directory doesn't exist
    if (!fs::is_directory(g_md))
        exit_fn();

    std::cout << "Running standard inference..." << "\n";
    run_inference();
    std::cout << "Done with standard inference." << "\n\n";

    std::cout << "Running toggling inference..." << "\n";
    run_inference(true);
    std::cout << "Done with toggling inference." << "\n";

    return 0;
}