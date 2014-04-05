//
//  Layout.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#include "Layout.h"

#include <QGraphicsLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsLinearLayout>

//////////////////////////////////////////////////////////////////////////////////////////////////
Layout::Layout(QGraphicsLayout* layout)
: _layout(layout)
{
    _layout->setContentsMargins(getContentInset().left, getContentInset().top, getContentInset().right, getContentInset().bottom);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Layout::Layout(const lua::Ref& style, QGraphicsLayout* layout)
: Drawable(style)
, _layout(layout)
{
    _layout->setContentsMargins(getContentInset().left, getContentInset().top, getContentInset().right, getContentInset().bottom);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Layout::~Layout()
{
    delete _layout;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF Layout::boundingRect() const
{
    return _layout->geometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::insertElement(const GraphicElement& element)
{
    switch (element.getType()) {
        case GraphicElement::Type::Grid:
            insertLayout(element.getIndex(), element.getLayout());
            break;
            
        case GraphicElement::Type::Item:
            insertItem(element.getIndex(), element.getItem());
            break;
            
        default:
            assert(false);
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::removeElement(const GraphicElement& element)
{
    switch (element.getType()) {
        case GraphicElement::Type::Grid:
            removeChild(element.getLayout());
            break;
            
        case GraphicElement::Type::Item:
            removeChild(element.getItem());
            break;
            
        default:
            assert(false);
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::insertItem(size_t index, const ItemPtr& item)
{
    assert(index <= _children.size());
    _children.insert(_children.begin() + index, item);
    
    item->setParent(shared_from_this());
    item->setParentLayoutItem(_layout);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const ItemPtr& Layout::getItem(size_t index)
{
    assert(index < _children.size() && _children[index].item != nullptr);
    return _children[index].item;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::insertLayout(size_t index, const LayoutPtr& layout)
{
    assert(index <= _children.size());
    _children.insert(_children.begin() + index, layout);
    
    layout->setParent(shared_from_this());
    layout->_layout->setParentLayoutItem(_layout);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const LayoutPtr& Layout::getLayout(size_t index)
{
    assert(index < _children.size() && _children[index].layout != nullptr);
    return _children[index].layout;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
DrawablePtr Layout::getDrawable(size_t index)
{
    assert(index < _children.size());
    
    if (_children[index].layout == nullptr)
        return _children[index].layout;
    
    if (_children[index].item == nullptr)
        return _children[index].item;
    
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::removeChild(size_t index)
{
    assert(index < _children.size() && index < _layout->count());
    
    _layout->removeAt(index);
    _children[index].deleteChild();
    _children.erase(_children.begin() + index);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::removeChild(const LayoutPtr& layout)
{
    for (int i = 0; i < _children.size(); ++i) {
        if (_children[i] == layout) {
            layout->_layout->setParentLayoutItem(nullptr);
            removeChild(i);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::removeChild(const ItemPtr& item)
{
    for (int i = 0; i < _children.size(); ++i) {
        if (_children[i] == item) {
            item->setParentLayoutItem(nullptr);
            removeChild(i);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
size_t Layout::childrenCount()
{
    return _children.size();
}

