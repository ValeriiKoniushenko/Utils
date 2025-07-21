
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(BENCH_NAME UtilsBench_String)
add_executable(${BENCH_NAME} StringBench.cpp)
target_link_libraries(${BENCH_NAME} PUBLIC benchmark Utils)

set(BENCH_NAME UtilsBench_Math)
add_executable(${BENCH_NAME} MathBench.cpp)
target_link_libraries(${BENCH_NAME} PUBLIC benchmark Utils)