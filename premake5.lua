-- premake5.lua
workspace "Lake"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Lake"
