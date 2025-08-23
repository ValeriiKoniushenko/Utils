include_guard()

function(UtilsCoreAddCompileOptionsTo Target)
    if (MSVC)
        target_compile_options(${Target} PRIVATE
            "/W4"
            "$<$<CONFIG:RELEASE>:/O2>"
        )
    else ()
        target_compile_options(${Target} PRIVATE
            "-Wall"
            "-Wextra"
            # "-Werror"
            "-Wno-error=unused-variable"
            "-Wno-unused-variable"
            "$<$<CONFIG:DEBUG>:-g3>"
            "$<$<CONFIG:DEBUG>:-O0>"
            "$<$<CONFIG:DEBUG>:-fno-inline>"
            "$<$<CONFIG:DEBUG>:-fno-inline-functions>"

            "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GCC>>:-fno-inline-small-functions>"
            "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GCC>>:-fno-inline-functions-called-once>"
            "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GCC>>:-fno-default-inline>"
            "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GCC>>:-fno-early-inlining>"
            "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GCC>>:-fno-inline-atomics>"

            "$<$<CONFIG:RELEASE>:-O3>"
        )

        if (BUILD_UTILS_WITH_TEST_COVERAGE)
            target_compile_options(${Target} PRIVATE
                "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate>"
                "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-fcoverage-mapping>"
            )

            target_link_options(${Target} PRIVATE
                "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate>"
                "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-fcoverage-mapping>"
            )
        endif ()

    endif ()
endfunction()
