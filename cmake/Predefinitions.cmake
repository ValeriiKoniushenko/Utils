include_guard()

message(DEBUG "Utils::Predefinitions.cmake was included" )

function(Utils_InstallBenchmarkDependenciesTesting)
	message(DEBUG "Utils::Utils_InstallBenchmarkDependenciesTesting - was called" )
	set(BENCHMARK_ENABLE_TESTING OFF PARENT_SCOPE)
endfunction()