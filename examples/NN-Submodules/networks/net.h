//
// Created by goose on 10/8/22.
//

#include <torch/torch.h>

#ifndef NN_SUBMODULES_NET_H
#define NN_SUBMODULES_NET_H

struct Net : torch::nn::Module
{
    /// Constructor now registers submodules, such as Linear, via the initializer list of the constructor.
    /// \param N size or dimension of Tensor
    /// \param M size or dimension of Tensor
    Net(int64_t N, int64_t M)
        : linear(register_module("linear", torch::nn::Linear(N, M)))
    {
        b = register_parameter("b", torch::randn(M));
    }

    torch::Tensor forward(torch::Tensor input)
    {
        return linear(input) + b;
    }

    torch::nn::Linear linear;
    torch::Tensor b;
};

#endif //NN_SUBMODULES_NET_H
