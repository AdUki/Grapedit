#include "pch.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	UI = std::make_shared<Ui::MainWindow>();
	UI->setupUi(this);
}

MainWindow::~MainWindow()
{
}
