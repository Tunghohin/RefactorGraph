﻿#ifndef FRONTEND_INFER_H
#define FRONTEND_INFER_H

#include "tensor.h"
#include <result.h>

namespace refactor::frontend {

    class Operator;
    using Edges = std::vector<Edge>;

    struct FatalError {};
    struct UnknownVariable {
        std::string name;
    };
    struct InferError : public std::runtime_error {
        std::variant<FatalError, UnknownVariable> value;

        explicit InferError(std::string);
        explicit InferError(UnknownVariable);
    };

    struct InferOptions {
        bool calculate;

        bool shouldCalculate(TensorRefs, Shape const &output);
    };

    using InferResult = Result<std::vector<Tensor_>, InferError>;
    using InferFn = InferResult (*)(Operator const &, TensorRefs, InferOptions);

    std::unordered_set<DimVariable> extractDependency(TensorRefs);

    using Indices = absl::InlinedVector<int64_t, 4>;
    /// @brief 将标量坐标 `k` 展开到 `shape` 空间。
    Indices locateN(Shape const &shape, size_t k);
    /// @brief 在 `tensor` 中定位空间坐标 `indices` 所指向的元素。
    void *locate1(Tensor const &tensor, Indices const &indices);

}// namespace refactor::frontend

#endif// FRONTEND_INFER_H
