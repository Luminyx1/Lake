-- dependencies.lua

include "vendor/glfw"

project "Lake"
    includedirs {
        "vendor/glfw/include"
    }

    links {
        "GLFW"
    }
