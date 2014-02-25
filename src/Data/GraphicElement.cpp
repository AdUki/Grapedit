#include "GraphicElement.h"

#include "../Ui/BaseGrid.h"
#include "../Ui/BaseItem.h"
#include "../Ui/TextItem.h"
#include "../Ui/LinearGrid.h"

GraphicElement::ReusableInstancesMap GraphicElement::_reusableInstances;

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElement::GraphicElement(const std::string& textType, const std::string& graphicType, size_t index, GraphicElement* parentPointer) 
	: _index(index)
    , _graphicType (graphicType)
	, _type(Type::Grid)
	, _parentPointer(parentPointer)
{
	qDebug() << "NEW GraphicElement" << (_type == Type::Grid ? "Grid" : "Item") 
		<< this << _graphicType.c_str() << _index << _parentPointer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElement::GraphicElement(const std::string& textType, const std::string& graphicType, size_t index, const std::string& text, GraphicElement* parentPointer)
	: _index(index)
	, _text(text)
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
BaseGridPtr GraphicElement::getGrid() const
{
	Q_ASSERT(_type == Type::Grid);
	Q_ASSERT(_graphicalElement != nullptr);

	return boost::static_pointer_cast<BaseGrid>(_graphicalElement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseItemPtr GraphicElement::getItem() const
{
	Q_ASSERT(_type == Type::Item);
	Q_ASSERT(_graphicalElement != nullptr);

	return boost::static_pointer_cast<BaseItem>(_graphicalElement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicElement::initGraphicalElement()
{
	// TODO: ziskat graficke info zo stavu Lua
	if (!isInitialized()) {
		// Pozrieme ci nemozme reusenut objekt
		_graphicalElement = dequeueReusableElement(_textType + _graphicType);
		if (_graphicalElement == nullptr) {
			// TODO: pre testovacie ucely sa vytvori iba text item a horizontalny layout
			switch (_type) 
			{
			case GraphicElement::Type::Item:
				_graphicalElement = boost::make_shared<TextItem>();
				break;

			case GraphicElement::Type::Grid:
				_graphicalElement = boost::make_shared<LinearGrid>(Qt::Orientation::Horizontal);
				break;

			default:
				Q_ASSERT(false);
				break;
			}
		}
	}

	// Nastavime novy text a nanovo nastylujeme element
	switch (_type)
	{
	case GraphicElement::Type::Item:
		boost::static_pointer_cast<BaseItem>(_graphicalElement)->setText(_text);
		_text.clear();
		break;

	case GraphicElement::Type::Grid:
		break;

	default:
		Q_ASSERT(false);
		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
BaseElementPtr GraphicElement::dequeueReusableElement(const std::string& graphicType)
{
	BaseElementPtr element;
	ReusableInstancesMap::iterator mapIterator = _reusableInstances.find(graphicType);
	if (mapIterator != _reusableInstances.end() && mapIterator->second.size() > 0) {
		element = mapIterator->second.front();
		mapIterator->second.pop_front();
	}
	return element;
}
