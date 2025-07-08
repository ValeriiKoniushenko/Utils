
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
	GLOB Sources
	"*.cpp"
)

add_executable(UtilsBench ${Sources})
target_link_libraries(UtilsBench PUBLIC benchmark Utils)