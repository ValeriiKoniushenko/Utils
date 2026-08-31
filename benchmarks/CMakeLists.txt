
set(BENCH_NAME UtilsBench_String)
add_executable(${BENCH_NAME} StringBench.cpp)
target_link_libraries(${BENCH_NAME} PUBLIC benchmark Utils)
UtilsCoreAddCompileOptionsTo(${BENCH_NAME})

set(BENCH_NAME UtilsBench_ProdStringsCase)
add_executable(${BENCH_NAME} ProdCaseBigStringBench.cpp Dictionary.h)
target_link_libraries(${BENCH_NAME} PUBLIC benchmark Utils)
UtilsCoreAddCompileOptionsTo(${BENCH_NAME})

set(BENCH_NAME UtilsBench_Math)
add_executable(${BENCH_NAME} MathBench.cpp)
target_link_libraries(${BENCH_NAME} PUBLIC benchmark Utils)
UtilsCoreAddCompileOptionsTo(${BENCH_NAME})
