#include "CoreBindings.h"

#include "../Console.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
static int lua_iowrite_output(lua_State* L) 
{
	Console::setTextColor(Console::TextColor::Blue);
	
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {  /* repeat for each level */

		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING:  // strings
				printf("%s", lua_tostring(L, i));
				break;

			case LUA_TBOOLEAN:  // booleans
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;

			case LUA_TNUMBER: { // numbers
				LUA_NUMBER number = lua_tonumber(L, i);
				LUA_NUMBER integralPart;
				if (std::modf(number, &integralPart) == 0)
					// integer
					printf("%d", static_cast<int>(integralPart));
				else
					// floating
					printf("%lf", lua_tonumber(L, i));
				break;
			}
			default:  // other values
				printf("%s", lua_typename(L, t));
				break;
		}
	}

	Console::resetTextColor();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
static int lua_print_output(lua_State* L) 
{
	Console::setTextColor(Console::TextColor::Blue);

	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {  /* repeat for each level */

		if (i != 1)
			printf("\t"); // put a separator

		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING:  // strings
				printf("%s", lua_tostring(L, i));
				break;

			case LUA_TBOOLEAN:  // booleans
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;

			case LUA_TNUMBER: { // numbers
				LUA_NUMBER number = lua_tonumber(L, i);
				LUA_NUMBER integralPart;
				if (std::modf(number, &integralPart) == 0)
					// integer
					printf("%d", static_cast<int>(integralPart));
				else
					// floating
					printf("%lf", lua_tonumber(L, i));
				break;
			}
			default:  // other values
				printf("%s", lua_typename(L, t));
				break;
		}
	}

	printf("\n");

	Console::resetTextColor();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void setupCoreBindings(lua_State* L)
{
	// set up print function
	lua_pushcfunction(L, lua_print_output);
	lua_setglobal(L, "print");

	// set up io.write function
	lua_getglobal(L, "io");                   // -2 on stack
	lua_pushcfunction(L, lua_iowrite_output); // -1 on stack
	lua_setfield(L, -2, "write");
	lua_pop(L, 1); // Pop io table
}