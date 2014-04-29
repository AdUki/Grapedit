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
Drawable::Drawable(const lua::CValue& style)
: _highlighted(false)
, _selected(false)
, _backgroundRadius(0)
{
    setAcceptHoverEvents(true);
    
    if (style["background"].is<lua::Table>()) {
        lua::CValue backgroundStyle = style["background"];
        
        // Farba pozadia
        QColor color;
        if (lua::readColor(backgroundStyle["color"], color))
            _backgroundColor = color;
        
        // Content inset
        lua::readInset(backgroundStyle["padding"], _contentInset);
        
        if (backgroundStyle["radius"].is<lua::Number>())
            _backgroundRadius = backgroundStyle["radius"];
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
    
    // Vykreslime pozadie
    drawBackground(painter, bounds);
    
    // Zmensime bounds o contentInset
    _contentInset.deflateRect(bounds);
    
    // Vykreslime content
    draw(painter, bounds);
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
void Drawable::drawBackground(QPainter *painter, const QRectF& bounds)
{
    QPainterPath path;
    path.addRoundRect(bounds, // radius je v percentach: 0 najmensi a 99 najvacsi
                      screen::scale<float>(_backgroundRadius) / bounds.width() * 99.f,
                      screen::scale<float>(_backgroundRadius) / bounds.height() * 99.f);
    
    if (isHighlighted()) {
        // TODO: stylovanie onHoverEvent
        painter->setPen(Qt::red);
        painter->drawPath(path);
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

