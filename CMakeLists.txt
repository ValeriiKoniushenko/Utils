cmake_minimum_required(VERSION 3.27.0)

project(Utils VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
	GLOB Sources
	"sources/*.h"
	"sources/*.cpp"
)

add_library(Utils STATIC Sources)
set_target_properties(Utils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Utils PUBLIC sources)

add_executable(Test main.cpp)
target_link_libraries(Test PUBLIC Utils)