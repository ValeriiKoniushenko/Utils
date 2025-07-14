include_guard()

message(DEBUG "Utils::Predefinitions.cmake was included" )

function(Utils_InstallDebugLevel)
	message(DEBUG "Utils::Utils_InstallDebugLevel - run" )
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_definitions(Utils INTERFACE UTILS_DEBUG=1)
	else()
		target_compile_definitions(Utils INTERFACE UTILS_DEBUG=0)
	endif()
endfunction()

function(Utils_InstallBenchmarkDependenciesTesting)
	message(DEBUG "Utils::Utils_InstallBenchmarkDependenciesTesting - run" )
	set(BENCHMARK_ENABLE_TESTING OFF PARENT_SCOPE)
endfunction()