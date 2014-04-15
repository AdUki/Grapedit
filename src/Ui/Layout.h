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

class Layout :
    public Drawable,
    public std::enable_shared_from_this<Layout>
{
public:
    
    Layout(QGraphicsLayout* layout);
    Layout(const lua::Ref& style, QGraphicsLayout* layout);
    virtual ~Layout();
    
    QRectF boundingRect() const override;
    
    // Functions to be overloaded
    virtual void insertItem(size_t index, const ItemPtr& item);
    virtual void insertLayout(size_t index, const LayoutPtr& layout);
    
    // Other functions
    void insertElement(const GraphicElement& element);
    void removeElement(const GraphicElement& element);
    
    void removeChild(const LayoutPtr& layout);
    void removeChild(const ItemPtr& item);
    void removeChild(size_t index);
    
    void removeAllChildrenFromScene(QGraphicsScene* scene);
    
    const ItemPtr& getItem(size_t index);
    const LayoutPtr& getLayout(size_t index);
    DrawablePtr getDrawable(size_t index);
    
    size_t childrenCount();
    QGraphicsLayout* getQtLayout() { return _layout; }
    
private:
    
    friend class Drawable;
    
    struct Child {
        LayoutPtr layout;
        ItemPtr item;
        
        Child(const LayoutPtr& layout) : layout(layout), item(nullptr) {}
        Child(const ItemPtr& item) : layout(nullptr), item(item) {}
        
        bool operator==(const ItemPtr& value) const {
            return item != nullptr && value != nullptr && item == value;
        }
        
        bool operator==(const LayoutPtr& value) const {
            return layout != nullptr && value != nullptr && layout == value;
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
    };
    
    std::vector<Child> _children;
    
    QGraphicsLayout* _layout;
};
