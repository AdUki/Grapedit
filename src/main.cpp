#include "mainwindow.h"

int main(int argc, char *argv[])
{
	lua_State *L = lua_open();
	lua_close(L);
	
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	
	return a.exec();
}
