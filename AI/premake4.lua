solution "SDP-AI"
	configurations { "Debug", "Release", "Test" }
 
	project "SDP-AI"
	kind "ConsoleApp"
	language "C++"
	files { "*.h", "*.cpp", 
		"AStar/*.h", "AStar/*.cpp", 
		"Eagle/*.h", "Eagle/*.cpp",
		"Foresee/*.h", "Foresee/*.cpp",
		"Impala/*.h", "Impala/*.cpp",
		"MathTypes/*.h", "MathTypes/*.cpp", 
		"UnitTests/*.h", "UnitTests/*.cpp" }

	links { "cpptest" }

	defines{ "STANDALONE" }

	libdirs { "./libs/cpptest/src/.libs" }
	includedirs { "./libs/cpptest/src" }
 
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetdir "./debug"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		targetdir "./release"

	configuration "Test"
		defines { "DEBUG", "TEST" }
		flags { "Symbols" }
		targetdir "./test"

