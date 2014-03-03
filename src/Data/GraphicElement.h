#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(BaseItem);
DECLARE_CLASS_PTR(BaseGrid);
DECLARE_CLASS_PTR(BaseElement);

DECLARE_CLASS_PTR(GraphicElement);
typedef std::list<GraphicElement*> GraphicElementsList;

/// Je to minimalisticka trieda, ktora vznikne ako prva pri pridavani prvkov do grafickej sceny. Po vytvoreni sa pointer na this vratime naspat do Lua, aby sme pri update a delete vedeli najst tieto prvky.
class GraphicElement
{
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

	BaseElementPtr getElement() const { return _graphicalElement; }

	/// Vrati graficky grid, tato trieda musi byt typu Grid.
	///
	/// @attention nemozme volat ak sme este neinicializovali graficky element, teda nezavolali funkciu initGraphicalElement
	BaseGridPtr getGrid() const;

	/// Vrati graficky item, tato trieda musi byt typu Item
	///
	/// @attention nemozme volat ak sme este neinicializovali graficky element, teda nezavolali funkciu initGraphicalElement
	BaseItemPtr getItem() const;

	bool isInitialized() const { return _graphicalElement != nullptr; }

	void setNewText(const std::string& text) { _text = text; }

	/// Tato funkcia vytvori prislusny Qt-ckovsky graficky prvok
	void initialize();
    
    // Tato funkcia updatuje graficke info
    void update();

private:

	typedef std::list<BaseElementPtr> ReusableInstances;
	typedef std::map<std::string, ReusableInstances> ReusableInstancesMap;

	/// Mapa mapujuca elementy, ktore sa daju zonapouzit namiesto vytvarania novych
	static ReusableInstancesMap _reusableInstances;

	/// Funckia vracia znovapouzitelne graficke elementy podla ich typu
	static BaseElementPtr dequeueReusableElement(const std::string& graphicType);

	size_t _index;
	std::string _text;
	std::string _textType;
	std::string _graphicType;
	Type _type;
	
	GraphicElement* _parentPointer;

	BaseElementPtr _graphicalElement;
};
