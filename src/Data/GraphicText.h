#pragma once

#include "pch.h"

#include "GraphicElement.h"

DECLARE_CLASS_PTR(QGraphicsScene)
DECLARE_CLASS_PTR(QGraphicsLinearLayout)

DECLARE_CLASS_PTR(GraphicTextState)
DECLARE_CLASS_PTR(GraphicText)

class GraphicText : public QObject
{
	Q_OBJECT //-V524

public:
	GraphicText(const std::string& textType);
	~GraphicText();
	
	void setTextType(const std::string& textType);

	QGraphicsScenePtr getScene() { return _scene; }

	void testScene();

public slots:

	/// Funkcia na nastavenie nov�ho obsahu textu s grafick�mi prvkami. Po nastaven� sa zavol� LPeg parser a n�sledne vytvoria grafick� elementy
	///
	/// @param text Nov� text
	void setText(const std::string& text);
	
private:

	typedef std::unordered_map<size_t, GraphicElementPtr> GraphicElements;

	GraphicElements _displayedElements;
	
	std::string _text;

	GraphicTextStatePtr _state;
	QGraphicsScenePtr _scene;

	QGraphicsLinearLayoutPtr _root;

private slots:

	void addElements(const GraphicElementsList& elements);
	void updateElements(const GraphicElementsList& elements);
	void removeElements(const GraphicElementsList& elements);
};
