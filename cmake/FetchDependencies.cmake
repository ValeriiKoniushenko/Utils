include(FetchContent)

message(DEBUG "Utils::FetchDependencies.cmake was included" )

function(Core_SuppressAllSubmoduleWarnings Target)
    target_compile_options(${Target} PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:/w>
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-w>
    )
endfunction()

FetchContent_Declare(Glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(Glm)
Core_SuppressAllSubmoduleWarnings(glm)


FetchContent_Declare(GoogleTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(GoogleTest)
Core_SuppressAllSubmoduleWarnings(gtest)



FetchContent_Declare(Benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.4
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(Benchmark)
Core_SuppressAllSubmoduleWarnings(benchmark)



FetchContent_Declare(PCRE2
        GIT_REPOSITORY https://github.com/PCRE2Project/pcre2.git
        GIT_TAG pcre2-10.45
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(PCRE2)
Core_SuppressAllSubmoduleWarnings(pcre2-8-static)
