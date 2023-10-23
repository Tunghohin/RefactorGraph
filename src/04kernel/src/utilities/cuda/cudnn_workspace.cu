﻿#include "cuda_mem.cuh"
#include "cudnn_workspace.hh"

namespace refactor::kernel::cudnn {

    CudnnWorkspace::CudnnWorkspace() noexcept
        : runtime::Resource(), ptr(cuda::BasicCudaMemManager::instance()->malloc(size)) {}
    CudnnWorkspace::~CudnnWorkspace() noexcept {
        cuda::BasicCudaMemManager::instance()->free(ptr);
        ptr = nullptr;
    }

    auto CudnnWorkspace::typeId() noexcept -> size_t {
        static uint8_t ID = 1;
        return reinterpret_cast<size_t>(&ID);
    }
    auto CudnnWorkspace::build() noexcept -> runtime::ResourceBox {
        return std::make_unique<CudnnWorkspace>();
    }

    auto CudnnWorkspace::resourceTypeId() const noexcept -> size_t {
        return typeId();
    }
    auto CudnnWorkspace::description() const noexcept -> std::string_view {
        return "CudnnWorkspace";
    }

}// namespace refactor::kernel::cudnn
