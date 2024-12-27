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
target_link_libraries(UtilsUtils INTERFACE glm)