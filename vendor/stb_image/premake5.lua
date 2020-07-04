project "stb_image"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/%{cfg.longname}_%{cfg.architecture}")
    objdir ("bin-int/%{cfg.longname}_%{cfg.architecture}")

    files {
        "include/**.h",
        "src/**.cpp"
    }

    includedirs {
        "include"
    }
    
    systemversion "latest"
