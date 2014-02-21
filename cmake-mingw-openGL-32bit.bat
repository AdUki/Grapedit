::
:: This batch will only set up environment variables for MinGW Codeblocks and run cmake
::

::Set up Lua dir, preferably LuaJIT
set PATH=C:\LuaJIT-2.0.2\bin;%PATH%
set LUA_DIR=C:\LuaJIT-2.0.2

:: Set up boost
set BOOST_ROOT=C:\boost_1_55_0
set BOOST_LIBRARYDIR=C:\boost_1_55_0\bin.v2\libs

:: Set up Qt5
set PATH=C:\Qt\Qt5.2.0\5.2.0\mingw48_32\bin;%PATH%
set QTDIR=C:\Qt\Qt5.2.0\5.2.0\mingw48_32

:: Run CMake Mingw generator for Debug
mkdir build-debug
cd build-debug
cmake .. -G "Sublime Text 2 - MinGW Makefiles" -DCMAKE_BUILD_TYPE="Debug"
cd ..

:: Run CMake Mingw generator for Release
mkdir build-release
cd build-release
cmake .. -G "Sublime Text 2 - MinGW Makefiles" -DCMAKE_BUILD_TYPE="Release"
cd ..

:: After cmd is available, type Grapedit.sln to open Visual Studio with set up environment
cmd