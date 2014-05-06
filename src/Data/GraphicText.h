#pragma once

#include "pch.h"

#include "GraphicElement.h"

#include "../Ui/LinearLayout.h"

DECLARE_CLASS_PTR(QGraphicsScene);
DECLARE_CLASS_PTR(QGraphicsLinearLayout);

DECLARE_CLASS_PTR(GraphicTextState);
DECLARE_CLASS_PTR(GraphicText);

/// Štruktúrovaný text, ktorý obsahuje svoj vlasntý Lua interpret a scénu, na ktorej manažuje grafické objekty.
class GraphicText : public QObject
{
	Q_OBJECT

public:
    /// Konštruktor nastavý typ textu
	///
	/// @param textType Názov priečinku, z ktorého bude načítaný súbor s gramatikou a štýlom
	GraphicText(const std::string& textType);
	~GraphicText();

	/// Funkcia nastavý typ textu
	///
	/// @param textType Názov priečinku, z ktorého bude načítaný súbor s gramatikou a štýlom
	void setTextType(const std::string& textType);
    
	/// Nanovo sa načíta štýl textu, pričom sa všetky celá scéna zmaže a nanovo vytvorí    
    void reloadTextStyle();

    /// Funkcia vratí práve používanú scénu.
	QGraphicsScene* getScene() { return _scene; }

signals:
    
    /// Signál notifikuje, kedy bol kliknutý grafický objekt na scéne.
    void onElementLeftButtonClicked(Drawable& drawable);
    
public slots:

	/// Funkcia na nastavenie nového obsahu textu s grafickými prvkami. Po nastavení sa zavolá LPeg parser a následne vytvoria grafické elementy
	///
	/// @param text Nový text
	void setText(const std::string& text);
    
private:

	std::string _text;
    
	GraphicTextStatePtr _state;
	QGraphicsScene* _scene;

	LayoutPtr _root;

    /// Funkcia na príjem ADD, UPDATE, DELETE operácií z interpreta jazyka Lua, ku ktorým dochádza počas parsovania
    ///
    /// @param newElements      Nové grafické objekty
    /// @param updateElements   Prvky ktoré si majú aktualizovať text
    /// @param deleteElements   Grafické objekty, ktoré sa zmažú
    void updateElementsOnScene(const GraphicElementsList& newElements,
                               const GraphicElementsList& updateElements,
                               const GraphicElementsList& deleteElements);

	void loadTextStyle(const std::string& textType);
    
private slots:
    
    void onElementClicked(Qt::MouseButton button);

};
