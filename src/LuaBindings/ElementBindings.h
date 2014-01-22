#pragma once

#include "pch.h"

class LuaElementException: public std::exception
{
public:
	enum class Type { LuaStateNil, InvalidArguments };

	LuaElementException(Type type) : _type(type) {}

	virtual const char* what() const throw()
	{
		switch (_type)
		{
		case LuaElementException::Type::LuaStateNil:
			return "Unable find LuaState in Lua interpreter!";

		case LuaElementException::Type::InvalidArguments:
			return "Invalid arguments to function!";

		default:
			return "Unknown error!";
		}
	}

private:

	Type _type;

};


/// Funkcia nabinduje do lua_State funkcie pre správu grafických elementov. Je to potrebné, aby sa elementy zobrazovali na grafickej scéne v aplikácii
///
/// @param L Otvorený Lua interpterter s natiahnutými základnými systémovými knižnicami
void setupElementBindings(lua_State* L);