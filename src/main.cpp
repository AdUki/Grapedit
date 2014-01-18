#include "mainwindow.h"

#include "LuaBindings\CoreBindings.h"
#include "Console.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	setupCoreBindings(L);
	lua_close(L);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
