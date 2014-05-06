//
//  Layout.h
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#pragma once

#include "pch.h"

#include "./Drawable.h"
#include "./Item.h"

#include "../Data/GraphicElement.h"

#include <QGraphicsLayout>

DECLARE_CLASS_PTR(Layout);

/// Usporiadanie, môže obsahovať ďalšie usporiadania, alebo prvky. Neobsahuje text.
class Layout :
    public Drawable,
    public std::enable_shared_from_this<Layout>
{
public:
    
    /// Konštruktor pre root prvok, ktorý sa vloží ako prvý do scény ako vertikálne usporiadanie.
    ///
    /// @param layout Implementované QtGraphicsLayout, ktoré sa môže použiť v usporiadaní. Trieda preberá jeho vlastníctvo .
    Layout(QGraphicsLayout* layout);
    
    /// Konštruktor pre usporiadania, ktoré obsahujú ďalšie usporiadania, alebo prvky
    ///
    /// @param style  Inicializovaná referencia na pole "value" uzla AST
    /// @param layout Implementované QtGraphicsLayout, ktoré sa môže použiť v usporiadaní. Trieda preberá jeho vlastníctvo .
    Layout(const lua::Value& style, QGraphicsLayout* layout);
    
    /// V deštruktore sa zmaže aj QGraphicsLayout _layout
    virtual ~Layout();
    
    QRectF boundingRect() const override;
    void findTextBoundaries(size_t& left, size_t& right) const final;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Funkcia pridá grafický element do tohto usporiadania
    ///
    /// @param element Inicializovaný grafický element, ktorý vznikol z ADD operácie interpreta Lua
    void insertElement(const GraphicElement& element);
    
    /// Funkcia zmaže grafický element z tohto usporiadania
    ///
    /// @param element Inicializovaný grafický element, ktorý vznikol z REMOVE operácie interpreta Lua
    void removeElement(const GraphicElement& element);
    
    /// Funkcia odstráni dané usporiadanie z tohto usporiadania podľa jeho raw pointra.
    /// Keď sa také nenájde, nesprví sa nič.
    ///
    /// @param layout Usporiadanie, ktoré sa má odstrániť.
    void removeChild(const LayoutPtr& layout);

    /// Funkcia odstráni daný prvok z tohto usporiadania podľa jeho raw pointra.
    /// Keď sa taký nenájde, nesprví sa nič.
    ///
    /// @param item Prvok, ktorý sa má odstrániť
    void removeChild(const ItemPtr& item);
    
    /// Funkcia odstráni dcérsky objekt na danom indexe
    ///
    /// @param index Index, z ktorého sa má odstrániť objekt
    void removeChild(size_t index);
    
    /// Funkcia odstráni všetky deti patriace do tohto usporiadania zo scény
    ///
    /// @param scene Scéna, do ktorej patrí toto usporiadania a všekty jej deti
    void removeAllChildrenFromScene(QGraphicsScene* scene);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Funkcia vráti dcérsky prvok na danom indexe
    const ItemPtr& getItem(size_t index);
    
    /// Funkcia vráti dcérske usporiadanie na danom indexe
    const LayoutPtr& getLayout(size_t index);
    
    /// Funkcia vráti ľubovoľný objekt na danom indexe
    DrawablePtr getDrawable(size_t index);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Funkcia vráti počet detí, patriacich do tohto rodiča
    size_t childrenCount();
    
    /// Funkcia vráti prislúchajúci QGraphicLayout, ktorý sa stará o polohovanie objektov na scéne
    QGraphicsLayout* getQtLayout() { return _layout; }
    
protected:
    
    size_t calculateTextLenght() const final;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Metóda na pridávanie prvok do usporiadania.
    ///
    /// @param index Pozícia, na ktorú sa má pridať nový prvok
    /// @param item  Prvok, ktorý sa ide pridať do usporiadania
    virtual void insertItem(size_t index, const ItemPtr& item);
    
    /// Metóda na pridávanie ďalších usporiadaní do usporiadania.
    ///
    /// @param index Pozícia, na ktorú sa má pridať nové usporiadanie
    /// @param item  Usporiadanie, ktoré sa ide pridať do tohto usporiadania
    virtual void insertLayout(size_t index, const LayoutPtr& layout);
    
private:
    
    friend class Item;
    friend class Drawable;
    
    /// Pomocná trieda pre všetky dcérske objekty
    struct Child {
        LayoutPtr layout;
        ItemPtr item;
        
        Child(const LayoutPtr& layout) : layout(layout), item(nullptr) {}
        Child(const ItemPtr& item) : layout(nullptr), item(item) {}
        
        bool operator==(const Drawable* value) const {
            if (layout != nullptr)
                return &*layout == value;
            else if (item != nullptr)
                return &*item == value;
            else
                return false;
        }
        
        bool operator==(const Child& value) const {
            return (layout != nullptr && value.layout != nullptr && layout == value.layout) || (item != nullptr && value.item != nullptr && item == value.item);
        }
        
        void deleteChild() {
            if (item != nullptr) {
                item->setParentLayoutItem(nullptr);
                item.reset();
            }
            
            else if (layout != nullptr) {
                layout->getQtLayout()->setParentLayoutItem(nullptr);
                layout.reset();
            }
        }
        
        DrawablePtr getDrawable() {
            if (layout != nullptr)
                return std::static_pointer_cast<Drawable>(layout);
            else if (item != nullptr)
                return std::static_pointer_cast<Drawable>(item);
            else
                return DrawablePtr();
        }
    };
    
    std::vector<Child> _children;
    
    /// QGraphicLayout, ktorý sa stará o polohovanie objektov na scéne
    QGraphicsLayout* _layout;
};
