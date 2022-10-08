#include <iostream>

#include <torch/torch.h>

int main(void)
{
    torch::Tensor tensor = torch::eye(3);
    std::cout << tensor << std::endl;

    return 0;
}
