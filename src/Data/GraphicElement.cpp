#include "GraphicElement.h"

#include "./GraphicTextState.h"

#include "../Ui/Layout.h"
#include "../Ui/Item.h"
#include "../Ui/TextItem.h"
#include "../Ui/LinearLayout.h"

GraphicElement::ReusableInstancesMap GraphicElement::_reusableInstances;

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElement::GraphicElement(const std::string& textType, const std::string& graphicType, GraphicElement* parentPointer, size_t index) 
: _index(index)
, _textType(textType)
, _graphicType (graphicType)
, _type(Type::Grid)
, _parentPointer(parentPointer)
{
	qDebug() << "NEW GraphicElement" << (_type == Type::Grid ? "Grid" : "Item")
		<< this << _graphicType.c_str() << _index << _parentPointer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElement::GraphicElement(const std::string& textType, const std::string& graphicType, const std::string& text, GraphicElement* parentPointer, size_t index)
: _index(index)
, _text(text)
, _textType(textType)
, _graphicType (graphicType)
, _type(Type::Item)
, _parentPointer(parentPointer)
{
	qDebug() << "NEW GraphicElement" << (_type == Type::Grid ? "Grid" : "Item") 
		<< this << _graphicType.c_str() << _index << _parentPointer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElement::~GraphicElement()
{
	// Reusneme element pred tym ako sa znici objekt
	if (_graphicalElement != nullptr) {
		std::string key = _textType + _graphicType;
		ReusableInstancesMap::iterator it = _reusableInstances.find(key);

		if (it != _reusableInstances.end())
			it->second.push_back(_graphicalElement);
		else {
			ReusableInstances newList;
			newList.push_back(_graphicalElement);
			_reusableInstances[key] = newList;
		}
	}

	qDebug() << "DELETE GraphicElement" << (_type == Type::Grid ? "Grid" : "Item") 
		<< this << _graphicType.c_str() << _index << _parentPointer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LayoutPtr GraphicElement::getLayout() const
{
	Q_ASSERT(_type == Type::Grid);
	Q_ASSERT(_graphicalElement != nullptr);

	return std::static_pointer_cast<Layout>(_graphicalElement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
ItemPtr GraphicElement::getItem() const
{
	Q_ASSERT(_type == Type::Item);
	Q_ASSERT(_graphicalElement != nullptr);

	return std::static_pointer_cast<Item>(_graphicalElement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicElement::initialize(const GraphicTextStatePtr& state)
{
	if (!isInitialized()) {
		// Pozrieme ci nemozme reusenut objekt
		_graphicalElement = dequeueReusableElement(_textType + _graphicType);
		if (_graphicalElement == nullptr)
            _graphicalElement = createElement(state->getLuaState(), _graphicType.c_str());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicElement::update()
{
    if (_type == GraphicElement::Type::Item)
        std::static_pointer_cast<Item>(_graphicalElement)->setText(_text);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
DrawablePtr GraphicElement::dequeueReusableElement(const std::string& graphicType)
{
	DrawablePtr element;
	ReusableInstancesMap::iterator mapIterator = _reusableInstances.find(graphicType);
	if (mapIterator != _reusableInstances.end() && mapIterator->second.size() > 0) {
		element = mapIterator->second.front();
		mapIterator->second.pop_front();
	}
	return element;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DrawablePtr GraphicElement::createElement(lua::State state, const char* type)
{
    lua::Ref item = state["currentActiveStyle"][type];
    lua::String objectType = item["object"];
    
    if (item["grid"] == true) {
        
        if (strcmp(objectType, "vertical") == 0)
            return std::make_shared<LinearLayout>(Qt::Orientation::Vertical, item);
        
        else if (strcmp(objectType, "horizontal") == 0)
            return std::make_shared<LinearLayout>(Qt::Orientation::Horizontal, item);
    }
    else if (item["item"] == true) {
        
        if (strcmp(objectType, "text") == 0)
            return std::make_shared<TextItem>(item);
    }
    
    Q_ASSERT(false); // Unknown type
}

