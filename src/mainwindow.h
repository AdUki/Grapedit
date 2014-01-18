#pragma once
#include "pch.h"

#include <QMainWindow>

DECLARE_NS_CLASS_PTR(Ui, MainWindow)

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindowPtr UI;

};

