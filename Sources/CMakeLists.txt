cmake_minimum_required(VERSION 3.5)
project(SeriousEngine)

# set minimal standart
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set @rpath for Mac OS X shared library install names.
#cmake_policy(SET CMP0042 NEW)

# Use system SDL2 is on by default
option(USE_SYSTEM_SDL2 "Use systems sdl2 development files" On)
option(USE_CCACHE "Set to ON to use ccache if present in the system" ${USE_CCACHE})

# fallback for cmake versions without add_compile_options # RAKE! Borrowed from dhewm3 project
if(NOT COMMAND add_compile_options)
	function(add_compile_options)
		foreach(arg ${ARGN})
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${arg}" PARENT_SCOPE)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${arg}" PARENT_SCOPE)
		endforeach()
	endfunction()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(CheckCXXCompilerFlag)

# SeriousSam expects the libs to be in Debug/
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Debug)

if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()

# Use systemwide SDL2 or custom build
# RAKE!: Find a way to use their custom built library if
# they want to use that instead or if their system only
# allows for a setup like this. Maybe use a SDL2_DIR var or
# some thing set in the system enviroment.
if(NOT USE_SYSTEM_SDL2)
    include_directories(${CMAKE_SOURCE_DIR}/External/SDL2)
else()
    find_package(SDL2 REQUIRED)
    if(SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "Error USE_SYSTEM_SDL2 is set but neccessary developer files are missing")
    endif()
endif()

# RAKE! Where to install the binaries.
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" OR CMAKE_INSTALL_PREFIX STREQUAL "") # Only works for linux since I don't
                                                                                   # know what default is for windows/macos/freebsd.
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../") # set install path to project root directory since
                                                        # since one wasn't set during config
    set(LOCAL_INSTALL TRUE)
endif()
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr" ) # Only works for linux since I don't know what default is for windows/macos/freebsd.
    set(LOCAL_INSTALL FALSE)
endif()

set(INTERNAL_NAME "serioussamse")

# Set up some sanity stuff...
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "GNU")
    SET(LINUX TRUE)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wno-reorder -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-class-memaccess -Wno-unused-function -Wno-format")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wno-reorder -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-class-memaccess -Wno-unused-function -Wno-format")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    SET(FREEBSD TRUE)
endif()
if(APPLE)
    SET(MACOSX TRUE)
endif()
if(MSVC)
    SET(WINDOWS TRUE)
endif()

# remove clang spam.
if(MACOSX OR FREEBSD)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-nullability-completeness")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nullability-completeness")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "None Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()
SET(DEBUG FALSE)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(DEBUG TRUE)
endif()

if (LINUX)
    set (CMAKE_OS_NAME "GNU/Linux" CACHE STRING "Operating system name" FORCE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
      set (CMAKE_OS_NAME "Debian" CACHE STRING "Operating system name" FORCE)
    endif (DEBIAN_FOUND)
    ##  Check for Fedora
    find_file (FEDORA_FOUND fedora-release
      PATHS /etc
      )
    if (FEDORA_FOUND)
      set (CMAKE_OS_NAME "Fedora" CACHE STRING "Operating system name" FORCE)
    endif (FEDORA_FOUND)
    ##  Check for RedHat
    find_file (REDHAT_FOUND redhat-release inittab.RH
      PATHS /etc
      )
    if (REDHAT_FOUND)
      set (CMAKE_OS_NAME "RedHat" CACHE STRING "Operating system name" FORCE)
    endif (REDHAT_FOUND)   
    ## Extra check for Ubuntu    
    if (DEBIAN_FOUND)
      ## At its core Ubuntu is a Debian system, with
      ## a slightly altered configuration; hence from
      ## a first superficial inspection a system will
      ## be considered as Debian, which signifies an
      ## extra check is required.
      find_file (UBUNTU_EXTRA legal issue
	PATHS /etc
	)
    if (UBUNTU_EXTRA)
	## Scan contents of file
	file (STRINGS ${UBUNTU_EXTRA} UBUNTU_FOUND
	  REGEX Ubuntu
	  )
	## Check result of string search
	if (UBUNTU_FOUND)
	  set (CMAKE_OS_NAME "Ubuntu" CACHE STRING "Operating system name" FORCE)
	  set (DEBIAN_FOUND FALSE)
	endif (UBUNTU_FOUND)
    endif (UBUNTU_EXTRA)      
    endif (DEBIAN_FOUND)
endif (LINUX)

message(STATUS "Operating system name: ${CMAKE_OS_NAME}")

set(CMAKE_ARCH_BITS 32)
set(CMAKE_INSTALL_LIBDIR "/usr/lib")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_ARCH_BITS 64)
    set(CMAKE_INSTALL_LIBDIR "/usr/lib64")
endif()

message(STATUS "Arch bits: ${CMAKE_ARCH_BITS}")

if(DEBIAN_FOUND OR UBUNTU_FOUND)
    #set libdir for debian based OS
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "riscv64")
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/riscv64-linux-gnu")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")  # 32bit)
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/arm-linux-gnueabihf")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*)")
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/aarch64-linux-gnu")
        if(NOT "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            message(FATAL_ERROR "invalid pointer size")
        endif()
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)64")
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/powerpc64-linux-gnu")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL s390x)
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/s390x-linux-gnu")
    elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86|AMD64") AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/i386-linux-gnu")
    elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64") AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/x86_64-linux-gnu")
    elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (APPLE))
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/i386-linux-gnu")
    else()
        message(FATAL_ERROR "Unknown architecture")
    endif()
endif()

message(STATUS "Installed libraries dir: ${CMAKE_INSTALL_LIBDIR}")

## ** RAKE! start compiler specific flags section **
## ** RAKE! Borrowed from dhewm3 project, need to **
## ** RAKE! clean up for SeriousEngine use. Also  **
## ** RAKE! need to make this pandora safe.	  **
# compiler specific flags
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
	# This section and the like are for flags/defines that can be shared between 
	# c and c++ compile options
	add_compile_options(-Wall)
	add_compile_options(-pipe)
	add_compile_options(-fPIC)
	add_compile_options(-fms-extensions)
	if(NOT PANDORA AND NOT PYRA AND NOT RPI4 AND NOT (MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64"))
		if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "riscv64")
			add_compile_options(-march=rv64gc)
		    add_compile_options(-ftree-vectorize)
		    add_compile_options(-fomit-frame-pointer)
			message(STATUS "Architecture RISCV64, -march=rv64gc")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)64")
			add_compile_options(-mpowerpc64)
			#add_compile_options(-m64)
		else()
            if(LOCAL_INSTALL)
			    add_compile_options(-march=native)
            elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i586|i686|x86|AMD64|x86_64")
			    add_compile_options(-march=native) # set mtune=generic if nedded
		    endif()
		endif()
	endif()
	if((PYRA OR PANDORA)  AND (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm.*"))
		if(PYRA)
		add_compile_options(-mfpu=neon-vfpv4)
		add_compile_options(-mcpu=cortex-a15)
		add_compile_options(-mfloat-abi=hard)
		add_compile_options(-marm)
                elseif(NOT (MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64"))
                    add_compile_options(-mfpu=neon)
		elseif(PANDORA)
		add_compile_options(-mfpu=neon)
		endif()
		add_compile_options(-fsigned-char)
	endif()
    if(NOT PYRA AND NOT PANDORA AND ${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "arm*")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")  # 32bit
            add_compile_options(-march=armv8-a+crc)
		    add_compile_options(-mfpu=neon-fp-armv8)
            if(RPI4) 
		        add_compile_options(-mcpu=cortex-a72)
            endif()
		    add_compile_options(-mfloat-abi=hard)
		    add_compile_options(-ftree-vectorize)
		    add_compile_options(-fomit-frame-pointer)
            message(STATUS "ARM 32bit, -march=armv8-a+crc, adding -mfpu=neon-fp-armv8")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*)")  # NEON is implied on aarch64
            if(NOT "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
                message(FATAL_ERROR "invalid pointer size")
            endif()
            add_compile_options(-march=armv8-a+crc+fp+simd)
            if(RPI4) 
		        add_compile_options(-mcpu=cortex-a72)
            endif()
		    add_compile_options(-ftree-vectorize)
		    add_compile_options(-fomit-frame-pointer)
            message(STATUS "AARCH64, -march=armv8-a+crc+fp+simd")
        else()
            message(FATAL_ERROR "Unknown ARM processor '${CMAKE_SYSTEM_PROCESSOR}'")
        endif()
    endif()
    add_compile_options(-fno-strict-aliasing)
	add_definitions(-D_REENTRANT=1)
	add_definitions(-D_MT=1)

	## Add your custom C and CXX flags on the command line aka -DCMAKE_C_FLAGS=-std=c98 or -DCMAKE_CXX_FLAGS=-std=c++11

	## For C flags
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -D_DEBUG=1 -DDEBUG=1 -O0")
        if(PANDORA)
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -faligned-new -ffast-math")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2  -faligned-new -ffast-math")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -ffast-math")
        elseif(PYRA)
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -marm -faligned-new -ffast-math")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -marm -faligned-new -ffast-math")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -marm -ffast-math")
        elseif(MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O0 -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O0 -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O0 -fno-unsafe-math-optimizations")
        else()
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -fno-unsafe-math-optimizations")
        endif()

	## For C++ flags
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -D_DEBUG=1 -DDEBUG=1 -O0")
        if(PANDORA)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O2  -faligned-new -ffast-math")
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2  -faligned-new -ffast-math")
            set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -ffast-math")
        elseif(PYRA)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O2 -marm -faligned-new -ffast-math")
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -marm -faligned-new -ffast-math")
            set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -marm -ffast-math")
        elseif(MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O0 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O0 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations") ## RAKE! Does -DNDEBUG=1 and -D_NDEBUG=1 mess with RelWithDebInfo?
            set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O0 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
        else()
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations") ## RAKE! Does -DNDEBUG=1 and -D_NDEBUG=1 mess with RelWithDebInfo?
            set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
        endif()

	# TODO fix these warnings
	add_compile_options(-Wno-format-security)
	add_compile_options(-Wno-switch)
	add_compile_options(-Wno-char-subscripts)
	add_compile_options(-Wno-unknown-pragmas)
	add_compile_options(-Wno-unused-variable) # TODO: maybe only enable this for Entities
	add_compile_options(-Wno-unused-value)    # same here (the Scripts generate tons of unused variables and values)
	add_compile_options(-Wno-missing-braces)
	add_compile_options(-Wno-overloaded-virtual)
	add_compile_options(-Wno-invalid-offsetof)
	#MESSAGE(WARNING, "re-enable some of the warnings some day!")

	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
		# !!! FIXME: turn a bunch of these off, this is just for now. I hope.  --ryan.
		add_compile_options(-Wno-c++11-compat-deprecated-writable-strings)
	endif()

	if(MACOSX)
		add_definitions(-DPLATFORM_UNIX=1)
    		add_definitions(-DPLATFORM_MACOSX=1)
    		add_definitions(-DPRAGMA_ONCE=1)
    		include_directories("/usr/local/include")
    		include_directories("/usr/X11/include/")
	elseif(WINDOWS)
		add_definitions(-DPLATFORM_WIN32=1)
    		add_definitions(-DPRAGMA_ONCE=1)
                add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
                add_definitions(-D_CRT_SECURE_NO_DEPRECATE=1)
	elseif(LINUX)
	    	set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
    		add_definitions(-DPLATFORM_UNIX=1)
    		add_definitions(-D_FILE_OFFSET_BITS=64)
    		add_definitions(-D_LARGEFILE_SOURCE=1)
    		add_definitions(-DPRAGMA_ONCE=1)
	elseif(FREEBSD)
    		set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
    		add_definitions(-DPLATFORM_UNIX=1)
    		add_definitions(-DPLATFORM_FREEBSD=1)
    		add_definitions(-D_FILE_OFFSET_BITS=64)
    		add_definitions(-D_LARGEFILE_SOURCE=1)
    		add_definitions(-DPRAGMA_ONCE=1)
    		include_directories("/usr/local/include")
	endif()
	
        if(MACOSX OR LINUX OR FREEBSD)
		add_compile_options(-pthread)
                add_compile_options(-fsigned-char)
	endif()

	if(CMAKE_COMPILER_IS_GNUCC)
	    	# !!! FIXME: turn a bunch of these off, this is just for now. I hope.  --ryan.
    		add_compile_options(-Wno-invalid-offsetof)
	endif()

elseif(MSVC) # RAKE! I don't know if this will build with MSVC
	add_compile_options(/W4)
	add_compile_options(/wd4100) # unreferenced formal parameter
	add_compile_options(/wd4127) # conditional expression is constant
	add_compile_options(/wd4244) # possible loss of data
	add_compile_options(/wd4245) # signed/unsigned mismatch
	add_compile_options(/wd4267) # possible loss of data
	add_compile_options(/wd4714) # 'function' marked as __forceinline not inlined
	add_compile_options(/wd4996) # 'function': was declared deprecated
	add_compile_options(/wd4068) # unknown pragma
        set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG /Od /Zi /MDd")
	set(CMAKE_C_FLAGS_RELEASE "/Ox /Oy /MD")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "/Ox /Oy /Zi /MD")
	set(CMAKE_C_FLAGS_MINSIZEREL "/Ox /Oy /Os /MD")
else()
	message(FATAL_ERROR "Unsupported compiler")
endif()

## ** RAKE! end compiler specific flags section **

if(DEBUG)
    set(DEBUGSUFFIX "D")
else()
    set(DEBUGSUFFIX "")
endif()

option(PANDORA "Compile for Pandora" FALSE)
if (PANDORA)
    add_definitions(-DPLATFORM_PANDORA=1)
endif()

option(PYRA "Compile for Pyra" FALSE)
if (PYRA)
    add_definitions(-DPLATFORM_PYRA=1)
endif()

option(RPI4 "Raspberry PI 4" FALSE)
if(RPI4)
    add_definitions(-DPLATFORM_RPI4=1)
endif()

option(USE_TREMOR "Use Tremor instead of Vorbis" FALSE)
if (USE_TREMOR)
    add_definitions(-DUSE_TREMOR=1)
endif()

set(MP "MP")
include_directories(
    .
    ${CMAKE_SOURCE_DIR}/External/libogg/include
)
if(USE_TREMOR)
if(PANDORA)
    include_directories(/mnt/utmp/codeblocks/usr/include/tremor)
else()
    # !!!Do something here!
endif()
else()
    include_directories(External/libvorbis/include)
endif()

# We build ECC, then use it to generate C++ code for the game entities...
macro(add_parser_and_scanner _PARSER _SCANNER)
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${_SCANNER}.cpp"
        MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/${_SCANNER}.l"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND flex
        ARGS -o${_SCANNER}.cpp ${_SCANNER}.l
    )

    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${_PARSER}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${_PARSER}.hpp"
        MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/${_PARSER}.y"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND bison
        ARGS -o${_PARSER}.cpp ${_PARSER}.y -d
    )

    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${_PARSER}.h"
        MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/${_PARSER}.hpp"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${_PARSER}.hpp ${_PARSER}.h
    )
endmacro()

# Build ECC from source if there wasn't a prebuilt-one specified on the command line.
#  Normally we build it here, but we might need a prebuilt, native binary if
#  we're cross-compiling the rest of the game.
if(NOT ECC)
    add_parser_and_scanner("Ecc/Parser" "Ecc/Scanner")
    add_executable(ecc Ecc/Main.cpp Ecc/Parser.cpp Ecc/Parser.h Ecc/Scanner.cpp)
    set(ECC "ecc")
endif()

macro(entity _NAME)
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.h" "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}_tables.h"
        MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.es"
        DEPENDS ${ECC}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ${ECC}
        ARGS "${_NAME}.es"
    )
    list(APPEND ENTITIES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.cpp")
    list(APPEND ENTITIES_H "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.h")
    list(APPEND ENTITIES_H "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}_tables.h")
endmacro()

set(ENTITIES_CPP "")
set(ENTITIES_H "")

entity(Engine/Classes/BaseEvents)
entity(Engine/Classes/MovableBrushEntity)
entity(Engine/Classes/MovableEntity)
entity(Engine/Classes/MovableModelEntity)
entity(Engine/Classes/PlayerEntity)

set(ENGINE_ENTITIES_CPP ${ENTITIES_CPP})
set(ENTITIES_CPP "")

entity(EntitiesMP/Global)
entity(ParametricParticlesMP/AutoHeightMap)
entity(ParametricParticlesMP/ParametricParticles)
entity(ParametricParticlesMP/ParticleRotation)
entity(ParametricParticlesMP/ParticleVelocity)
entity(ParametricParticlesMP/SpawnShapeBase)
entity(ParametricParticlesMP/SpawnShapeBox)
entity(ParametricParticlesMP/SpawnShapeCylinder)
entity(ParametricParticlesMP/SpawnShapeSphere)

add_custom_target(ParseEntities DEPENDS ${ENTITIES_H})
set(ENTITIESMPLIB "Entities${MP}${DEBUGSUFFIX}")
add_library(${ENTITIESMPLIB} SHARED
     ${ENTITIES_CPP}
     ParametricParticlesMP/StdH/StdH.cpp
     ParametricParticlesMP/Particles.cpp
     ParametricParticlesMP/WeakPointer.cpp
)


if(MACOSX)
    target_link_libraries(${ENTITIESMPLIB} "-undefined dynamic_lookup")
endif()
add_dependencies(${ENTITIESMPLIB} ParseEntities)

add_parser_and_scanner("Engine/Base/Parser" "Engine/Base/Scanner")
add_parser_and_scanner("Engine/Ska/smcPars" "Engine/Ska/smcScan")

set_target_properties(Entities${MP} PROPERTIES OUTPUT_NAME "ParametricParticles${MP}")

# RAKE! Install Section.
if(LOCAL_INSTALL)
    if(DEBUG) # RAKE! Will this work with TFE?
        install(TARGETS ${ENTITIESMPLIB}
            LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/Bin/Debug"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    else()
        install(TARGETS ${ENTITIESMPLIB}
            LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/Bin"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    endif()
else()
    if(DEBUG) # RAKE! Will this work with TFE?
        install(TARGETS ${ENTITIESMPLIB}
            RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}/${INTERNAL_NAME}/Mods/OddWorld/Debug"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    else()
        install(TARGETS ${ENTITIESMPLIB}
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${INTERNAL_NAME}/Mods/OddWorld"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    endif()
endif()

# end of CMakeLists.txt ...
