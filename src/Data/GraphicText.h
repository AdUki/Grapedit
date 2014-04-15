#pragma once

#include "pch.h"

#include "GraphicElement.h"

#include "../Ui/LinearLayout.h"

DECLARE_CLASS_PTR(QGraphicsScene);
DECLARE_CLASS_PTR(QGraphicsLinearLayout);

DECLARE_CLASS_PTR(GraphicTextState);
DECLARE_CLASS_PTR(GraphicText);

class GraphicText : public QObject
{
	Q_OBJECT

public:
	GraphicText(const std::string& textType);
	~GraphicText();

	/// Funkcia nastavý typ textu
	///
	/// textType Názov priečinku, z ktorého bude načítaný súbor s gramatikou a štýlom 
	void setTextType(const std::string& textType);
    
    void reloadTextStyle();

	QGraphicsScene* getScene() { return _scene; }

signals:
    
    void onElementLeftButtonClicked(Drawable& drawable);
    
public slots:

	/// Funkcia na nastavenie nového obsahu textu s grafickými prvkami. Po nastavení sa zavolá LPeg parser a následne vytvoria grafické elementy
	///
	/// @param text Nový text
	void setText(const std::string& text);
    
private:

	std::string _text;
    
	GraphicTextStatePtr _state;
	QGraphicsScene* _scene;

	LayoutPtr _root;

    void updateElementsOnScene(const GraphicElementsList& newElements,
                        const GraphicElementsList& updateElements,
                        const GraphicElementsList& deleteElements);

	void loadTextStyle(const std::string& textType);
    
private slots:
    
    void onElementClicked(Qt::MouseButton button);

};
