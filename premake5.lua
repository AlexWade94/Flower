workspace "Flower"
    architecture "x86_64"
    configurations
    {
        "Debug",
        "Release",
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

IncludeDir["olcEngine"] = "%{wks.location}/%{prj.name}/vendor/olcEngine"

project "Flower"
    location "Flower"
    kind "ConsoleApp"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
	{
		"%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/olcEngine/**.h",
        "%{prj.name}/vendor/olcEngine/**.cpp"
        
    }

    includedirs
	{
        "%{prj.name}/src",
        "%{IncludeDir.olcEngine}",
    }

filter "system:windows"
    staticruntime "on"
    systemversion "latest"


filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "on"

    postbuildcommands
    {
        ("{COPY} %{wks.location}/Flower/Settings.txt \"../bin/" .. outputdir .. "/Flower\"")
    }

filter "configurations:Release"
    defines "_NDEBUG"
    runtime "Release"
    optimize "on"

    postbuildcommands
    {
        ("{COPY} %{wks.location}/Flower/Settings.txt \"../bin/" .. outputdir .. "/Flower\"")
    }
