#include "./GraphicText.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLayout>

#include "GraphicElement.h"
#include "GraphicTextState.h"

#include "../Ui/LinearLayout.h"
#include "../Ui/TextItem.h"

using namespace std::placeholders;

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicText::GraphicText(const std::string& textType)
{
    // TODO: zmazat scenu
	_scene = new QGraphicsScene();

    _root = std::make_shared<LinearLayout>(Qt::Orientation::Horizontal);
    
    // Vytvorime root element
	QGraphicsWidget* container = new QGraphicsWidget();
	container->setLayout(_root->getQtLayout());
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
	_state = std::make_shared<GraphicTextState>();
    
    _state->setUpdateCallback(std::bind(&GraphicText::updateElementsOnScene, this, _1, _2, _3));
	_state->loadGrammar(textType);
	_state->reparseText(_text);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::setText(const std::string& text)
{
	_text = text;
	_state->reparseText(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::updateElementsOnScene(const GraphicElementsList& newElements, const GraphicElementsList& updateElements, const GraphicElementsList& deleteElements)
{
    _root->getQtLayout()->invalidate();
    
    qDebug() << "Deleting " << boost::lexical_cast<int>(deleteElements.size()) << " elements.";
    for (GraphicElement* element : deleteElements) {
        
		if (element->getParent() == nullptr)
            _root->removeElement(*element);
		else
			element->getParent()->getLayout()->removeElement(*element);
        
        _scene->removeItem(element->getElement().get());
		delete element;
	}
    
    qDebug() << "Updating " << boost::lexical_cast<int>(updateElements.size()) << " elements.";
    for (GraphicElement* element : updateElements) {
        element->update();
	}
    
    qDebug() << "Adding " << boost::lexical_cast<int>(newElements.size()) << " elements.";
	for (GraphicElement* element : newElements) {
		element->initialize(_state);
        element->update();
		_scene->addItem(element->getElement().get());
        
		if (element->getParent() == nullptr) {
            element->getElement()->setZValue(1);
            _root->insertElement(*element);
		}
		else {
			assert(element->getParent()->getType() == GraphicElement::Type::Grid);
			assert(element->getParent()->isInitialized());
            
			element->getParent()->getLayout()->insertElement(*element);
            
            // NOTE: ZValue by sa mohla vyratat uz pri parsovani LPegom a poslat do aplikacie
            size_t zValue = 1;
            LayoutPtr layout = element->getElement()->getParent();
            while (layout != nullptr) {
                ++zValue;
                layout = layout->getParent();
            }
            
            element->getElement()->setZValue(zValue);
		}
	}
    
    _scene->update();
}

