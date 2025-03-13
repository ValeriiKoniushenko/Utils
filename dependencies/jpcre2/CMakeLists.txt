
add_library(_jpcre2 INTERFACE jpcre2.hpp)
add_library(jpcre2 ALIAS _jpcre2)
target_compile_features(_jpcre2 INTERFACE ${CXX_STANDARD})
set_target_properties(_jpcre2 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(_jpcre2 INTERFACE ./)
target_link_libraries(_jpcre2 INTERFACE pcre2-32)