#include <iostream>

#include "networks/net.h"

int main(void)
{
    Net net(4, 5);

    // Forward pass of network
    std::cout << net.forward(torch::ones({2, 4})) << std::endl;

    return 0;
}
