#pragma once

#include "pch.h"

#include "GraphicElement.h"

DECLARE_CLASS_PTR(GraphicTextState)

/// Ka�d� textov� s�bor m� vlastn� interpreter jazyka Lua. T�to trieda obsluhuje tento interpreter a poskytuje z�kladn� API.
class GraphicTextState : public QObject
{
	Q_OBJECT //-V524

public:

	/// Nazov identifikatora ktory drzi this pointer na LuaState, cez tento pointer vieme najst v statickych bindovanych funkciach triedu, ktora k tomu volaniu patri
	static const char* globalLuaName;

	GraphicTextState();
	~GraphicTextState();

	/// Funkcia na z�skanie typu textu.
	///
	/// @returns Funkcia vr�ti typ textu, ktor� ud�va, �o je jeho obsahom, napr. XML, C, Java, Lua s�bor
	std::string getTextType() const { return _textType; }

	/// Funkcia na reparsovanie noveho textu, volanie sa spusta na novom threade a nasledne su volane callbacky
	///
	/// @param text Text na parsovanie
	void reparseText(const std::string& text);

	/// Funkcia n�m na��ta s�bor s gramatikov. Tento s�bor mus� by� ulo�en� na preddefinovanom mieste.
	///
	/// @param filename Ud�va n�zov s�boru, ktor� sa ma na��ta�. Kon�� pr�ponov .lua, ale t�to pr�ponu nezad�vame
	void loadGrammarFile(const std::string& filename);

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

	std::string _textType;
};
