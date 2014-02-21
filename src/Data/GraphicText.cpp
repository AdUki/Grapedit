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
	_scene = boost::make_shared<QGraphicsScene>();
	_root = boost::make_shared<QGraphicsLinearLayout>(Qt::Orientation::Vertical);

    // Vytvorime root element
	QGraphicsWidget* container = new QGraphicsWidget();
	container->setLayout(_root.get());
	_scene->addItem(container);

	setTextType(textType);

	qDebug() << "NEW" << this << "textType = " << textType.c_str();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicText::~GraphicText()
{
	qDebug() << "DELETE" << this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::setTextType(const std::string& textType)
{
	std::string filename(textType);
	if (boost::algorithm::ends_with(textType, ".lua")) {
		filename = filename.substr(0, filename.size() - 4);
	}

	if (_state != nullptr && textType == _state->getTextType())
		return;

	// Zmazeme zobrazene elementy
	_displayedElements.clear();

	// Nastartujeme novy state, ak mame stary automaticky sa znici
	_state = boost::make_shared<GraphicTextState>();
	_state->loadGrammar(textType);

	connect(_state.get(), &GraphicTextState::addElementsToScene, this, &GraphicText::addElements);
	connect(_state.get(), &GraphicTextState::removeElementsFromScene, this, &GraphicText::removeElements);
	connect(_state.get(), &GraphicTextState::updateElementsOnScene, this, &GraphicText::updateElements);

	_state->reparseText(_text);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::setText(const std::string& text)
{
	_text = text;
	_state->reparseText(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::testScene()
{
	QGraphicsScene& scene = *_scene;

	GraphicElement* grid;
	GraphicElement* item;

	grid = new GraphicElement("default", "grid", 0, nullptr);
	grid->initGraphicalElement();
	scene.addItem(grid->getElement().get());
	_root->addItem(grid->getGrid()->getLayout());

	for (int i = 0; i < 10; ++i) {
		item = new GraphicElement("default", "item", i, boost::lexical_cast<std::string>(i), grid);
		item->initGraphicalElement();
		scene.addItem(item->getElement().get());
		grid->getGrid()->insertElement(*item);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::addElements(const GraphicElementsList& elements)
{
	for (GraphicElement* element : elements) {
		element->initGraphicalElement();

		_scene->addItem(element->getElement().get());

		if (element->getParent() == nullptr) {
			switch (element->getType())
			{
			case GraphicElement::Type::Item:
				_root->insertItem(element->getIndex(), element->getItem().get());
				break;

			case GraphicElement::Type::Grid:
				_root->insertItem(element->getIndex(), element->getGrid()->getLayout());
				break;

			default:
				Q_ASSERT(false);
				break;
			}
		}
		else {

			// Kriticka cast kodu, rodic musi splnat vsetky tieto podmienky ak je dany
			Q_ASSERT(element->getParent()->getType() == GraphicElement::Type::Grid);
			Q_ASSERT(element->getParent()->isInitialized());

			element->getParent()->getGrid()->insertElement(*element);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::updateElements(const GraphicElementsList& elements)
{
	for (GraphicElement* element : elements) {
		element->initGraphicalElement();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::removeElements(const GraphicElementsList& elements)
{
	for (GraphicElement* element : elements) {

		_scene->removeItem(element->getElement().get());

		if (element->getParent() == nullptr) {
			switch (element->getType())
			{
			case GraphicElement::Type::Item:
				_root->removeItem(element->getItem().get());
				break;

			case GraphicElement::Type::Grid:
				_root->removeItem(element->getGrid()->getLayout());
				break;

			default:
				Q_ASSERT(false);
				break;
			}
		}
		else
			element->getParent()->getGrid()->removeElement(*element);

		delete element;
	}
}
