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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicText::~GraphicText()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::setTextType(const std::string& textType)
{
	if (_state != nullptr && textType == _state->getTextType())
		return;

    loadTextStyle(textType);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::clear()
{
    _root->removeAllChildrenFromScene(_scene);
    
    std::string textType = _state->getTextType();
    
	// Nastartujeme novy state, ak mame stary automaticky sa znici
	_state = std::make_shared<GraphicTextState>();
    
    _state->setUpdateCallback(std::bind(&GraphicText::updateElementsOnScene, this, _1, _2, _3));
    _state->loadGrammar(textType);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::reloadTextStyle()
{
    assert(_state != nullptr);
    
    // QGraphicsScene* oldScene = _scene;
    // TODO: delete oldScene; - memory leak
    
    _root->removeAllChildrenFromScene(_scene);
    _scene->removeItem(_root.get());
    
    GraphicElement::_reusableInstances.clear();
    
	QGraphicsWidget* container = new QGraphicsWidget();
	container->setLayout(_root->getQtLayout());
	_scene->addItem(container);

    std::string textType = _state->getTextType();
    loadTextStyle(textType);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::loadTextStyle(const std::string& textType)
{
    _root->removeAllChildrenFromScene(_scene);
    
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
    qDebug() << "Deleting " << boost::lexical_cast<int>(deleteElements.size()) << " elements.";
    for (GraphicElement* element : deleteElements) {
        
        LayoutPtr layout = element->getParent()->getLayout();
        assert(layout != nullptr);
        
        layout->removeElement(*element);
        _scene->removeItem(element->getDrawable().get());
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
        
        connect(element->getDrawable().get(), SIGNAL(onElementClicked(Qt::MouseButton)), this, SLOT(onElementClicked(Qt::MouseButton)));
        
		_scene->addItem(element->getDrawable().get());
        
		if (element->getParent() == nullptr) {
            element->getDrawable()->setZValue(1);
            _root->insertElement(*element);
		}
		else {
			assert(element->getParent()->getType() == GraphicElement::Type::Grid);
			assert(element->getParent()->isInitialized());
            
			element->getParent()->getLayout()->insertElement(*element);
            
            // NOTE: ZValue by sa mohla vyratat uz pri parsovani LPegom a poslat do aplikacie
            size_t zValue = 1;
            LayoutPtr layout = element->getDrawable()->getParent();
            while (layout != nullptr) {
                ++zValue;
                layout = layout->getParent();
            }
            
            element->getDrawable()->setZValue(zValue);
		}
	}
    
    _scene->update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicText::onElementClicked(Qt::MouseButton button)
{
    if (button & Qt::MouseButton::LeftButton) {
        Drawable* drawable = static_cast<Drawable*>(QObject::sender());
        emit onElementLeftButtonClicked(*drawable);
    }
}

