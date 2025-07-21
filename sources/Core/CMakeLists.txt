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



if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(MY_DEBUG_OPTIONS -Wno-unused-value)
    set(MY_RELEASE_OPTIONS -Wno-unused-value)
    target_compile_options(UtilsCore PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
    target_compile_options(UtilsCore PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
endif()
