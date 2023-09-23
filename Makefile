﻿.PHONY : build clean clean-log format test-all

TYPE ?= release
BUILD_SHARED ?= OFF
FORMAT_ORIGIN ?=

CMAKE_OPT = -DCMAKE_BUILD_TYPE=$(TYPE) -DBUILD_SHARED=$(BUILD_SHARED)

build:
	mkdir -p build/$(TYPE)
	cd build/$(TYPE) && cmake $(CMAKE_OPT) ../.. && make -j

clean:
	rm -rf build

clean-log:
	rm -rf log

format:
	@python3 scripts/format.py $(FORMAT_ORIGIN)

test-all:
	./build/$(TYPE)/src/00common/common_test
	./build/$(TYPE)/src/01graph_topo/graph_topo_test
	./build/$(TYPE)/src/03frontend/frontend_test
	./build/$(TYPE)/src/04onnx/onnx_test
