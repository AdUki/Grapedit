#include "./GraphicText.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

#include "GraphicElement.h"
#include "GraphicTextState.h"

#include "../Ui/LinearGrid.h"
#include "../Ui/TextItem.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicText::GraphicText(const std::string& textType)
{
	_scene = std::make_shared<QGraphicsScene>();

	setTextType(textType);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicText::~GraphicText()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::setTextType(const std::string& textType)
{
	if (textType == _textType)
		return;

	_textType = textType;

	// Zmazeme zobrazene elementy
	_displayedElements.clear();

	// Nastartujeme novy state, ak mame stary automaticky sa znici
	_state = std::make_shared<GraphicTextState>();
	_state->loadGrammarFile(_textType);
	_state->reparseText(_text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::testScene()
{
	QGraphicsScene& scene = *_scene;

	static GraphicElementPtr root = GraphicElement::createGridElement("default", "grid", 0, nullptr);
	root->initGraphicalElement();
	scene.addItem(root->getGrid().get());

	QGraphicsWidget* container = new QGraphicsWidget();
	container->setLayout(root->getGrid()->getLayout());
	scene.addItem(container);

	static GraphicElementPtr item1 = GraphicElement::createItemElement("default", "item", 0, "Hello world! ", root.get());
	item1->initGraphicalElement();
	scene.addItem(item1->getItem().get());

	static GraphicElementPtr item2 = GraphicElement::createItemElement("default", "item", 1, "Hello world! ", root.get());
	item2->initGraphicalElement();
	scene.addItem(item2->getItem().get());

	static GraphicElementPtr item3 = GraphicElement::createItemElement("default", "item", 2, "Hello world! ", root.get());
	item3->initGraphicalElement();
	scene.addItem(item3->getItem().get());
}