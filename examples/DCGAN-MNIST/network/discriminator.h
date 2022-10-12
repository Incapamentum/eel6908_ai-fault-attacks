//
// Created by goose on 10/11/22.
//

#include <torch/torch.h>

#ifndef DCGAN_MNIST_DISCRIMINATOR_H
#define DCGAN_MNIST_DISCRIMINATOR_H

using namespace torch;

nn::Sequential discriminator(
        // Layer 1
        nn::Conv2d(
                nn::Conv2dOptions(1, 64, 4).stride(2).padding(1).bias(false)
                ),
        nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
        // Layer 2
        nn::Conv2d(
                nn::Conv2dOptions(64, 128, 4).stride(2).padding(1).bias(false)
                ),
        nn::BatchNorm2d(128),
        nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
        // Layer 3
        nn::Conv2d(
                nn::Conv2dOptions(128, 256, 4).stride(2).padding(1).bias(false)
                ),
        nn::BatchNorm2d(256),
        nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
        // Layer 4
        nn::Conv2d(
                nn::Conv2dOptions(256, 1, 3).stride(1).padding(0).bias(false)
                ),
        nn::Sigmoid()
        );

#endif //DCGAN_MNIST_DISCRIMINATOR_H
