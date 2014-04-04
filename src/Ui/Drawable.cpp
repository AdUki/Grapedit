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

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::Drawable()
: _highlighted(false)
, _selected(false)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::Drawable(const lua::Ref& style)
: _highlighted(false)
, _selected(false)
{
    setAcceptHoverEvents(true);
    
    lua::Ref backgroundStyle = style["background"];
    if (!backgroundStyle.is<lua::Table>())
        return;
    
    if (backgroundStyle["color"].is<lua::Table>()) {
        _backgroundColor = Qt::white;
        lua::Ref color = backgroundStyle["color"];
        if (color[1].is<lua::Number>())
            _backgroundColor->setRedF(color[1]);
        
        if (color[2].is<lua::Number>())
            _backgroundColor->setGreenF(color[2]);
        
        if (color[3].is<lua::Number>())
            _backgroundColor->setBlueF(color[3]);
        
        if (color[4].is<lua::Number>())
            _backgroundColor->setAlphaF(color[4]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::~Drawable()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
    QRectF bounds(boundingRect());
    if (isHighlighted()) {
        // TODO: stylovanie onHoverEvent
        painter->setPen(Qt::red);
        painter->drawRect(bounds);
    }
    
    if (_backgroundColor.is_initialized())
        painter->fillRect(bounds, QBrush(*_backgroundColor));
    
    draw(painter, bounds);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    _highlighted = true;
    update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    _highlighted = false;
    update();
}
