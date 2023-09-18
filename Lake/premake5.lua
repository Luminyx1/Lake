project "Lake"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    files {
        "src/**.cpp",
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "LK_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "LK_DEBUG"
        runtime "Debug"
        optimize "off"
        symbols "on"
    
    filter "configurations:Release"
        defines "LK_RELEASE"
        runtime "Release"
        optimize "on"
        symbols "on"
    
    filter "configurations:Dist"
        defines "LK_DIST"
        runtime "Release"
        optimize "on"
        symbols "off"

include "dependencies.lua"
