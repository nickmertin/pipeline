cmake_minimum_required(VERSION 3.10)
project(pipeline)

set(CMAKE_CXX_STANDARD 17)

add_library(pipeline SHARED src/pipeline.cpp src/pipeline.h src/source.h src/sink.h src/binding.h src/source_binding.h src/sink_binding.h src/filter.h src/generic_filter.h src/function_filter.h src/function_queue.h src/as_pipeline.h)

add_executable(example_simple_caluclations examples/simple_calculation.cpp)
