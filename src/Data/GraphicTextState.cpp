#include "./GraphicTextState.h"

#include "../LuaBindings/CoreBindings.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
const char* GraphicTextState::globalLuaName = "cpp_LuaState_this_ptr";

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicTextState::GraphicTextState()
{
	_state = lua_open();
	luaL_openlibs(_state);

	setupCoreBindings(_state);

	// Na to aby sme nasli prislusny LuaState pri statickych volaniach C funkcii, pridame this pointer do Lua stavu
	lua_pushlightuserdata(_state, this);
	lua_setglobal(_state, globalLuaName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
GraphicTextState::~GraphicTextState()
{
	lua_close(_state);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::reparseText(const std::string& text)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::loadGrammarFile(const std::string& filename)
{
	_textType = filename;
	
	boost::format format("scripts\\grammars\\%s.lua");
	format % filename;

	luaL_dofile(_state, format.str().c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::addNewElement(GraphicElement* element)
{
	size_t key = reinterpret_cast<size_t>(element->getParent());

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
	// Zmazeme elementy
	emit removeElementsFromScene(_deleteElements);
	_deleteElements.clear();

	// Updatujeme elementy
	emit updateElementsOnScene(_updateElements);
	_updateElements.clear();

	// Na konci pridame elementy
	GraphicElementsList newElementsList;
	for (size_t key : _newElementsIndexes)
		newElementsList.insert(newElementsList.begin(), _newElementsBuckets[key].begin(), _newElementsBuckets[key].end());

	emit addElementsToScene(newElementsList);
	_newElementsBuckets.clear();
	_newElementsIndexes.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicTextState::reset()
{
	_newElementsBuckets.clear();
	_newElementsIndexes.clear();
	_updateElements.clear();
	_deleteElements.clear();
}