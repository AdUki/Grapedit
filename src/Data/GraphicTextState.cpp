#include "./GraphicTextState.h"

#include "../LuaBindings/CoreBindings.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicTextState::GraphicTextState()
{
	_state = lua_open();
	luaL_openlibs(_state);

	setupCoreBindings(_state);

	// Na to aby sme nasli prislusny LuaState pri statickych volaniach C funkcii, pridame this pointer do Lua stavu
	lua_pushlightuserdata(_state, this);
	lua_setglobal(_state, LUA_STATE_THIS_PTR_NAME);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicTextState::~GraphicTextState()
{
	lua_close(_state);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::reparseText(const std::string& text)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::loadGrammarFile(const std::string& filename)
{
}