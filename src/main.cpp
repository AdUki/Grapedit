#include "mainwindow.h"

#include "Console.h"

#include <QGraphicsLayout>

//////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	QApplication a(argc, argv);
    QGraphicsLayout::setInstantInvalidatePropagation(true);

	MainWindow mainWindow;
	mainWindow.show();

	return a.exec();
}
