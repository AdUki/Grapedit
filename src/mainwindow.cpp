#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include "Data/GraphicText.h"
#include "Data/GraphicElement.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow() : QMainWindow()
{
	UI = std::make_shared<Ui::MainWindow>();
	UI->setupUi(this);

	// TODO: testovaci kod
	testCanvas();

	qDebug() << "NEW" << this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
	qDebug() << "DELETE" << this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::setGraphicText(const GraphicTextPtr& graphicText)
{
	_currentGraphicText = graphicText;
	UI->graphicsView->setScene(graphicText->getScene().get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::testCanvas()
{
	GraphicTextPtr graphicText = std::make_shared<GraphicText>("default");
	setGraphicText(graphicText);
	graphicText->testScene();
}