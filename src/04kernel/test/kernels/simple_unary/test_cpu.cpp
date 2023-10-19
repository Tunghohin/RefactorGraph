﻿#include "../../../src/kernels/simple_unary/cpu_kernel.hh"
#include <gtest/gtest.h>

using namespace refactor;
using namespace kernel;

TEST(kernel, SimpleUnaryCpu) {
    // build routine
    auto dataTensor = Tensor::share(DataType::F32, Shape{20, 30, 50});
    auto kernel = SimpleUnaryCpu::build(SimpleUnaryType::Tanh, *dataTensor);
    ASSERT_TRUE(kernel);
    auto routine = kernel->lower();
    // malloc
    auto cpuMem = mem_manager::ForeignBlob::share(
        Target(Target::Cpu).memFunc(),
        dataTensor->bytesSize());
    // put input data
    std::vector<float> data(dataTensor->elementsSize());
    for (auto i : range0_(data.size())) { data[i] = i * 1e-4f; }
    cpuMem->copyIn(data.data(), dataTensor->bytesSize());
    // inference
    auto res = runtime::Resources();
    void const *inputs[]{*cpuMem};
    void *outputs[]{*cpuMem};
    routine(res, inputs, outputs);
    // take output data
    std::vector<float> result(dataTensor->elementsSize());
    cpuMem->copyOut(result.data(), dataTensor->bytesSize());
    // check
    for (auto i : range0_(data.size())) {
        EXPECT_FLOAT_EQ(std::tanh(data[i]), result[i]);
    }
}
