#pragma once

#include "pch.h"

#include <QMutex>

#include "GraphicElement.h"

DECLARE_CLASS_PTR(GraphicTextState);

/// Každý textový súbor má vlastný interpreter jazyka Lua. Táto trieda obsluhuje tento interpreter a poskytuje základné API.
class GraphicTextState : public QObject
{
	Q_OBJECT //-V524

public:

	/// Nazov identifikatora ktory drzi this pointer na LuaState, cez tento pointer vieme najst v statickych bindovanych funkciach triedu, ktora k tomu volaniu patri
	static const char* globalLuaName;

	GraphicTextState();
	~GraphicTextState();

	/// Funkcia na získanie typu textu.
	///
	/// @returns Funkcia vráti typ textu, ktorý udáva, čo je jeho obsahom, napr. XML, C, Java, Lua súbor
	std::string getTextType() const { return _textType; }

	/// Funkcia na reparsovanie noveho textu, volanie sa spusta na novom threade a nasledne su volane callbacky
	///
	/// @param text Text na parsovanie
	void reparseText(const std::string& text);

	/// Funkcia nám načíta súbory s gramatikov a so štýlom. Tieto súbory musia byť uložené na preddefinovanom mieste v priečinku.
	///
	/// @param name Udáva názov priečinku, kde sú uložené súbory s gramatikou a štýlom.
	void loadGrammar(const std::string& name);

	void addNewElement(GraphicElement* element);
	void addUpdateElement(GraphicElement* element);
	void addDeleteElement(GraphicElement* element);

public slots:

	void commit();
	void reset();

signals:

	void addElementsToScene(const GraphicElementsList& elements);
	void removeElementsFromScene(const GraphicElementsList& elements);
	void updateElementsOnScene(const GraphicElementsList& elements);

private:
	
	struct GraphicElementComparator {
		bool operator() (GraphicElement* lhs, GraphicElement* rhs) const{
			return lhs->getIndex() < rhs->getIndex();
		}
	};

	typedef std::set<GraphicElement*, GraphicElementComparator> SortedElements;
	typedef std::unordered_map<size_t, SortedElements> NewElementsBuckets;
	typedef std::list<size_t> NewElementsIndexes;

	NewElementsBuckets _newElementsBuckets;
	NewElementsIndexes _newElementsIndexes;
	GraphicElementsList _updateElements;
	GraphicElementsList _deleteElements;

	lua_State* _state;
	QMutex _mutex;

	std::string _textType;
	
};
