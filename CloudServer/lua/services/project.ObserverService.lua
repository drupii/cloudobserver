project "ObserverService"
	kind "SharedLib"
	language "C++"
	location ( "projects/" .. os.get() .. "-" .. action )
	
	cloud.project.init()

	links { "cf-util" }
	includedirs { "src/cf-util"}
	includedirs { "3rdparty/cf-http" }
	
	configuration { "gmake" }
		linkoptions {
			"-lcf-util"
		}
	configuration { }
	
	cloud.project.useBoost()
	cloud.project.useopenSSL()
	
	files { "src/default-services/observer_service/**"}

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" , "Unicode"}
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "OptimizeSpeed" , "Unicode"}