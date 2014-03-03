#include "ElementBindings.h"

#include "../Data/GraphicTextState.h"

/// Pomocná funkcia na získanie GraphicTextState
///
/// @return Vráti uložený graphicTextState, ktorý k danému lua_Statu patrí
GraphicTextState* getGraphicTextState(lua_State* L)
{
	lua_getglobal(L, GraphicTextState::globalLuaName);
	GraphicTextState* graphicTextState = static_cast<GraphicTextState*>(lua_touserdata(L, -1));
	lua_pop(L, 1);

	if (graphicTextState == nullptr)
		throw LuaElementException(LuaElementException::Type::LuaStateNil);

	return graphicTextState;
}

/// Funkcia na pridvanie itemov.
///
/// @param graphicType [LUA] Typ grafického objektu
/// @param text [LUA] Obsahujúci text
/// @param parent [LUA] Rodič elementu. Môže byť nullptr, vtedy je to root
/// @param index [LUA] Index v rodičovskej hierarchii
/// @return [LUA] Funkcia vracia pointer (lightuserdata) na GraphicElement
int lua_add_item(lua_State* L)
{
	if (!lua_isstring(L, 1) || // element type
        !lua_isstring(L, 2) || // element value/text
        !(lua_isuserdata(L, 3) || lua_isnil(L, 3)) || // parent pointer
        !lua_isnumber(L, 4)) // element parent index
		throw LuaElementException(LuaElementException::Type::InvalidArguments);

	GraphicTextState* state = getGraphicTextState(L);
    
	GraphicElement* newElement = new GraphicElement(state->getTextType(), lua_tostring(L, 1), lua_tostring(L, 2), static_cast<GraphicElement*>(lua_touserdata(L, 3)), lua_tointeger(L, 4));

	state->addNewElement(newElement);

	lua_pushlightuserdata(L, newElement);
	return 1;
}

/// Funkcia na pridávanie gridov
///
/// @param graphicType [LUA] Typ grafického objektu
/// @param parent [LUA] Rodič elementu. Môže byť nullptr, vtedy je to root
/// @param index [LUA] Index v rodičovskej hierarchii
/// @return [LUA] Funkcia vracia pointer (lightuserdata) na GraphicElement
int lua_add_grid(lua_State* L)
{
	if (!lua_isstring(L, 1) || // element typ
        !(lua_isuserdata(L, 2) || lua_isnil(L, 2)) || // parent pointer
        !lua_isnumber(L, 3)) // element parent index
		throw LuaElementException(LuaElementException::Type::InvalidArguments);

	GraphicTextState* state = getGraphicTextState(L);
	GraphicElement* newElement = new GraphicElement(state->getTextType(), lua_tostring(L, 1), static_cast<GraphicElement*>(lua_touserdata(L, 2)), lua_tointeger(L, 3));

	state->addNewElement(newElement);

	lua_pushlightuserdata(L, newElement);
	return 1;
}

/// Funckia na aktualizovanie obsahu elementov
///
/// @param graphicElement [LUA] Pointer na grafický element
/// @param text [LUA] Nový text
/// @return 0
int lua_update_item(lua_State* L)
{
	if (!lua_touserdata(L, 1) || !lua_isstring(L, 2))
		throw LuaElementException(LuaElementException::Type::InvalidArguments);

	GraphicElement* element = static_cast<GraphicElement*>(lua_touserdata(L, 1));
	element->setNewText(lua_tostring(L, 2));
	getGraphicTextState(L)->addUpdateElement(element);

	return 0;
}

/// Funckia na mazanie elementov
///
/// @param graphicElement [LUA] Pointer na grafick˝ element
/// @return 0
int lua_remove_element(lua_State* L)
{
	if (!lua_touserdata(L, 1))
		throw LuaElementException(LuaElementException::Type::InvalidArguments);

	getGraphicTextState(L)->addUpdateElement(static_cast<GraphicElement*>(lua_touserdata(L, 1)));

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void setupElementBindings(lua_State* L)
{
	lua_pushcfunction(L, lua_add_item);
	lua_setglobal(L, "send_addItem");

	lua_pushcfunction(L, lua_add_grid);
	lua_setglobal(L, "send_addGrid");

	lua_pushcfunction(L, lua_update_item);
	lua_setglobal(L, "send_updateItem");

	lua_pushcfunction(L, lua_remove_element);
	lua_setglobal(L, "send_removeItem");
}