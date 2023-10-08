﻿#include "mem_manager/foreign_blob.hh"
#include <utility>

namespace refactor::mem_manager {

    ForeignBlob::ForeignBlob(MemFunctions const &f, size_t bytes)
        : _memFunctions(f),
          _ptr(f.malloc(bytes)) {}
    ForeignBlob::~ForeignBlob() {
        _memFunctions.free(std::exchange(_ptr, nullptr));
    }

    std::shared_ptr<ForeignBlob>
    ForeignBlob::share(MemFunctions const &f, size_t bytes) {
        return std::shared_ptr<ForeignBlob>(new ForeignBlob(f, bytes));
    }
    void const *ForeignBlob::operator*() const noexcept { return _ptr; }
    void *ForeignBlob::operator*() noexcept { return _ptr; }

}// namespace refactor::mem_manager
