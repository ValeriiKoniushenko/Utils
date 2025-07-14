include_guard()

message(DEBUG "Utils' Predefinitions.cmake was included" )

function(InstallDebugLevel)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_compile_definitions(UTILS_DEBUG=1)
	else()
		add_compile_definitions(UTILS_DEBUG=0)
	endif()
endfunction()

function(InstallBenchmarkDependenciesTesting)
	set(BENCHMARK_ENABLE_TESTING OFF PARENT_SCOPE)
endfunction()