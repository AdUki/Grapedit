::
:: This batch will only set up environment variables for MSVC 11 and run cmake
::

:: For OpenGL on windows you must have Windows SDK installed and included in your path
set PATH=C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x86;%PATH%

::Set up Lua dir, preferably LuaJIT
set PATH=D:\LuaJIT\2.0.2\bin;%PATH%
set LUA_DIR=D:\LuaJIT\2.0.2

:: Set up boost
set BOOST_ROOT=D:\msvc11\boost_1_55_0_x86\
set BOOST_LIBRARYDIR=D:\msvc11\boost_1_55_0_x86\lib32-msvc-11.0

:: Set up Qt5
set PATH=D:\Qt\5.2.0\msvc2010_opengl\bin;%PATH%
set QTDIR=D:\Qt\5.2.0\msvc2010_opengl

:: Run CMake Visual Studio 2012 generator
cmake -G "Visual Studio 11"

:: After cmd is available, type Grapedit.sln to open Visual Studio with set up environment
cmd