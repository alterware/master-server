dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

dependencies.load()

workspace "alterware-master"
startproject "alterware-master"
location "./build"
objdir "%{wks.location}/obj"
targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

configurations {"debug", "release"}

language "C++"
cppdialect "C++20"

if os.istarget("darwin") then
	platforms {"x64", "arm64"}
else
	platforms {"x86", "x64", "arm64"}
end

filter "platforms:x86"
architecture "x86"
filter {}

filter "platforms:x64"
architecture "x86_64"
filter {}

filter "platforms:arm64"
architecture "ARM64"
filter {}

symbols "On"
staticruntime "On"
editandcontinue "Off"
warnings "Extra"
characterset "ASCII"

filter { "system:linux", "system:macosx" }
	buildoptions "-pthread"
	linkoptions "-pthread"
filter {}

if os.istarget("linux") then
	filter { "toolset:clang*", "platforms:arm64" }
		buildoptions "--target=arm64-linux-gnu"
		linkoptions "--target=arm64-linux-gnu"
	filter {}

	filter { "toolset:clang*" }
		buildoptions "-stdlib=libc++"
		linkoptions "-stdlib=libc++"

		-- always try to use lld. LD or Gold will not work
		linkoptions "-fuse-ld=lld"
	filter {}
end

filter { "system:macosx", "platforms:arm64" }
	buildoptions "-arch arm64"
	linkoptions "-arch arm64"
filter {}

if _OPTIONS["dev-build"] then
	defines {"DEV_BUILD"}
end

if os.getenv("CI") then
	defines "CI"
end

flags {"NoIncrementalLink", "NoMinimalRebuild", "MultiProcessorCompile", "No64BitChecks"}

filter "configurations:release"
	optimize "Size"
	defines "NDEBUG"
	flags "FatalCompileWarnings"
filter {}

filter "configurations:debug"
	optimize "Debug"
	defines {"DEBUG", "_DEBUG"}
filter {}

project "alterware-master"
kind "ConsoleApp"
language "C++"

pchheader "std_include.hpp"
pchsource "src/std_include.cpp"

files {"./src/**.rc", "./src/**.hpp", "./src/**.cpp"}

includedirs {"./src", "%{prj.location}/src"}

filter "system:windows"
	files {
		"./src/**.rc",
	}
filter {}

filter { "system:windows", "toolset:not msc*" }
	resincludedirs {
		"%{_MAIN_SCRIPT_DIR}/src"
	}
filter {}

filter { "system:windows", "toolset:msc*" }
	resincludedirs {
		"$(ProjectDir)src"
	}
filter {}

dependencies.imports()


group "Dependencies"
dependencies.projects()
