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
#include <QGraphicsScene>

//////////////////////////////////////////////////////////////////////////////////////////////////
Layout::Layout(QGraphicsLayout* layout)
: _layout(layout)
{
    assert(layout != nullptr);
    
    RectInset inset = getContentInset();
    _layout->setContentsMargins(inset.left, inset.top, inset.right, inset.bottom);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Layout::Layout(const lua::Value& style, QGraphicsLayout* layout)
: Drawable(style)
, _layout(layout)
{
    assert(layout != nullptr);
    
    RectInset inset = getContentInset();
    _layout->setContentsMargins(inset.left, inset.top, inset.right, inset.bottom);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Layout::~Layout()
{
    removeAllChildrenFromScene(this->scene());
    delete _layout;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF Layout::boundingRect() const
{
    return _layout->geometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::findTextBoundaries(size_t& left, size_t& right) const
{
    left = findLeftOffset();
    right = left + calculateTextLenght();
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
    
    _layout->updateGeometry();
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
    
    _layout->updateGeometry();
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
    
    _layout->invalidate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::removeChild(const LayoutPtr& layout)
{
    for (int i = 0; i < _children.size(); ++i) {
        if (_children[i] == layout) {
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
            removeChild(i);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Layout::removeAllChildrenFromScene(QGraphicsScene* scene)
{
    if (scene == nullptr)
        return;
    
    for (int i = 0; i < _children.size(); ++i) {
        
        if (_children[i].layout != nullptr) {
            _children[i].layout->removeAllChildrenFromScene(scene);
            scene->removeItem(_children[i].layout.get());
        }
        else {
            scene->removeItem(_children[i].item.get());
        }
        
        _layout->removeAt(i);
        _children[i].deleteChild();
    }
    _children.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
size_t Layout::childrenCount()
{
    return _children.size();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
size_t Layout::calculateTextLenght() const
{
    size_t textLenght = 0;
    for (size_t i = 0; i < _children.size(); ++i) {
        if (_children[i].layout != nullptr) {
            textLenght += _children[i].layout->calculateTextLenght();
        }
        else if (_children[i].item != nullptr) {
            textLenght += _children[i].item->getText().length();
        }
    }
    
    return textLenght;
}

