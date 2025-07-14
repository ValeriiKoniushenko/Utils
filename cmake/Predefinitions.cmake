include_guard()

message(DEBUG "Utils' Predefinitions.cmake was included" )

function(Utils_InstallDebugLevel)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_compile_definitions(UTILS_DEBUG=1)
	else()
		add_compile_definitions(UTILS_DEBUG=0)
	endif()
endfunction()

function(Utils_InstallBenchmarkDependenciesTesting)
	set(BENCHMARK_ENABLE_TESTING OFF PARENT_SCOPE)
endfunction()