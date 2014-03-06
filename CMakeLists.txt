cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(PrecompiledHeader)

# Set precompiled header file, on MSVC must be top level!
set(PC_HEADER "pch")

# Declare project variables...

set (PROJECT "Grapedit")

set(SOURCE_GROUP
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/Console.h
    src/Console.cpp
)
source_group("src" FILES ${SOURCE_GROUP})
set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_GROUP})

set(SOURCE_GROUP
    src/Data/GraphicText.h
    src/Data/GraphicText.cpp
    src/Data/GraphicElement.h
    src/Data/GraphicElement.cpp
    src/Data/GraphicTextState.h
    src/Data/GraphicTextState.cpp
)
source_group("src\\Data" FILES ${SOURCE_GROUP})
set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_GROUP})

set(SOURCE_GROUP
    src/Ui/BaseElement.h
    src/Ui/BaseElement.cpp
    src/Ui/BaseItem.h
    src/Ui/BaseItem.cpp
    src/Ui/TextItem.h
    src/Ui/TextItem.cpp
    src/Ui/BaseGrid.h
    src/Ui/BaseGrid.cpp
    src/Ui/LinearGrid.h
    src/Ui/LinearGrid.cpp
)
source_group("src\\Ui" FILES ${SOURCE_GROUP})
set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_GROUP})

set(SOURCE_GROUP
    src/LuaBindings/CoreBindings.h
    src/LuaBindings/CoreBindings.cpp
    src/LuaBindings/ElementBindings.h
    src/LuaBindings/ElementBindings.cpp
)
source_group("src\\LuaBindings" FILES ${SOURCE_GROUP})
set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_GROUP})

set(SOURCE_GROUP
    src/LuaUtils/Argument.h
    src/LuaUtils/Argument.cpp
    src/LuaUtils/LuaWorker.h
    src/LuaUtils/LuaWorker.cpp
)
source_group("src\\LuaUtils" FILES ${SOURCE_GROUP})
set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_GROUP})

set(SOURCE_FILES ${SOURCE_FILES}
    ${PC_HEADER}.h
    ${PC_HEADER}.cpp
)

set(UI_FILES
	ui/mainwindow.ui
)

# Set project name
project(${PROJECT})

ADD_DEFINITIONS(
    -std=c++11
    -DUSING_PCH
)

# Qt Stuff
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(QT_PRECOMPILED_HEADER "pch.h")

# Boost stuff
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)

# Find packages...

find_package(Boost 1.55)
include_directories(${Boost_INCLUDE_DIRS})

# Will find also LuaJIT, but must be named same as Lua51 and installed into directories
# find_package(Lua51)
find_package(LuaJIT)
include_directories(${LUAJIT_INCLUDE_DIR})

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
target_link_libraries(${PROJECT} ${LUAJIT_LIBRARY})

# Add precompiled header
add_precompiled_header(${PROJECT} ${PC_HEADER}.h FORCEINCLUDE)

# Flags for MacOS which links directly or indirectly against LuaJIT
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
