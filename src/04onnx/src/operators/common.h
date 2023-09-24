﻿#ifndef ONNX_INFER_H
#define ONNX_INFER_H

#include "common/error_handler.h"
#include "frontend/operator.h"
#include <optional>

namespace refactor::onnx {
    using namespace frontend;
    using ShapeOrNot = std::optional<Shape>;

#define ERROR_MSG(msg) buildMsg(msg, __FILE__, __LINE__)
    // clang-format off
    InferResult inferConstant        (Operator const &, Tensors);
    InferResult inferConstantOfShape (Operator const &, Tensors);
    InferResult inferRange           (Operator const &, Tensors);
    InferResult inferShape           (Operator const &, Tensors);
    InferResult inferArithmetic      (Operator const &, Tensors); computation::SharedOp lowerArithmetic (Operator const &, TensorRefs);
    InferResult inferCast            (Operator const &, Tensors); computation::SharedOp lowerCast       (Operator const &, TensorRefs);
    InferResult inferCompair         (Operator const &, Tensors); computation::SharedOp lowerCompair    (Operator const &, TensorRefs);
    InferResult inferConcat          (Operator const &, Tensors); computation::SharedOp lowerConcat     (Operator const &, TensorRefs);
    InferResult inferCumSum          (Operator const &, Tensors); computation::SharedOp lowerCumSum     (Operator const &, TensorRefs);
    InferResult inferExpand          (Operator const &, Tensors); computation::SharedOp lowerExpand     (Operator const &, TensorRefs);
    InferResult inferGather          (Operator const &, Tensors); computation::SharedOp lowerGather     (Operator const &, TensorRefs);
    InferResult inferGemm            (Operator const &, Tensors); computation::SharedOp lowerGemm       (Operator const &, TensorRefs);
    InferResult inferMatMul          (Operator const &, Tensors); computation::SharedOp lowerMatMul     (Operator const &, TensorRefs);
    InferResult inferPow             (Operator const &, Tensors); computation::SharedOp lowerPow        (Operator const &, TensorRefs);
    InferResult inferReduce          (Operator const &, Tensors); computation::SharedOp lowerReduce     (Operator const &, TensorRefs);
    InferResult inferReshape         (Operator const &, Tensors); computation::SharedOp lowerReshape    (Operator const &, TensorRefs);
    InferResult inferSelect          (Operator const &, Tensors); computation::SharedOp lowerSelect     (Operator const &, TensorRefs);
    InferResult inferSlice           (Operator const &, Tensors); computation::SharedOp lowerSlice      (Operator const &, TensorRefs);
    InferResult inferSoftmax         (Operator const &, Tensors); computation::SharedOp lowerSoftmax    (Operator const &, TensorRefs);
    InferResult inferSplit           (Operator const &, Tensors); computation::SharedOp lowerSplit      (Operator const &, TensorRefs);
    InferResult inferSqueeze         (Operator const &, Tensors); computation::SharedOp lowerSqueeze    (Operator const &, TensorRefs);
    InferResult inferTranspose       (Operator const &, Tensors); computation::SharedOp lowerTranspose  (Operator const &, TensorRefs);
    InferResult inferUnary           (Operator const &, Tensors); computation::SharedOp lowerUnary      (Operator const &, TensorRefs);
    InferResult inferUnsqueeze       (Operator const &, Tensors); computation::SharedOp lowerUnsqueeze  (Operator const &, TensorRefs);
    InferResult inferWhere           (Operator const &, Tensors); computation::SharedOp lowerWhere      (Operator const &, TensorRefs);

    // clang-format on
    using ShapeResult = Result<Shape, std::string>;
    using ShapeRefs = std::vector<std::reference_wrapper<const Shape>>;

    /// @brief 多方向形状广播。
    /// @param inputs 所有输入的形状。
    /// @return 广播后的形状。
    ShapeResult multidirBroadcast(ShapeRefs const &);

    /// @brief 单方向形状广播。
    /// @param target 目标形状。
    /// @param test 测试形状。
    /// @return 测试形状是否可以广播到目标形状。
    bool unidirBroadcast(Shape const &target, Shape const &test);

    /// @brief 池化形状推断。
    /// @param data 输入张量的形状。
    /// @param kernel kernel 的形状。
    /// @param dilations 空洞参数。
    /// @param pads 扩张参数。
    /// @param strides 跳步参数。
    /// @return 池化后的形状。
    ShapeResult pool(Shape const &data,
                     Shape const &kernel,
                     ShapeOrNot const &dilations,
                     ShapeOrNot const &pads,
                     ShapeOrNot const &strides);

#define EXPECT_SIZE(N)                                         \
    if (inputs.size() != (N)) {                                \
        return Err(InferError(ERROR_MSG("Input size error"))); \
    }

#define EXPECT_VAL(DIM, VAL)                                             \
    int64_t VAL;                                                         \
    if ((DIM).hasValue()) {                                              \
        VAL = (DIM).value();                                             \
    } else {                                                             \
        return Err(InferError(UnknownVariable{(DIM.variable()->name)})); \
    }

#define MULTIDIR_BROADCAST(SHAPES)                              \
    Shape output;                                               \
    {                                                           \
        auto res = multidirBroadcast(SHAPES);                   \
        if (res.isErr()) {                                      \
            return Err(InferError(ERROR_MSG(res.unwrapErr()))); \
        }                                                       \
        output = std::move(res.unwrap());                       \
    }
}// namespace refactor::onnx

#endif// ONNX_INFER_H
