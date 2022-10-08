//
// Created by goose on 10/8/22.
//

#include <torch/torch.h>

#ifndef NN_MODELS_NET_H
#define NN_MODELS_NET_H

// Simple Net class definition (struct used for sake of simplicity)
struct Net : torch::nn::Module
{
    /// Default constructor registers weight W and bias B parameters
    /// \param N Size or dimension of tensor
    /// \param M Size or dimension of tensor
    Net(int64_t N, int64_t M)
    {
        W = register_parameter("W", torch::randn({N, M}));
        b = register_parameter("b", torch::randn(M));
    }

    /// Performs matrix multiplication of the input with weight matrix, bias is added to the final result.
    /// \param input Tensor input for forward direction
    /// \return
    torch::Tensor forward(torch::Tensor input)
    {
        return torch::addmm(b, input, W);
    }

    torch::Tensor W, b;
};

#endif //NN_MODELS_NET_H
