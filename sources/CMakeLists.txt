add_subdirectory(Core)
add_subdirectory(Utils)

add_library(Utils INTERFACE)
set_target_properties(Utils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Utils INTERFACE ./)
target_link_libraries(Utils INTERFACE Utils::Core Utils::Utils)