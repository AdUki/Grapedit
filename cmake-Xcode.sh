# ::
# :: This batch will only set up environment variables for MinGW Codeblocks and run cmake
# ::

# ::Set up Lua dir, preferably LuaJIT
# set PATH=C:\LuaJIT-2.0.2\bin;%PATH%
# set LUA_DIR=C:\LuaJIT-2.0.2

# Set up Qt5
PATH=/Users/Simon/libs/Qt/5.2.1/clang_64/bin:$PATH
QTDIR=/Users/Simon/libs/Qt/5.2.1/clang_64

# Run CMake Xcode generator
mkdir build
cd build
cmake .. -G "Xcode"
cd ..
