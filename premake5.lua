workspace "ImageUtils"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
	}

project "ImageUtils"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	includedirs {"external/stb"}

	targetdir "out/bin/%{cfg.buildcfg}"
	objdir "out/obj/%{cfg.buildcfg}"

	files {"src/**.h", "src/**.cpp"}

	filter "configurations:Debug"
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "Off"