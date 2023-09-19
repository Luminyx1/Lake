-- premake5.lua
workspace "Lake"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Sandbox"
include "Lake"
