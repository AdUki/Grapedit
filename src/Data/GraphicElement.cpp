#include "GraphicElement.h"

#include "../Ui/BaseGrid.h"
#include "../Ui/BaseItem.h"
#include "../Ui/TextItem.h"
#include "../Ui/LinearGrid.h"

GraphicElement::ReusableInstancesMap GraphicElement::_reusableInstances;

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElementPtr GraphicElement::createGridElement(const std::string& textType, const std::string& graphicType, size_t index, GraphicElement* parentPointer)
{
	GraphicElementPtr element = std::make_shared<GraphicElement>();

	element->_graphicType = graphicType;
	element->_index = index;
	element->_parentPointer = parentPointer;
	element->_type = Type::Grid;

	return element;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicElementPtr GraphicElement::createItemElement(const std::string& textType, const std::string& graphicType, size_t index, const std::string& text, GraphicElement* parentPointer)
{
	GraphicElementPtr element = std::make_shared<GraphicElement>();

	element->_graphicType = graphicType;
	element->_index = index;
	element->_parentPointer = parentPointer;
	element->_type = Type::Item;
	element->_text = text;

	return element;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseGridPtr GraphicElement::getGrid() const
{
	Q_ASSERT(_type == Type::Grid);
	Q_ASSERT(_graphicalElement != nullptr);

	return std::static_pointer_cast<BaseGrid>(_graphicalElement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseItemPtr GraphicElement::getItem() const
{
	Q_ASSERT(_type == Type::Item);
	Q_ASSERT(_graphicalElement != nullptr);

	return std::static_pointer_cast<BaseItem>(_graphicalElement);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicElement::initGraphicalElement()
{
	// TODO: pre testovacie ucely sa vytvori iba text item a horizontalny layout

	// Pozrieme ci nemozme reusenut objekt
	_graphicalElement = dequeueReusableElement(_textType + _graphicType);
	if (_graphicalElement == nullptr) {
		switch (_type) 
		{
		case GraphicElement::Type::Item:
			_graphicalElement = std::make_shared<TextItem>();
			break;

		case GraphicElement::Type::Grid:
			_graphicalElement = std::make_shared<LinearGrid>(Qt::Orientation::Horizontal);
			break;

		default:
			Q_ASSERT(false);
			break;
		}
	}

	// Nastavime novy text a nanovo nastylujeme element
	switch (_type)
	{
	case GraphicElement::Type::Item:
		std::static_pointer_cast<BaseItem>(_graphicalElement)->setText(_text);
		_text.clear();
		break;

	case GraphicElement::Type::Grid:
		break;

	default:
		Q_ASSERT(false);
		break;
	}

	if (_parentPointer != nullptr) {
		GraphicElement* element = dynamic_cast<GraphicElement*>(_parentPointer);
		
		// Kriticka cast kodu, rodic musi splnat vsetky tieto podmienky ak je dany
		Q_ASSERT(element != nullptr);
		Q_ASSERT(element->getType() == GraphicElement::Type::Grid);
		Q_ASSERT(element->isInitialized());

		element->getGrid()->insertElement(*this, _index);
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
