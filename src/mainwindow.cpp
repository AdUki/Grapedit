#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include "Data/GraphicText.h"
#include "Data/GraphicElement.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow() : QMainWindow()
{
	UI = boost::make_shared<Ui::MainWindow>();
	UI->setupUi(this);

	// TODO: testovaci kod
//	testCanvas();
//    return;
    
	GraphicTextPtr graphicText = boost::make_shared<GraphicText>("test");
	setGraphicText(graphicText);

	connect(UI->plainTextEdit, &QPlainTextEdit::textChanged, [this](){
		if (_currentGraphicText != nullptr)
			_currentGraphicText->setText(UI->plainTextEdit->toPlainText().toStdString());
	});
    connect(UI->reloadCanvasButton, &QPushButton::released, [this, graphicText](){
        graphicText->getScene()->update();
    });

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
	GraphicTextPtr graphicText = boost::make_shared<GraphicText>("test");
	setGraphicText(graphicText);
	graphicText->testScene();

	//connect(UI->plainTextField, SIGNAL(textChanged()), graphicText.get(), SLOT(setText()));

	connect(UI->plainTextEdit, &QPlainTextEdit::textChanged, [=](){
                graphicText->setText(UI->plainTextEdit->toPlainText().toStdString());
            });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::setupTextEditField()
{

}