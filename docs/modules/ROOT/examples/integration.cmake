# tag::all_features[]
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE Utils)
# end::all_features[]

# tag::core_only[]
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE Utils::Core)
# end::core_only[]

# tag::foundation_only[]
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE Utils::Utils)
# end::foundation_only[]
