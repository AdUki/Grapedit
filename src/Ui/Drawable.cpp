//
//  Drawable.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#include "Drawable.h"

#include <QPainter>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>

#include "./Layout.h"

#include "../Utils/LuaReader.h"


//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::Drawable()
: _highlighted(false)
, _selected(false)
, _backgroundRadius(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::Drawable(const lua::Value& style)
: _highlighted(false)
, _selected(false)
, _backgroundRadius(0)
{
    setAcceptHoverEvents(true);
    
    if (style["background"].is<lua::Table>()) {
        lua::Value backgroundStyle = style["background"];
        
        // Farba pozadia
        QColor color;
        if (lua::readColor(backgroundStyle["color"], color))
            _backgroundColor = color;
        
        // Content inset
        lua::readInset(backgroundStyle["padding"], _contentInset);
        
        if (backgroundStyle["radius"].is<lua::Number>())
            _backgroundRadius = backgroundStyle["radius"];
    }

    if (style["border"].is<lua::Table>()) {
        lua::Value borderStyle = style["border"];
        
        // Farba okrajov
        QColor color;
        if (lua::readColor(borderStyle["color"], color))
            _borderColor = color;
        
        // Border inset
        lua::readInset(borderStyle["size"], _borderInset);
    }
    
    qDebug() << "NEW" << (QGraphicsItem*)this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::~Drawable()
{
    qDebug() << "DELETE" << (QGraphicsItem*)this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
    QRectF bounds(boundingRect());
    
    // Vykreslime okraje
    drawBorder(painter, bounds);
    _borderInset.deflateRect(bounds);
    
    // Vykreslime pozadie
    
    QPainterPath backgroundPath;
    backgroundPath.addRoundRect(bounds, // radius je v percentach: 0 najmensi a 99 najvacsi
                                screen::scale<float>(_backgroundRadius) / bounds.width() * 99.f,
                                screen::scale<float>(_backgroundRadius) / bounds.height() * 99.f);
    
    if (_backgroundColor.is_initialized()) {
        painter->fillPath(backgroundPath, QBrush(*_backgroundColor));
    }
    _contentInset.deflateRect(bounds);
    
    // Vykreslime content
    draw(painter, bounds);
    
    // Vykreslime selekciu
    if (isHighlighted()) {
        QColor color;
        color.setRgbF(1.f, 0.8f, 0.f);
        color.setAlphaF(0.3f);
        painter->fillPath(backgroundPath, QBrush(color));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    _highlighted = true;
    update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    _highlighted = false;
    update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit onElementClicked(event->button());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::drawBorder(QPainter *painter, const QRectF &bounds)
{
    if (_borderColor.is_initialized()) {
        
        painter->setPen(QPen(*_borderColor, _borderInset.left));
        painter->drawLine(bounds.left()   + _borderInset.left / 2,
                          bounds.top()    + _borderInset.top / 2,
                          bounds.left()   + _borderInset.left / 2,
                          bounds.bottom() - _borderInset.bottom / 2);
        
        painter->setPen(QPen(*_borderColor, _borderInset.bottom));
        painter->drawLine(bounds.left()   + _borderInset.left / 2,
                          bounds.bottom() - _borderInset.bottom / 2,
                          bounds.right()  - _borderInset.right / 2,
                          bounds.bottom() - _borderInset.bottom / 2);
        
        painter->setPen(QPen(*_borderColor, _borderInset.right));
        painter->drawLine(bounds.right()  - _borderInset.right / 2,
                          bounds.bottom() - _borderInset.bottom / 2,
                          bounds.right()  - _borderInset.right / 2,
                          bounds.top()    + _borderInset.top / 2);
        
        painter->setPen(QPen(*_borderColor, _borderInset.top));
        painter->drawLine(bounds.right() - _borderInset.right / 2,
                          bounds.top()   + _borderInset.top / 2,
                          bounds.left()  + _borderInset.left / 2,
                          bounds.top()   + _borderInset.top / 2);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::drawBackground(QPainter *painter, const QRectF& bounds)
{
    QPainterPath path;
    path.addRoundRect(bounds, // radius je v percentach: 0 najmensi a 99 najvacsi
                      screen::scale<float>(_backgroundRadius) / bounds.width() * 99.f,
                      screen::scale<float>(_backgroundRadius) / bounds.height() * 99.f);
    
    if (isHighlighted()) {
        painter->fillPath(path, QBrush(QColor(0, 0, 0, 0.5)));
    }
    
    if (_backgroundColor.is_initialized()) {
        painter->fillPath(path, QBrush(*_backgroundColor));
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////
size_t Drawable::findLeftOffset() const
{
    // Rekurzivne spocitame velkost textu vsetkych itemov nalavo v strome
    for (size_t itemIndex = 0; itemIndex < getParent()->_children.size(); ++itemIndex) {
        if (getParent()->_children[itemIndex] == this) {
            return calculateLeftOffset(itemIndex);
        }
    }
    
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
size_t Drawable::calculateLeftOffset(size_t elementIndex) const
{
    size_t leftOffset = 0;
    
    // Rekurzivne spocitame velkost textu vsetkych itemov nalavo v strome
    if (elementIndex == 0) {
        const LayoutPtr& parent = getParent()->getParent();
        
        if (parent != nullptr) {
            leftOffset += getParent()->findLeftOffset();
        }
    }
    else {
        leftOffset += getParent()->_children[elementIndex - 1].getDrawable()->calculateTextLenght();
        leftOffset += calculateLeftOffset(elementIndex - 1);
    }
    
    return leftOffset;
}

