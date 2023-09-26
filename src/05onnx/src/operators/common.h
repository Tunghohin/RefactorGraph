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
    InferResult inferConstant        (Operator const &, TensorRefs, InferOptions);
    InferResult inferConstantOfShape (Operator const &, TensorRefs, InferOptions);
    InferResult inferRange           (Operator const &, TensorRefs, InferOptions);
    InferResult inferShape           (Operator const &, TensorRefs, InferOptions);
    InferResult inferArithmetic      (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerArithmetic (Operator const &, TensorRefs);
    InferResult inferCast            (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerCast       (Operator const &, TensorRefs);
    InferResult inferCompair         (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerCompair    (Operator const &, TensorRefs);
    InferResult inferConcat          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerConcat     (Operator const &, TensorRefs);
    InferResult inferCumSum          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerCumSum     (Operator const &, TensorRefs);
    InferResult inferExpand          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerExpand     (Operator const &, TensorRefs);
    InferResult inferGather          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerGather     (Operator const &, TensorRefs);
    InferResult inferGemm            (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerGemm       (Operator const &, TensorRefs);
    InferResult inferMatMul          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerMatMul     (Operator const &, TensorRefs);
    InferResult inferPow             (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerPow        (Operator const &, TensorRefs);
    InferResult inferReduce          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerReduce     (Operator const &, TensorRefs);
    InferResult inferReshape         (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerReshape    (Operator const &, TensorRefs);
    InferResult inferSelect          (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerSelect     (Operator const &, TensorRefs);
    InferResult inferSlice           (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerSlice      (Operator const &, TensorRefs);
    InferResult inferSoftmax         (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerSoftmax    (Operator const &, TensorRefs);
    InferResult inferSplit           (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerSplit      (Operator const &, TensorRefs);
    InferResult inferSqueeze         (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerSqueeze    (Operator const &, TensorRefs);
    InferResult inferTranspose       (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerTranspose  (Operator const &, TensorRefs);
    InferResult inferUnary           (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerUnary      (Operator const &, TensorRefs);
    InferResult inferUnsqueeze       (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerUnsqueeze  (Operator const &, TensorRefs);
    InferResult inferWhere           (Operator const &, TensorRefs, InferOptions); computation::SharedOp lowerWhere      (Operator const &, TensorRefs);

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
