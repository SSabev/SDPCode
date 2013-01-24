solution "SDP-AI"
	configurations { "Debug", "Release" }
 
	project "SDP-AI"
	kind "ConsoleApp"
	language "C++"
	files { "**.h", "**.cpp" }
 
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetdir "./debug"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		targetdir "./release"
