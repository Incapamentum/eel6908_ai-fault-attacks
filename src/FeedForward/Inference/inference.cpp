#include <cstdlib>
#include <filesystem>

#include <torch/torch.h>

#include "../net/net.h"

namespace fs = std::filesystem;

void exit_fn(void)
{
    std::cout << "The directory \"model\" doesn't exist!\n";
    std::cout << "Exiting...\n";

    exit(0);
}

int main(void)
{
    // Create a new Net
    auto net { std::make_shared<Net>() };

    // Creating model path
    fs::path md { fs::current_path() / "../model" };

    // Exit if model directory doesn't exist
    if (!fs::is_directory(md))
        exit_fn();

    // Load the trained model
    torch::load(net, md / "net.pt");

    // Create a multi-threaded data loader for the MNIST dataset.
    auto data_loader { torch::data::make_data_loader(
            torch::data::datasets::MNIST("../data").map(
                    torch::data::transforms::Stack<>()),
            /*batch_size=*/1) };

    unsigned int guess{ 0 };
    unsigned int correct{ 0 };
    for (auto& batch : *data_loader)
    {
        torch::Tensor prediction = net->forward(batch.data);

        auto result{ torch::max(prediction, 1) };
        auto max_index{ std::get<1>(result) };

        if (torch::equal(batch.target, max_index))
            correct++;

        ++guess;

        if (guess > 10)
            break;
    }

    std::cout << "Prediction accuracy: ";
    std::cout << static_cast<double>(correct) / guess << "\n";

    return 0;
}