#include "mainwindow.h"

#include "Console.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	QApplication a(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	return a.exec();
}
