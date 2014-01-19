#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(BaseItem)
DECLARE_CLASS_PTR(BaseGrid)
DECLARE_CLASS_PTR(BaseElement)
DECLARE_CLASS_PTR(GraphicElement)

/// Je to minimalisticka trieda, ktora vznikne ako prva pri pridavani prvkov do grafickej sceny. Po vytvoreni sa pointer na this vratime naspat do Lua, aby sme pri update a delete vedeli najst tieto prvky.
class GraphicElement
{
public:
	~GraphicElement();

	enum class Type { Item, Grid };

	/// Vytvori GraphicElement pre Item (je to list v abstraktnom syntaktickom strome), ktory obsahuje iba text a nie dalsie vnorene graficke elementy.
	///
	/// @param typ, ktory hovori o type textu, napr. C++, Lua, Java, txt...
	/// @param typ, ktory hovori o grafickom obsahu elementu
	/// @param index v rodicovskej hierarchii
	/// @param obsahujuci text
	/// @param rodic, ak je NULL tak je to root
	///
	/// @returns vytvoreny GraphicElement
	static GraphicElementPtr createItemElement(
		const std::string& textType, 
		const std::string& graphicType, 
		size_t index, 
		const std::string& 
		text, GraphicElement* parentPointer);

	/// Vytvori GraphicElement pre Grid (je to uzol v abstraktnom syntaktickom strome), ktory NEobsahuje text ALE dalsie vnorene graficke elementy.
	///
	/// @param typ, ktory hovori o type textu, napr. C++, Lua, Java, txt...
	/// @param typ, ktory hovori o grafickom obsahu elementu
	/// @param index v rodicovskej hierarchii
	/// @param rodic, ak je NULL tak je to root
	///
	/// @returns vytvoreny GraphicElement
	static GraphicElementPtr createGridElement(
		const std::string& textType, 
		const std::string& graphicType, 
		size_t index, 
		GraphicElement* parentPointer);

	/// @returns index v rodicovskej hierarchii
	size_t getIndex() const { return _index; }

	/// @returns rodic, ak je NULL tak je to root
	GraphicElement* getParent() const { return _parentPointer; }

	Type getType() const { return _type; }

	/// @returns typ, ktory hovori o grafickom obsahu elementu
	const std::string& getGraphicType() const { return _graphicType; }

	bool isInitialized() const { return _graphicalElement != nullptr; }

	/// Tato funkcia vytvori prislusny Qt-ckovsky graficky prvok
	void initGraphicalElement();

	/// Vrati graficky grid, tato trieda musi byt typu Grid.
	///
	/// @attention nemozme volat ak sme este neinicializovali graficky element, teda nezavolali funkciu initGraphicalElement
	BaseGridPtr getGrid() const;

	/// Vrati graficky item, tato trieda musi byt typu Item
	///
	/// @attention nemozme volat ak sme este neinicializovali graficky element, teda nezavolali funkciu initGraphicalElement
	BaseItemPtr getItem() const;

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

