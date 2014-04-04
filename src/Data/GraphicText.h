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

	QGraphicsScene* getScene() { return _scene; }

public slots:

	/// Funkcia na nastavenie nového obsahu textu s grafickými prvkami. Po nastavení sa zavolá LPeg parser a následne vytvoria grafické elementy
	///
	/// @param text Nový text
	void setText(const std::string& text);

private:

	typedef std::unordered_map<size_t, GraphicElementPtr> GraphicElements;

	GraphicElements _displayedElements;

	std::string _text;

	GraphicTextStatePtr _state;
	QGraphicsScene* _scene;

	LayoutPtr _root;

    void updateElementsOnScene(const GraphicElementsList& newElements,
                        const GraphicElementsList& updateElements,
                        const GraphicElementsList& deleteElements);

};
