#pragma once
#include "pch.h"

#include <QMainWindow>

DECLARE_NS_CLASS_PTR(Ui, MainWindow)

DECLARE_CLASS_PTR(GraphicText)
DECLARE_CLASS_PTR(MainWindow)

class MainWindow : public QMainWindow
{
	Q_OBJECT //-V524

public:
	MainWindow();
	~MainWindow();

public slots:

	/// Funkcia zobrazi dany text v hlavnom okne
	///
	/// @param graphicText textovy objekt s grafickymi elementami
	void setGraphicText(const GraphicTextPtr& graphicText);

private:

	Ui::MainWindowPtr UI;

	GraphicTextPtr _currentGraphicText;

	void testCanvas();
};

