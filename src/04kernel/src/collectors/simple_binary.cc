﻿#include "kernel/collectors/simple_binary.h"
#include "../kernels/simple_binary/arthimetic11.hh"
#include "../kernels/simple_binary/binary_cuda.hh"
#include "../kernels/simple_binary/binary_cudnn.hh"
#include "common.h"

namespace refactor::kernel {

#define REGISTER(T)                             \
    if (auto ptr = T::build(type, a, b); ptr) { \
        ans.emplace_back(std::move(ptr));       \
    }
#define REGISTER_BROCAST(T)                        \
    if (auto ptr = T::build(type, a, b, c); ptr) { \
        ans.emplace_back(std::move(ptr));          \
    }

    std::vector<KernelBox>
    SimpleBinaryCollector::filter(TensorRefs inputs, TensorRefs outputs) const {
        auto const &a = inputs[0].get();
        auto const &b = inputs[1].get();
        auto const &c = outputs[0].get();

        std::vector<KernelBox> ans;
        switch (target) {
            case Target::Cpu:
                REGISTER(Arthimetic11)
                break;
            case Target::NvidiaGpu:
                REGISTER_BROCAST(BinaryCudnn)
                REGISTER(BinaryCuda)
                break;
            default:
                UNREACHABLEX(void, "Unknown target");
        }
        return ans;
    }

}// namespace refactor::kernel
