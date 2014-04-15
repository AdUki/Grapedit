#include "mainwindow.h"

#include <QGraphicsScene>
#include <QPushButton>
#include <QFileDialog>

#include "Data/GraphicText.h"
#include "Data/GraphicElement.h"

#include "./ui_mainwindow.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow() : QMainWindow()
{
	UI = std::make_shared<Ui::MainWindow>();
	UI->setupUi(this);
    UI->plainTextEdit->setFocus();

    connect(UI->plainTextEdit, &QPlainTextEdit::textChanged, [this]() {
		if (_currentGraphicText != nullptr)
			_currentGraphicText->setText(UI->plainTextEdit->toPlainText().toStdString());
	});
    
    connect(UI->reloadCanvasButton, &QPushButton::clicked, [this]() {
        if (_currentGraphicText != nullptr)
            _currentGraphicText->getScene()->update();
    });
    
    connect(UI->reloadGrammarButton, &QPushButton::clicked, [this]() {
        if (_currentGraphicText != nullptr) {
            _currentGraphicText->reloadTextStyle();

            UI->graphicsView->setScene(_currentGraphicText->getScene());
        }
    });
    
    connect(UI->clearButton, &QPushButton::clicked, [this]() {
        UI->plainTextEdit->setPlainText("");
    });
    
    connect(UI->openFileButton, &QPushButton::clicked, [this]() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setWindowTitle(tr("Open file"));
        QString fileName = dialog.getOpenFileName();
        
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly | QFile::Text))
                return;
            
            QTextStream in(&file);
            UI->plainTextEdit->setPlainText(in.readAll());
            file.close();
        }
    });
    
	GraphicTextPtr graphicText = std::make_shared<GraphicText>("lua");
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
    
    QGraphicsScene* scene = graphicText->getScene();
	UI->graphicsView->setScene(scene);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::setupTextEditField()
{

}