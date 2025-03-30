workspace "Fable"
    architecture "x64"
    startproject "Sandbox"
    buildoptions "/utf-8"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Fable/vendor/GLFW"

project "Fable"
    location "Fable"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "fbpch.h"
    pchsource "Fable/src/fbpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Fable/src",
        "Fable/vendor/GLFW/include",
        "Fable/vendor/glm",
        "/home/user/VulkanSDK/1.4.304.1/Include",
    }

    libdirs 
    {
		"/home/user/VulkanSDK/1.4.304.1/Lib"
	}

    links 
    {
        "GLFW",
        "vulkan-1.lib"
    }

    filter "system:windows"
        staticruntime "on"
        systemversion "latest"

        defines
        {
            "FB_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "FB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "FB_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "FB_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Fable/src",
        "Fable/vendor/glm",
        "C:/VulkanSDK/1.4.304.1/Include",
    }

    links
    {
        "Fable"
    }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "FB_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "FB_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "FB_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "FB_DIST"
        optimize "on"