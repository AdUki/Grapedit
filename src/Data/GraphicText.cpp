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
    // Vytvorime custom deleter pre odstranenie vsetkych objektov pri mazani sceny, ktore sa automaticky mazu v destruktore QGraphicScene
	_scene = boost::shared_ptr<QGraphicsScene>(new QGraphicsScene(), [] (QGraphicsScene* ptr) {
        for (QGraphicsItem* item : ptr->items()) {
            ptr->removeItem(item);
        }
    });

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

	grid = new GraphicElement("default", "grid", nullptr, 0);
	grid->initialize();
	scene.addItem(grid->getElement().get());
	_root->addItem(grid->getGrid()->getLayout());

	for (int i = 0; i < 10; ++i) {
		item = new GraphicElement("default", "item", boost::lexical_cast<std::string>(i + pow(10, i)), grid, i);
		item->initialize();
        item->update();
        
		scene.addItem(item->getElement().get());
		grid->getGrid()->insertElement(*item);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::addElements(const GraphicElementsList& elements)
{
    qDebug() << "Adding " << boost::lexical_cast<int>(elements.size()) << " elements.";
	for (GraphicElement* element : elements) {
		element->initialize();
        element->update();
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
        element->update();
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
