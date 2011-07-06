-- A solution contains projects, and defines the available configurations
-- Action name is defined by os

local action = _ACTION or ""

if action == "clean" then
	os.rmdir("projects/**")
	os.rmdir("projects")
end

-- Options for libs search

newoption {
	trigger     = "libsPath",
	value       = "PATH",
	description = "Choose a particular directory for general libs search. ';' separators are allowed like --libsPath=C:/src/Libs;C:/src/MoreLibs"
}

newoption {
	trigger     = "BoostLibsPath",
	value       = "PATH",
	description = "Choose a particular directory for Boost libs search"
}

newoption {
	trigger     = "FFmpegLibsPath",
	value       = "PATH",
	description = "Choose a particular directory for FFMpeg libs search"
}

newoption {
	trigger     = "OpenALLibsPath",
	value       = "PATH",
	description = "Choose a particular directory for OpenAL libs search"
}

newoption {
	trigger     = "OpenCVLibsPath",
	value       = "PATH",
	description = "Choose a particular directory for OpenCV libs search"
}

newoption {
	trigger     = "TclLibsPath",
	value       = "PATH",
	description = "Choose a particular directory for Tcl libs search"
}

-- Options for includes

newoption {
	trigger     = "includesPath",
	value       = "PATH",
	description = "Choose a particular directory for general includes search. ';' separators are allowed like --includesPath=C:/src/Libs;C:/src/MoreLibs "
}

newoption {
	trigger     = "BoostIncludesPath",
	value       = "PATH",
	description = "Choose a particular directory for Boost includes search"
}

newoption {
	trigger     = "FFmpegIncludesPath",
	value       = "PATH",
	description = "Choose a particular directory for FFMpeg includes search"
}

newoption {
	trigger     = "OpenALIncludesPath",
	value       = "PATH",
	description = "Choose a particular directory for OpenAL includes search"
}


newoption {
	trigger     = "OpenCVIncludesPath",
	value       = "PATH",
	description = "Choose a particular directory for OpenCV includes search"
}

newoption {
	trigger     = "TclIncludesPath",
	value       = "PATH",
	description = "Choose a particular directory for Tcl includes search"
}

newoption {
	trigger     = "c99IncludesPath",
	value       = "PATH",
	description = "Choose a particular directory for C99 includes search if it is not defined on your system by defauft. WINDOWS-MSVC-SPECIFIC-REQUIRED-FOR-FFMPEG-OPTION! because on 2011.03.** msvc++ does not support C99 and FFmpeg release requires C99 there are errors compiling in VS environment. Choose a particular directory for C99 includes search. Problem described here: http://ffmpeg.arrozcru.org/wiki/index.php?title=Inttypes.h . Currently required files could be found here http://code.google.com/p/msinttypes/downloads/list ."
}

-- Options for Shared Libs

newoption {
	trigger     = "soPath",
	value       = "PATH",
	description = "Choose a particular directory for general Shared Libs (dll, so...) search. ';' separators are allowed"
}

-- Script Options

newoption {
	trigger     = "CopySharedLibraries",
	value       = "true",
	description = "Set if we shall copy shared librarys into application build folders"
}

newoption {
	trigger     = "DiscLetterWithOs",
	value       = "L",
	description = "Choose a particular disc letter where the system is installed (used only in case you followed read me and installed all librarys into your system Programm Files folder but system disc letter is not 'C' )"
}

if  _OPTIONS["DiscLetterWithOs"] then
	disc = _OPTIONS["DiscLetterWithOs"] 
else
	disc = "C"
end

cloud = { }

function os.copydir(src_dir, dst_dir, filter, single_dst_dir)
	if not os.isdir(src_dir) then error(src_dir .. " is not an existing directory!") end
	filter = filter or "**"
	src_dir = src_dir .. "/"
	print('copy "' .. src_dir .. filter .. '" to "' .. dst_dir .. '".')
	dst_dir = dst_dir .. "/"
	local dir = path.rebase(".",path.getabsolute("."), src_dir) -- root dir, relative from src_dir
 
	os.chdir( src_dir ) -- change current directory to src_dir
		local matches = os.matchfiles(filter)
	os.chdir( dir ) -- change current directory back to root
 
	local counter = 0
	for k, v in ipairs(matches) do
		local target = iif(single_dst_dir, path.getname(v), v)
		--make sure, that directory exists or os.copyfile() fails
		os.mkdir( path.getdirectory(dst_dir .. target))
		if os.copyfile( src_dir .. v, dst_dir .. target) then
			counter = counter + 1
		end
	end
 
	if counter == #matches then
		print( counter .. " files copied.")
		return true
	else
		print( "Error: " .. counter .. "/" .. #matches .. " files copied.")
		return nil
	end
end

function cloud.addLibDir(path)
	--
	if  path then
		libdirs {  string.explode(path , ";") }
	end
end

function cloud.addIncludeDir(path)
	--
	if path then
		includedirs {  string.explode(path , ";") }
	end
end

function cloud.copyFileIntoBuild(filePath, fileName)
	--
	if not os.isdir("projects/" .. os.get() .. "-" .. action .. "/bin/debug" ) then
		os.mkdir("projects/" .. os.get() .. "-" .. action .. "/bin/debug")
	end
	if not os.isdir("projects/" .. os.get() .. "-" .. action .."/bin/release" ) then
		os.mkdir("projects/" .. os.get() .. "-" .. action .. "/bin/release")
	end
	os.copyfile(filePath , "projects/" .. os.get() .. "-" .. action .. "/bin/debug/" .. fileName  )
	os.copyfile(filePath , "projects/" .. os.get() .. "-" .. action .. "/bin/release/" .. fileName )
end

cloud.win = { }

function cloud.win.addLibDirFromProgrammFiles(relativePath)
	--
	if os.isdir( disc .. ":/Program Files (x86)/" .. relativePath ) then
		libdirs{disc .. ":/Program Files (x86)/" .. relativePath}
	end
	if os.isdir( disc .. ":/Program Files/" .. relativePath ) then
		libdirs{disc .. ":/Program Files/" .. relativePath}
	end
end

function cloud.win.addIncludeDirFromProgrammFiles(relativePath)
	--
	if os.isdir( disc .. ":/Program Files (x86)/" .. relativePath ) then
		includedirs{disc .. ":/Program Files (x86)/" .. relativePath}
	end
	if os.isdir( disc .. ":/Program Files/" .. relativePath ) then
		includedirs{disc .. ":/Program Files/" .. relativePath}
	end
end

function cloud.win.addLibFromProgrammFiles(folderName)
	--
	cloud.win.addIncludeDirFromProgrammFiles(folderName .. "/include")
	cloud.win.addLibDirFromProgrammFiles(folderName .. "/lib")
end

function cloud.win.addLibFromProgrammFiles2(folderName, specialLibraryFolder)
	--
	cloud.win.addIncludeDirFromProgrammFiles(folderName .. "/include")
	cloud.win.addLibDirFromProgrammFiles(folderName .. "/" .. specialLibraryFolder)
end

function cloud.win.copyDLL( folderName ,fileName)
	--
	if _OPTIONS["soPath"] then
		pa = os.pathsearch(fileName, disc .. ":/Program Files (x86)/" .. folderName .. "/bin", disc .. ":/Program Files/" .. folderName .. "/bin", string.explode( _OPTIONS["soPath"] , ";") )  .. "/".. fileName
		if pa then
			cloud.copyFileIntoBuild(pa, fileName)
		end
	else
		pa = os.pathsearch(fileName,  fileName, disc .. ":/Program Files (x86)/" .. folderName .. "/bin", disc .. ":/Program Files/" .. folderName .. "/bin") .. "/".. fileName
		if pa then
			cloud.copyFileIntoBuild(pa, fileName)
		end
	end
	if pa then
		printf("file: ".. fileName.. " copied into builds dirs" )
	end
end

cloud.project = {}

function cloud.project.init()
	--
	cloud.addLibDir(  _OPTIONS["libsPath"] )
	cloud.addIncludeDir( _OPTIONS["includesPath"] )
	if os.get() == "windows" then
		defines { "WIN" }
	end
	
	if os.get() == "linux" then
		defines { "LIN" }
		libdirs {
		"/opt/local/lib",
		"/usr/lib",
		"/usr/local/lib"
		}
		
		includedirs {
		"/opt/local/include",
		"/usr/include",
		"/usr/local/include"
		}	 
	end
	
	if os.get() == "macosx" then
		defines { "MAC" }
		links {
		"Carbon.framework"
		}
		
		libdirs {
		"/usr/lib",
		"/usr/local/lib"
		}
		
		includedirs {
		"/usr/include",
		"/usr/local/include"
		}
	end 
end

function cloud.project.useCV()
	--
	cloud.addLibDir( _OPTIONS["OpenCVLibsPath"] )
	cloud.addIncludeDir(  _OPTIONS["OpenCVIncludesPath"] )
	if os.get() == "windows" then
		defines { "WIN" }
		links {
		"opencv_core220",
		"opencv_highgui220",
		"opencv_imgproc220"
		}
		
		cloud.win.addLibFromProgrammFiles("OpenCV-2.2.0")
		
		if  _OPTIONS["CopySharedLibraries"] then
			cloud.win.copyDLL("OpenCV-2.2.0", "opencv_core220.dll")
			cloud.win.copyDLL("OpenCV-2.2.0", "opencv_highgui220.dll")
			cloud.win.copyDLL("OpenCV-2.2.0", "opencv_imgproc220.dll")
		end
	end
	
	if os.get() == "linux" then
		defines { "LIN" }
		links {
		"opencv_core",
		"opencv_highgui",
		"opencv_imgproc"
		}	
	end
	
	if os.get() == "macosx" then
		defines { "MAC" }
		links {
		"opencv_core",
		"opencv_highgui",
		"opencv_imgproc",
		"QuickTime.framework"
		}	 
	end 
end

function cloud.project.useAL()
	--
	cloud.addIncludeDir(  _OPTIONS["OpenALIncludesPath"] )
	cloud.addLibDir(  _OPTIONS["OpenALLibsPath"] )
	if os.get() == "windows" then
		defines { "WIN" }
		links {
		"openal32"
		}
		cloud.win.addLibFromProgrammFiles("OpenAL-1.13")
		if  _OPTIONS["CopySharedLibraries"] then
			cloud.win.copyDLL("OpenAL-1.13", "OpenAL32.dll")
		end
	end
	
	if os.get() == "linux" then
		defines { "LIN" }
		links {
		"openal"
		}
	end
	
	if os.get() == "macosx" then
		defines { "MAC" }
		links {
		"openal"
		}
	end 
end
	
function cloud.project.useBoost()
	--
	cloud.addIncludeDir( _OPTIONS["BoostIncludesPath"] )
	cloud.addLibDir( _OPTIONS["BoostLibsPath"])
	if os.get() == "windows" then
		defines { "WIN" } 
		cloud.win.addLibFromProgrammFiles("Boost-1.46.1")
	end	
	if os.get() == "linux" then
		defines { "LIN" }
		links {
		"boost_regex",
		"boost_system",
		"boost_thread"
		}
	end
	if os.get() == "macosx" then
		defines { "MAC" }
		links {
		"boost_regex",
		"boost_system",
		"boost_thread"
		}	
	end 
end

function cloud.project.useFFmpeg()
	--
	cloud.addLibDir(  _OPTIONS["FFmpegLibsPath"] )
	cloud.addIncludeDir(  _OPTIONS["FFmpegIncludesPath"] )
	cloud.addIncludeDir( _OPTIONS["c99IncludesPath"] 	) 
	if os.get() == "windows" then
		defines { "WIN" }
		links {
		"avcodec-52",
		"avformat-52",
		"avutil-50",
		"swscale-0"
		}
		cloud.win.addLibFromProgrammFiles2("FFmpeg-0.6.1", "bin")
		if  _OPTIONS["CopySharedLibraries"] then
			cloud.win.copyDLL("FFmpeg-0.6.1", "avcodec-52.dll")
			cloud.win.copyDLL("FFmpeg-0.6.1", "avformat-52.dll")
			cloud.win.copyDLL("FFmpeg-0.6.1", "avutil-50.dll")
			cloud.win.copyDLL("FFmpeg-0.6.1", "swscale-0.dll")
		end
	end
	
	if os.get() == "linux" then
		defines { "LIN" }
		links {
		"avcodec", 
		"avformat",
		"avutil",
		"swscale"
		}
	end
	
	if os.get() == "macosx" then
		defines { "MAC" }
		links {
		"avcodec", 
		"avformat",
		"avutil",
		"swscale"
		}
	end 	
end

function cloud.project.useTcl()
	--
	cloud.addLibDir( _OPTIONS["TclLibsPath"] )
	cloud.addIncludeDir(  _OPTIONS["TclIncludesPath"] )
	if os.get() == "windows" then
		defines { "WIN" }
		links {
		"tcl85",
		"tk85"
		}
		cloud.win.addLibFromProgrammFiles("Tcl")
	end
	
	if os.get() == "linux" then
		defines { "LIN" }
		links {
		"tcl8.5",
		"tk8.5"
		}	
	end
	
	if os.get() == "macosx" then
		defines { "MAC" }
		links {
		"tcl8.5",
		"tk8.5"
		}	 
	end
end

function cloud.project.copyAssets()
	cwd = os.getcwd()
	if not os.isdir("projects/" .. os.get() .. "-" .. action .. "/bin/debug/assets" ) then
		os.mkdir("projects/" .. os.get() .. "-" .. action .. "/bin/debug/assets")
	end
	if not os.isdir("projects/" .. os.get() .. "-" .. action .."/bin/release/assets" ) then
		os.mkdir("projects/" .. os.get() .. "-" .. action .. "/bin/release/assets")
	end
	os.copydir("assets",  "projects/" .. os.get() .. "-" .. action .. "/bin/debug/assets")
	os.copydir("assets",  "projects/" .. os.get() .. "-" .. action .. "/bin/release/assets")
	os.copydir("assets",  "projects/" .. os.get() .. "-" .. action .. "/assets")
end

solution "AudioLatencyTester"
	location ( "projects/".. os.get() .. "-" ..  action )
	configurations { "Debug", "Release" }
	objdir     ( "projects/" .. os.get() .. "-" .. action .. "/bin/obj" )
	
	configuration { "Debug" }
		targetdir ( "projects/" .. os.get() .. "-" .. action .. "/bin/debug" )
	
	configuration { "Release" }
		targetdir ( "projects/" .. os.get() .. "-" .. action ..  "/bin/release" )
	
	-- A project defines one build target
	
	project "AudioLatencyTester"
		kind "ConsoleApp"
		language "C++"
		location ( "projects/" .. os.get() .. "-" .. action )
		cloud.project.init()
		cloud.project.useFFmpeg()
		cloud.project.useBoost()
		cloud.project.useAL()
		cloud.project.useCV()
		cloud.project.useTcl()
		
		cloud.project.copyAssets()
		
		files { "src/**.h", "src/**.cpp" }
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
		
		
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }