cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(PrecompiledHeader)

# Set precompiled header file, on MSVC must be top level!
set(PC_HEADER "pch")

# Declare project variables...

set (PROJECT "Grapedit")

# Must be executed BEFORE add_executable!
macro(add_folder FOLDER)
	file(GLOB SOURCE_GROUP ${FOLDER}/*.c ${FOLDER}/*.h ${FOLDER}/*.cpp ${FOLDER}/*.hpp)
	string(REGEX REPLACE "/" "\\\\\\\\" SOURCE_GROUP_NAME ${FOLDER})
	source_group(${SOURCE_GROUP_NAME} FILES ${SOURCE_GROUP})
	set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_GROUP})
endmacro()

add_folder("src")
add_folder("src/Data")
add_folder("src/Ui")
add_folder("src/Utils")

set(SOURCE_FILES ${SOURCE_FILES}
    ${PC_HEADER}.h
    ${PC_HEADER}.cpp
)

file(GLOB UI_FILES "ui/*.ui")

# Set project name
project(${PROJECT})

add_definitions(
    -std=c++11
    -DUSING_PCH
)

# Qt Stuff
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(QT_PRECOMPILED_HEADER ${PC_HEADER}.h)

# Find packages...

include_directories("libs/LuaState/include")

# Boost
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# Will find also LuaJIT
find_package(LuaJIT)
include_directories(${LUA_INCLUDE_DIR})

# Find Qt modules, every module separately
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Create nice groups in IDEs
source_group("UI Files" FILES ${UI_FILES})
source_group("UI Headers" FILES ${UI_HEADERS})

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
target_link_libraries(${PROJECT} ${LUA_LIBRARY})

# Add precompiled header
add_precompiled_header(${PROJECT} ${PC_HEADER}.h FORCEINCLUDE)

# Flags for MacOS which links directly or indirectly against LuaJIT
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Compile lua files
add_custom_command(
	TARGET ${PROJECT}
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
		${CMAKE_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:Grapedit>/scripts)
