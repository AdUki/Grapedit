#pragma once
#include "pch.h"

#include <QMainWindow>

class Drawable;

DECLARE_NS_CLASS_PTR(Ui, MainWindow);

DECLARE_CLASS_PTR(GraphicText);
DECLARE_CLASS_PTR(MainWindow);

/// Hlavné okno programu
class MainWindow : public QMainWindow
{
	Q_OBJECT

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
    
    lua::State _grammarsState;

	void setupTextEditField();
    
private slots:
    
    void onElementLeftButtonClicked(Drawable& drawable);
};

