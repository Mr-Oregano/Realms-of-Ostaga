workspace "Realms of Ostaga"

	startproject "Realms of Ostaga"
	architecture "x64"

	staticruntime "On"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	defines {
		"PROFILE_RESULTS_DIR=\"profile-results\""
	}

	filter "configurations:Debug"
		symbols "On"
		optimize "Off"
		defines "OSTAGA_DEBUG"

	filter "configurations:Release"
		symbols "Off"
		optimize "On"
		defines "OSTAGA_RELEASE"

	filter "configurations:Dist"
		symbols "Off"
		optimize "On"
		defines "OSTAGA_DIST"

	filter {}

	targetdir "bin/%{cfg.buildcfg}_%{cfg.architecture}"
	objdir "bin-int/%{cfg.buildcfg}_%{cfg.architecture}"

	group "Vendor"
		include "vendor/glfw"
		include "vendor/glad"
		include "vendor/stb_image"
		include "vendor/openal-soft"
		include "vendor/dr_wav"
	group ""

project "Realms of Ostaga"
	language "C++"
	cppdialect "C++17"

	filter "configurations:Debug"
		kind "ConsoleApp"

	filter "configurations:Release"
		kind "ConsoleApp"

	filter "configurations:Dist"
		kind "WindowedApp"

	filter {}

	files {
		"include/**.h",
		"src/**.cpp",

		"tests/**.h",
		"tests/**.cpp"
	}

	includedirs {
		"include",
		"tests",

		"vendor/glfw/include",
		"vendor/glad/include",
		"vendor/spdlog/include",
		"vendor/stb_image/include",
		"vendor/imgui",
		"vendor/glm",
		"vendor/openal-soft/include",
		"vendor/dr_wav/include"
	}

	links {
		"GLFW",
		"glad",
		"stb_image",
		"opengl32.lib",
		"openal-soft",
		"dr_wav"
	}

	pchheader "ospch.h"
	pchsource "src/ospch.cpp"

	defines {
		-- This project uses Glad as an OpenGL loader library so it is not necessary for
		-- GLFW to include the OpenGL header.
		"GLFW_INCLUDE_NONE",

		-- Some dependencies use C std functions which cause warnings in C++17
		-- the following flag will silence these warnings that may be produce.
		"_CRT_SECURE_NO_WARNINGS",

		-- Using the C++17 dialect on the 'fmt' library (used by 'spdlog'')
		-- Will generate several warnings, the following flag will suppress 
		-- these warnings.
		"_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"OSTAGA_BUILD_WINDOWS"
		}

	filter {}