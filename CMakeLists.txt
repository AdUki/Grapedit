cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(PrecompiledHeader)

# Set precompiled header file, on MSVC must be top level!
set(PC_HEADER "pch")

# Declare project variables...

set (PROJECT "Grapedit")

# Search for sources in src dir
AUX_SOURCE_DIRECTORY(src SOURCE_FILES)
set(SOURCE_FILES
	${SOURCE_FILES}
	${PC_HEADER}.h
	${PC_HEADER}.cpp
 )

set(UI_FILES
	ui/mainwindow.ui
)

# Set project name
project(${PROJECT})

# Qt Stuff
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Boost stuff
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)

# Find packages...

find_package(Boost 1.55)
include_directories(${Boost_INCLUDE_DIRS})

# Will find also LuaJIT, but must be named same as Lua51 and installed into directories
find_package(Lua51)
include_directories(${LUA_INCLUDE_DIR})

# Find Qt modules, every module separately
find_package(Qt5Widgets REQUIRED)

# Create nice groups in IDEs
source_group("UI Files" FILES ${UI_FILES})
source_group("Generated UI Headers" FILES ${UI_HEADERS})

# Use Qt UI files
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# Create executable
add_executable (
	${PROJECT} 
	${SOURCE_FILES} 
	${UI_HEADERS} 
	${UI_FILES}
)

# Link libraries... 
# Must be after executable is created!

# Qt with modules
qt5_use_modules (
	${PROJECT} 
	Widgets
)

# Boost
target_link_libraries(${PROJECT} ${Boost_LIBRARIES})

# LuaJIT
target_link_libraries(${PROJECT} ${LUA_LIBRARIES})

# Add precompiled header
add_precompiled_header(${PROJECT} ${PC_HEADER}.h FORCEINCLUDE)

# Will not show new windows prompt when running program
# But also you will don't have aplication output :(
#if (MSVC)
#  set_target_properties(${PROJECT} PROPERTIES
#    WIN32_EXECUTABLE YES
#    LINK_FLAGS "/ENTRY:mainCRTStartup"
#  )
#endif ()
