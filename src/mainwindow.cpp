#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include "Data/GraphicText.h"
#include "Data/GraphicElement.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow() : QMainWindow()
{
	UI = boost::make_shared<Ui::MainWindow>();
	UI->setupUi(this);

    connect(UI->plainTextEdit, &QPlainTextEdit::textChanged, [this]() {
		if (_currentGraphicText != nullptr)
			_currentGraphicText->setText(UI->plainTextEdit->toPlainText().toStdString());
	});
    
    connect(UI->reloadCanvasButton, &QPushButton::clicked, [this]() {
        // TODO: testovaci kod
        _currentGraphicText->testSceneUpdate();
        return;
        
        if (_currentGraphicText != nullptr)
            _currentGraphicText->getScene()->update();
    });
    
	// TODO: testovaci kod
	testCanvas();
    return;
    
	GraphicTextPtr graphicText = boost::make_shared<GraphicText>("test");
	setGraphicText(graphicText);

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
	_currentGraphicText = boost::make_shared<GraphicText>("test");
	setGraphicText(_currentGraphicText);
	_currentGraphicText->testScene();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::setupTextEditField()
{

}