#include "LuaState.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
LuaState::LuaState()
{
    _luaState = luaL_newstate();
	luaL_openlibs(_luaState);
    
	// Na to aby sme nasli prislusny LuaState pri statickych volaniach C funkcii, pridame this pointer do Lua stavu
	lua_pushlightuserdata(_luaState, this);
//	lua_setglobal(_luaState, globalLuaName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LuaState::LuaState(std::function<void(lua_State* L)> initialization)
: LuaState()
{
    initialization(_luaState);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LuaState::~LuaState()
{
    lua_close(_luaState);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LuaState::tryExec(ExecMode execMode, const LuaArgsPtr& arguments, const OnWorkerStateChangedFnc& callback)
{
    
}

