﻿#ifndef COMPUTATION_POOL_H
#define COMPUTATION_POOL_H

#include "../operator.h"
#include "kernel/collectors/pool.h"

namespace refactor::computation {
    using kernel::PoolAttributes;
    using kernel::PoolType;

    struct Pool final : public Operator {
        PoolType type;
        PoolAttributes attributes;

        Pool(PoolType, PoolAttributes) noexcept;

        static size_t typeId(PoolType) noexcept;
        size_t opTypeId() const noexcept final;
        std::string_view name() const noexcept final;
        kernel::CollectorBox candidateKernels(Target) const noexcept final;
    };

}// namespace refactor::computation

#endif// COMPUTATION_POOL_H
