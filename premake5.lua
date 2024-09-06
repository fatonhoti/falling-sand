WORKSPACE_NAME = "falling-sand"
PROJECT_NAME = "falling-sand"
PROJECT_LOCATION = PROJECT_NAME

workspace(WORKSPACE_NAME)
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}

-- cfg.buildcfg: e.g. debug, release
-- cfg.system: e.g. windows
-- cfg.architecture: e.g. x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project(PROJECT_NAME)
	location(PROJECT_LOCATION)
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.vert",
		"%{prj.name}/src/**.frag",
		"%{prj.name}/vendor/glad/src/**.c",
	}
	
	includedirs
	{
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/glm/include",
	}
	
	libdirs
	{
		"%{prj.name}/vendor/glfw/lib-vc2022"
	}
	
	links
	{
		"glfw3_mt.lib",
		"opengl32.lib",
	}
	
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		optimize "Off"
	
	filter "configurations:Release"
		symbols "Off"
		optimize "On"