#include <iostream>

#include <torch/torch.h>

struct Net : torch::nn::Module{ };

// Value semantics: the simplest approach to use.
// Module objects are allocated on the stack and when passed to a function, can be either copied, moved
// (with std::move) or taken by reference/pointer:
void a(Net net){ };
void b(Net& net){ };
void c(Net* net){ };

// Reference semantics: reduces cognitive overhead of thinking about how modules must be passed to functions
// and how arguments must be declared (assuming shared_ptr is used everywhere)
void d(std::shared_ptr<Net> net){ };

int main(void)
{
    Net net1;
    a(net1);
    a(std::move(net1));
    b(net1);
    c(&net1);

    auto net2 = std::make_shared<Net>();
    d(net2);

    return 0;
}
