file(
    GLOB Sources
    "*.h"
    "*.cpp"
)

add_library(UtilsUtils INTERFACE ${Sources})
add_library(Utils::Utils ALIAS UtilsUtils)
target_compile_features(UtilsUtils INTERFACE ${CXX_STANDARD})
set_target_properties(UtilsUtils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(UtilsUtils INTERFACE ../)
target_link_libraries(UtilsUtils INTERFACE glm)