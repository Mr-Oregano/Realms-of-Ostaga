project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/%{cfg.longname}_%{cfg.architecture}")
    objdir ("bin-int/%{cfg.longname}_%{cfg.architecture}")

    files
    {

        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    
    }

    includedirs
    {

        "include"
    
    }
    
    systemversion "latest"
