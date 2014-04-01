#include "./mainwindow.h"
#include "./ui_mainwindow.h"

#include "Data/GraphicText.h"
#include "Data/GraphicElement.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow() : QMainWindow()
{
	UI = std::make_shared<Ui::MainWindow>();
	UI->setupUi(this);

    connect(UI->plainTextEdit, &QPlainTextEdit::textChanged, [this]() {
		if (_currentGraphicText != nullptr)
			_currentGraphicText->setText(UI->plainTextEdit->toPlainText().toStdString());
	});
    
    connect(UI->reloadCanvasButton, &QPushButton::clicked, [this]() {
        if (_currentGraphicText != nullptr)
            _currentGraphicText->getScene()->update();
    });
    
	GraphicTextPtr graphicText = std::make_shared<GraphicText>("test");
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
void MainWindow::setupTextEditField()
{

}