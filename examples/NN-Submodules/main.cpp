#include <iostream>

#include "networks/net.h"

int main(void)
{
    Net net(4, 5);

    // Calling parameters() returns a vector of Tensors
    for (const auto& p : net.parameters())
    {
        std::cout << p << std::endl;
    }

    // To see the names of each individual parameters and their values, used named_parameters()
    for (const auto& pair : net.named_parameters())
    {
        std::cout << pair.key() << ": " << pair.value() << std::endl;
    }

    return 0;
}
