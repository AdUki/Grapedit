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


/// Funkcia nabinduje do lua_State funkcie pre spr�vu grafick�ch elementov. Je to potrebn�, aby sa elementy zobrazovali na grafickej sc�ne v aplik�cii
///
/// @param L Otvoren� Lua interpterter s natiahnut�mi z�kladn�mi syst�mov�mi kni�nicami
void setupElementBindings(lua_State* L);