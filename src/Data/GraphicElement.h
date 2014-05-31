#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(Item);
DECLARE_CLASS_PTR(Layout);
DECLARE_CLASS_PTR(Drawable);

DECLARE_CLASS_PTR(GraphicElement);
DECLARE_CLASS_PTR(GraphicTextState);

namespace lua {
    class State;
}

typedef std::list<GraphicElement*> GraphicElementsList;

/// Je to minimalisticka trieda, ktora vznikne ako prva pri pridavani prvkov do grafickej sceny. Po vytvoreni sa pointer na this vratime naspat do Lua, aby sme pri update a delete vedeli najst tieto prvky.
class GraphicElement
{
    friend class GraphicText;
    
public:

	enum class Type { Item, Grid };

	/// Vytvori GraphicElement pre Item (je to list v abstraktnom syntaktickom strome), ktory obsahuje iba text a nie dalsie vnorene graficke elementy.
	///
	/// @param textType typ, ktory hovori o type textu, napr. C++, Lua, Java, txt...
	/// @param graphicType typ, ktory hovori o grafickom obsahu elementu
	/// @param text obsahujuci text
	/// @param parentPointer rodic, ak je NULL tak je to root
    /// @param index index v rodicovskej hierarchii
	///
	/// @return vytvoreny GraphicElement
	GraphicElement(
		const std::string& textType, 
		const std::string& graphicType,
        const std::string& text,
        GraphicElement* parentPointer,
		size_t index);

	/// Vytvori GraphicElement pre Grid (je to uzol v abstraktnom syntaktickom strome), ktory NEobsahuje text ALE dalsie vnorene graficke elementy.
	///
	/// @param textType typ, ktory hovori o type textu, napr. C++, Lua, Java, txt...
	/// @param graphicType typ, ktory hovori o grafickom obsahu elementu
	/// @param parentPointer rodic, ak je NULL tak je to root
	/// @param index index v rodicovskej hierarchii
	/// @return vytvoreny GraphicElement
	GraphicElement(
		const std::string& textType, 
		const std::string& graphicType,
		GraphicElement* parentPointer,
        size_t index);

	
	~GraphicElement();

	/// @return index v rodicovskej hierarchii
	size_t getIndex() const { return _index; }

	/// @return rodic, ak je NULL tak je to root
	GraphicElement* getParent() const { return _parentPointer; }

	Type getType() const { return _type; }

	/// @return typ, ktory hovori o grafickom obsahu elementu
	const std::string& getGraphicType() const { return _graphicType; }

	DrawablePtr getDrawable() const { return _graphicalElement; }

	/// Vrati graficky grid, tato trieda musi byt typu Grid.
	///
	/// @attention nemozme volat ak sme este neinicializovali graficky element, teda nezavolali funkciu initGraphicalElement
	LayoutPtr getLayout() const;

	/// Vrati graficky item, tato trieda musi byt typu Item
	///
	/// @attention nemozme volat ak sme este neinicializovali graficky element, teda nezavolali funkciu initGraphicalElement
	ItemPtr getItem() const;

	bool isInitialized() const { return _graphicalElement != nullptr; }

	void setNewText(const std::string& text) { _text = text; }

	/// Tato funkcia vytvori prislusny Qt-ckovsky graficky prvok
	void initialize(const GraphicTextStatePtr& state);
    
    // Tato funkcia updatuje graficke info
    void update();

private:

	typedef std::list<DrawablePtr> ReusableInstances;
	typedef std::map<std::string, ReusableInstances> ReusableInstancesMap;

	/// Mapa mapujuca elementy, ktore sa daju zonapouzit namiesto vytvarania novych
	static ReusableInstancesMap _reusableInstances;

	/// Funckia vracia znovapouzitelne graficke elementy podla ich typu
	static DrawablePtr dequeueReusableElement(const std::string& graphicType);

	size_t _index;
	std::string _text;
	std::string _textType;
	std::string _graphicType;
	Type _type;
	
	GraphicElement* _parentPointer;

	DrawablePtr _graphicalElement;
    
    static DrawablePtr createElement(const lua::State& state, const char* type);
};
