#include "ElementBindings.h"

#include "../Data/GraphicTextState.h"

/// Pomocn� funkcia na z�skanie GraphicTextState
/// @returns Vr�ti ulo�en� graphicTextState, ktor� k dan�mu lua_Statu patr�
GraphicTextState* getGraphicTextState(lua_State* L)
{
	lua_getglobal(L, GraphicTextState::globalLuaName);
	GraphicTextState* graphicTextState = static_cast<GraphicTextState*>(lua_touserdata(L, -1));
	lua_pop(L, 1);

	if (graphicTextState == nullptr)
		throw LuaElementException(LuaElementException::Type::LuaStateNil);

	return graphicTextState;
}

/// Funkcia na prid�vanie itemov.
///
/// @param graphicType [LUA] Typ grafick�ho objektu
/// @param index [LUA] Index v rodi�ovskej hierarchii
/// @param text [LUA] Obsahuj�ci text
/// @param parent [LUA] Rodi� elementu. M��e by� nullptr, vtedy je to root
/// @returns [LUA] Funkcia vracia pointer (lightuserdata) na GraphicElement
int lua_add_item(lua_State* L)
{
	if (!lua_isstring(L, 1) || !lua_isnumber(L, 2) || !lua_isstring(L, 3))
		throw LuaElementException(LuaElementException::Type::InvalidArguments);

	GraphicTextState* state = getGraphicTextState(L);
	GraphicElement* newElement = new GraphicElement(
		state->getTextType(), 
		lua_tostring(L, 1), 
		lua_tointeger(L, 2), 
		lua_tostring(L, 3), 
		static_cast<GraphicElement*>(lua_touserdata(L, 4)));

	state->addNewElement(newElement);

	lua_pushlightuserdata(L, newElement);
	return 1;
}

/// Funkcia na prid�vanie gridov
///
/// @param graphicType [LUA] Typ grafick�ho objektu
/// @param index [LUA] Index v rodi�ovskej hierarchii
/// @param parent [LUA] Rodi� elementu. M��e by� nullptr, vtedy je to root
/// @returns [LUA] Funkcia vracia pointer (lightuserdata) na GraphicElement
int lua_add_grid(lua_State* L)
{
	if (!lua_isstring(L, 1) || !lua_isnumber(L, 2))
		throw LuaElementException(LuaElementException::Type::InvalidArguments);

	GraphicTextState* state = getGraphicTextState(L);
	GraphicElement* newElement = new GraphicElement(
		state->getTextType(), 
		lua_tostring(L, 1), 
		lua_tointeger(L, 2), 
		static_cast<GraphicElement*>(lua_touserdata(L, 4)));

	state->addNewElement(newElement);

	lua_pushlightuserdata(L, newElement);
	return 1;
}

/// Funckia na aktualizovanie obsahu elementov
///
/// @param graphicElement [LUA] Pointer na grafick� element
/// @param text [LUA] Nov� text
/// @returns 0
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
/// @param graphicElement [LUA] Pointer na grafick� element
/// @returns 0
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