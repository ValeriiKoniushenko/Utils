file(
        GLOB Sources
        "*.h"
        "*.cpp"
)

add_library(UtilsCore STATIC ${Sources})
add_library(Utils::Core ALIAS UtilsCore)
target_compile_features(UtilsCore PUBLIC ${CXX_STANDARD})
set_target_properties(UtilsCore PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(UtilsCore PUBLIC ../)
target_link_libraries(UtilsCore PUBLIC glm UtilsUtils pcre2-8)