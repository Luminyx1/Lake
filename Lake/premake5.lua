project "Lake"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    links {
        "GLFW",
        "glad",
    }

    includedirs {
        "include",

        "vendor/glad/include",
        "vendor/glfw/include",
        "vendor/glm/",
        "vendor/imgui"
    }

    files {
        "src/**.cpp",
    }

    defines {
        "LK_INTERNAL"
    }

    flags {
        "MultiProcessorCompile",
        "ShadowedVariables",
        "FatalWarnings"
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
        optimize "speed"
        symbols "on"
        flags {
            "LinkTimeOptimization"
        }
    
    filter "configurations:Dist"
        defines {
            "LK_DIST",
            "LK_DISABLE_LOG_TRACE",
            "LK_DISABLE_LOG_INFO"
        }
        runtime "Release"
        optimize "speed"
        symbols "off"
        flags {
            "LinkTimeOptimization"
        }

group "Dependencies"
    include "vendor/glfw"
    include "vendor/glad"
group ""
