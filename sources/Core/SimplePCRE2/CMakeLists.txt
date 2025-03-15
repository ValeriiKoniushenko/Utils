file(
        GLOB Sources
        "*.h"
        "*.cpp"
)

add_library(UtilsSimplePCRE2 STATIC ${Sources})
add_library(Utils::SimplePCRE2 ALIAS UtilsSimplePCRE2)
target_compile_features(UtilsSimplePCRE2 PUBLIC ${CXX_STANDARD})
set_target_properties(UtilsSimplePCRE2 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(UtilsSimplePCRE2 PUBLIC ../)
target_link_libraries(UtilsSimplePCRE2 PUBLIC pcre2-8 Utils::Core)