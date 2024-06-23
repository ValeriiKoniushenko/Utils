file(
	GLOB Sources
	"Utils/*.h"
	"Utils/*.cpp"
)

add_library(Utils STATIC ${Sources})
set_target_properties(Utils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Utils PUBLIC ./)
target_link_libraries(Utils PUBLIC glm)