file(
    GLOB Sources
    "*.h"
    "*.cpp"
)

add_library(UtilsUtils STATIC ${Sources})
add_library(Utils::Utils ALIAS UtilsUtils)
target_compile_features(UtilsUtils PUBLIC ${CXX_STANDARD})
set_target_properties(UtilsUtils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(UtilsUtils PUBLIC ../)
target_link_libraries(UtilsUtils PUBLIC glm libassert::assert)

# On windows copy libassert.dll to the same directory as the executable for your_target
if (WIN32)
    add_custom_command(
        TARGET UtilsUtils POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:libassert::assert>
        $<TARGET_FILE_DIR:UtilsUtils>
    )
endif ()

UtilsCoreAddCompileOptionsTo(UtilsUtils)