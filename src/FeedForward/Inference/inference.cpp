#include <torch/torch.h>

#include "../net/net.h"

// Testing stuff real quick
#include <filesystem>
#include <iostream>

int main(void)
{
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << cwd;
}