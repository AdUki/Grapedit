#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(QGraphicsScene)

DECLARE_CLASS_PTR(GraphicElement)
DECLARE_CLASS_PTR(GraphicTextState)
DECLARE_CLASS_PTR(GraphicText)

class GraphicText : public QObject
{
	Q_OBJECT

public:
	GraphicText(const std::string& textType);
	~GraphicText();
	
	void setTextType(const std::string& textType);

	QGraphicsScenePtr getScene() { return _scene; }

	void testScene();
	
private:

	typedef std::unordered_map<size_t, GraphicElementPtr> GraphicElements;

	GraphicElements _displayedElements;
	
	std::string _textType;
	std::string _text;

	GraphicTextStatePtr _state;

	QGraphicsScenePtr _scene;
};
