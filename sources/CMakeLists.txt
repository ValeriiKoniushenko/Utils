file(
    GLOB Sources
    "Utils/*.h"
    "Utils/*.cpp"
    "Core/*.h"
    "Core/*.cpp"
)

add_library(Utils INTERFACE ${Sources})
set_target_properties(Utils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Utils INTERFACE ./)
target_link_libraries(Utils INTERFACE glm)