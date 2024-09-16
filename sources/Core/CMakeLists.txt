file(
    GLOB Sources
    "*.h"
    "*.cpp"
)

add_library(UtilsCore INTERFACE ${Sources})
add_library(Utils::Core ALIAS UtilsCore)
set_target_properties(UtilsCore PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(UtilsCore INTERFACE ../)
target_link_libraries(UtilsCore INTERFACE glm UtilsUtils)