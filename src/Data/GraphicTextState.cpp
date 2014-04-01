#include "./GraphicTextState.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicTextState::GraphicTextState()
{
    _state["send_addItem"] = [this] (lua::String elementType, lua::String elementText, lua::Pointer parentPointer, lua::Integer elementIndex) -> lua::Pointer
    {
        GraphicElement* newElement = new GraphicElement(getTextType(), elementType, elementText, static_cast<GraphicElement*>(parentPointer), elementIndex);
        
        addNewElement(newElement);
        
        return newElement;
    };

    _state["send_addGrid"] = [this] (lua::String elementType, lua::Pointer parentPointer, lua::Integer elementIndex) -> lua::Pointer
    {
        GraphicElement* newElement = new GraphicElement(getTextType(), elementType, static_cast<GraphicElement*>(parentPointer), elementIndex);
        
        addNewElement(newElement);
        
        return newElement;
    };
    
    _state["send_updateItem"] = [this] (lua::Pointer elementPtr, lua::String newElementText)
    {
        GraphicElement* element = static_cast<GraphicElement*>(elementPtr);
        element->setNewText(newElementText);
        addUpdateElement(element);
    };
    
    _state["send_removeItem"] = [this] (lua::Pointer elementPtr)
    {
        addDeleteElement(static_cast<GraphicElement*>(elementPtr));
    };
    
    _state["send_commit"] = [this] () { commit(); };
    
	// Nacitame core
    _state.doFile("scripts/init.lua");
    _state.doFile("scripts/core.lua");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicTextState::~GraphicTextState()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::reparseText(const std::string& text)
{
	qDebug() << "Request text reparse...";

    _state["parseText"](lua::String(text.c_str()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::loadGrammar(const std::string& name)
{
	_textType = name;
    
    _state["loadGrammarAndStyle"](lua::String(name.c_str()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::addNewElement(GraphicElement* element)
{
    // Pointer rodica pouzivame ako kluc
    GraphicElementKey key = element->getParent();
    
	NewElementsBuckets::iterator bucketsIterator = _newElementsBuckets.find(key);
	if (bucketsIterator == _newElementsBuckets.end()) {

		// Daneho rodica s klucom este nemame, vytvorime novy
		SortedElements sortedElements;
		sortedElements.insert(element);

		_newElementsBuckets[key] = sortedElements;
		_newElementsIndexes.push_back(key);
	}
	else {

		// Mame uz daneho rodica s klucom, pridame k rodicovi
		bucketsIterator->second.insert(element);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::addUpdateElement(GraphicElement* element)
{
	_updateElements.push_back(element);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::addDeleteElement(GraphicElement* element)
{
	_deleteElements.push_back(element);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::commit()
{
	// Vytvorime zoznam prvkov, ktore sa maju pridat...
	GraphicElementsList newElementsList;
	for (GraphicElementKey key : _newElementsIndexes)
        for (GraphicElement* element : _newElementsBuckets[key])
            newElementsList.push_back(element);
    
    if (_updatesCallback != NULL) {
        _updatesCallback(newElementsList, _updateElements, _deleteElements);
    }
    
	_newElementsBuckets.clear();
	_newElementsIndexes.clear();
	_updateElements.clear();
    _deleteElements.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::reset()
{
	_newElementsBuckets.clear();
	_newElementsIndexes.clear();
	_updateElements.clear();
	_deleteElements.clear();
}