workspace "Arctix"
	architecture		"x64"
	configurations		{ "Debug", "Release" }
	startproject		"Sandbox"
	
	
outdir				= "%{cfg.system}_%{cfg.architecture}_%{cfg.buildcfg}"

include_dirs			= {}
include_dirs["src"]		= "Engine/Src"
include_dirs["graphics"]	= "Engine/External/Vulkan/Include"

lib_dirs			= {}	
lib_dirs["graphics"]		= "Engine/External/Vulkan/Lib"


project "Engine"
	location		"Engine"
	kind			"SharedLib"
	language		"C"
	intrinsics		"On"
	floatingpoint		"Fast"
	compileas		"C"	
		
	targetdir       	("bin/" .. outdir .. "/%{prj.name}")
	objdir          	("obj/" .. outdir .. "/%{prj.name}")
	
	files {
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.c"
	}

	includedirs {
                "%{include_dirs.src}",
		"%{include_dirs.graphics}"
        }
	
	libdirs {
		"%{lib_dirs.graphics}"
	}
	
	links {
		"SDL2.lib",
		"SDL2main.lib",
		"Vulkan-1.lib",
		"volk.lib"
	}
	
        defines {
                "AX_EXPORT_DLL"
        }
	
	filter "configurations:Debug"
		defines         { "AX_CONFIG_DEBUG", "AX_ENABLE_ASSERTIONS" }
                runtime         "Debug"
		symbols         "On"

	filter "configurations:Release"
                defines         "AX_CONFIG_RELEASE"
		runtime         "Release"
		optimize        "On"
		
	filter "system:windows"
		systemversion           "latest"
                postbuildcommands	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outdir .. "/Sandbox")

        filter "not system:windows"
                postbuildcommands       ("cp %{cfg.buildtarget.relpath} ../bin/" .. outdir .. "/Sandbox")
		
		
project "Sandbox"
        location        	"Sandbox"
        kind            	"ConsoleApp"
        language        	"C"
        floatingpoint   	"Fast"
        compileas       	"C"
	
        targetdir       	("bin/" .. outdir .. "/%{prj.name}")
	objdir          	("obj/" .. outdir .. "/%{prj.name}")

	files {
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.c"
	}

	includedirs {
		"%{include_dirs.src}",
		"%{include_dirs.graphics}"
	}

	libdirs {
		"%{lib_dirs.graphics}"
	}

	links {
                "Engine",
		
		"SDL2.lib",
		"SDL2main.lib"
	}

        defines {
                "AX_EXPORT_DLL"
        }

	filter "configurations:Debug"
		defines         { "AX_CONFIG_DEBUG", "AX_ENABLE_ASSERTIONS" }
                runtime         "Debug"
		symbols         "On"

	filter "configurations:Release"
                defines         "AX_CONFIG_RELEASE"
		runtime         "Release"
		optimize        "On"

	filter "system:windows"
		systemversion           "latest"
		prebuildcommands	"call \"Shaders/compile_shaders.bat\""
		
		
		