include_guard()

function(UtilsCoreAddCompileOptionsTo Target)
    target_compile_definitions(${Target} PUBLIC NOMINMAX)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${Target} PUBLIC
            UTILS_DEBUG=1
        )
    endif ()

    if (MSVC)
        target_compile_options(${Target} PRIVATE
            "/wd4005"            # disable: macro redefinition (closest to -Wno-comment, MSVC warns differently)
            "/wd4101"            # disable: unused variable (-Wno-unused-variable)
            "/wd4100"            # disable: unused parameter (-Wno-unused-parameter)
            "/we4715"            # treat "not all control paths return a value" as error (-Werror=return-type)
            "/wd4702"            # no unreachable code

            # Debug config
            "$<$<CONFIG:DEBUG>:/Od>"
            "$<$<CONFIG:DEBUG>:/Ob0>"
            "$<$<CONFIG:DEBUG>:/Oy->"

            # Release config
            "$<$<CONFIG:RELEASE>:/O2>"
        )
    else ()
        target_compile_options(${Target} PRIVATE
            "-Wall"
            "-Wextra"
            "-Wno-comment"
            # "-Werror"
            "-Wno-error=unused-variable"
            "-Wno-unused-variable"
            "-Wno-unused-parameter"
            "-Wno-deprecated-declarations"
            "-Werror=return-type"

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
