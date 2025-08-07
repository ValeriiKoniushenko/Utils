
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
    GLOB Sources
    "*.cpp"
)

add_executable(UtilsTests ${Sources})
target_link_libraries(UtilsTests PUBLIC gtest Utils)
target_compile_features(UtilsTests PUBLIC ${CXX_STANDARD})

set(TEST_COVERAGE_OPTIONS
    "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate>"
    "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-fcoverage-mapping>"
)

target_compile_options(UtilsTests PRIVATE ${TEST_COVERAGE_OPTIONS})
target_compile_options(UtilsTests PRIVATE ${TEST_COVERAGE_OPTIONS})

set(COVERAGE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/coverage")

set(PROFRAW_FILE "${CMAKE_CURRENT_BINARY_DIR}/default.profraw")
set(PROFDATA_FILE "${CMAKE_CURRENT_BINARY_DIR}/default.profdata")

add_custom_target(UtilsCoverage
    COMMENT "Generating coverage report using llvm-cov"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_OUTPUT_DIR}"
    COMMAND llvm-profdata merge -sparse "${PROFRAW_FILE}" -o "${PROFDATA_FILE}"
    COMMAND llvm-cov show "./UtilsTests"
    -instr-profile="${PROFDATA_FILE}"
    -format=html -output-dir="${COVERAGE_OUTPUT_DIR}"
)

add_dependencies(UtilsCoverage UtilsTests)
