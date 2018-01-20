workspace "TeknoFfb"
	configurations { "Debug", "Public", "PublicNoUpdate" }
	platforms { "x86" }

	flags { "StaticRuntime" }

	systemversion "10.0.15063.0"

	symbols "On"

	characterset "Unicode"

	flags { "NoIncrementalLink" }

	editandcontinue "Off"

	configuration "Debug"
		targetdir "build/bin/debug"
		defines "NDEBUG"
		objdir "build/obj/debug"

	configuration "Release"
		targetdir "build/bin/release"
		defines "NDEBUG"
		optimize "Speed"
		objdir "build/obj/release"

project "TeknoFfb"
	targetname "ffbplugin"
	language "C++"
	kind "SharedLib"

	buildoptions { "/MP" }

	files
	{
		"src/**.cpp", "src/**.h",
		"deps/cpp/**.cpp",
	}

	includedirs { "src", "deps/inc/" }
	libdirs { "deps/lib/" }

	postbuildcommands {	}