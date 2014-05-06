#pragma once

#include "pch.h"

#include "GraphicElement.h"

DECLARE_CLASS_PTR(GraphicTextState);

/// Každý textový súbor má vlastný interpreter jazyka Lua. Táto trieda obsluhuje tento interpreter a poskytuje základné API.
class GraphicTextState : public QObject
{
	Q_OBJECT

public:
    
    typedef std::function<void(const GraphicElementsList& newElements, const GraphicElementsList& updateElements, const GraphicElementsList& deleteElements)> UpdatesFncCallback;

	GraphicTextState();
	~GraphicTextState();

	/// Funkcia na získanie typu textu.
	///
	/// @return Funkcia vráti typ textu, ktorý udáva, čo je jeho obsahom, napr. XML, C, Java, Lua súbor
	std::string getTextType() const { return _textType; }

	/// Funkcia na reparsovanie noveho textu, volanie sa spusta na novom threade a nasledne su volane callbacky
	///
	/// @param text Text na parsovanie
	void reparseText(const std::string& text);

	/// Funkcia nám načíta súbory s gramatikov a so štýlom. Tieto súbory musia byť uložené na preddefinovanom mieste v priečinku.
	///
	/// @param name Udáva názov priečinku, kde sú uložené súbory s gramatikou a štýlom.
	void loadGrammar(const std::string& name);
    
    /// Funkcia nastaví notifikačnú funkcia pre ADD, UPDATE, DELETE operácie z interpreta jazyka Lua, ku ktorým dochádza počas parsovania
    void setUpdateCallback(const UpdatesFncCallback& callback) {
        _updatesCallback = callback;
    }
    
    /// Funckia vráti stav interpreta jazyka Lua
    lua::State getLuaState() { return _state; }

public slots:

    /// Notifikačná funkcia pre interpret jazyka Lua, ktorá sa zavolá po úspešnom sparsovaní textu.
	void commit();
    
    /// Notifikačná funkcia pre interpret jazyka Lua, ktorá sa zavolá po neúspešnom sparsovaní textu.
	void reset();

private:
	
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
	struct GraphicElementIndexComparator {
		bool operator() (GraphicElement* lhs, GraphicElement* rhs) const{
			return lhs->getIndex() < rhs->getIndex();
		}
	};
    
    typedef GraphicElement* GraphicElementKey;
	typedef std::set<GraphicElement*, GraphicElementIndexComparator> SortedElements;
	typedef std::map<GraphicElementKey, SortedElements> NewElementsBuckets;
	typedef std::list<GraphicElementKey> NewElementsIndexes;

    /// Rodičovia grafických objektov usporiadané podľa príchodu ADD operácie ich budúceho dieťaťa
	NewElementsBuckets _newElementsBuckets;
    
    /// Skupiny grafických objektov usporiadané podľa ich rodičovských indexov, ktoré sa majú pridať zo scény ku danému rodičovi
	NewElementsIndexes _newElementsIndexes;
    
    /// Zoznam grafických objektov, ktoré sa majú aktualizovať na scéne
	GraphicElementsList _updateElements;
    
    /// Zoznam grafických objektov, ktoré sa majú zmazať zo scény
	GraphicElementsList _deleteElements;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    UpdatesFncCallback _updatesCallback;

    lua::State _state;

	std::string _textType;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Funkcia na vytvorenia ADD operácie
    void addNewElement(GraphicElement* element);
    
    /// Funkcia na vytvorenie UPDATE operácie
	void addUpdateElement(GraphicElement* element);
    
    /// Funkcia na vytvorenie DELETE operácie
	void addDeleteElement(GraphicElement* element);
};
