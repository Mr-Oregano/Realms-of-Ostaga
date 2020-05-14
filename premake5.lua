workspace "Realms of Ostaga"

	startproject "Realms of Ostaga"
	architecture "x64"

	staticruntime "On"

	configurations {
		"Debug",
		"Release",
		"Dist"
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

	targetdir "bin/%{cfg.bulidcfg}_%cfg.architecture}"
	objdir "bin-int/%{cfg.bulidcfg}_%cfg.architecture}"

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
		"src/**.h",
		"src/**.cpp"
	}

	-- TODO: Use precompiled headers

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

	includedirs {
		"src"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"OSTAGA_BUILD_WINDOWS"
		}

	filter {}