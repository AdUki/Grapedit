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

#include "../Utils/LuaReader.h"


//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::Drawable()
: _highlighted(false)
, _selected(false)
, _backgroundRadius(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::Drawable(const lua::Ref& style)
: _highlighted(false)
, _selected(false)
, _backgroundRadius(0)
{
    setAcceptHoverEvents(true);
    
    if (style["background"].is<lua::Table>()) {
        lua::Ref backgroundStyle = style["background"];
        
        // Farba pozadia
        QColor color;
        if (lua::readColor(backgroundStyle["color"], color))
            _backgroundColor = color;
        
        // Content inset
        lua::readInset(backgroundStyle["padding"], _contentInset);
        
        if (backgroundStyle["radius"].is<lua::Number>())
            _backgroundRadius = backgroundStyle["radius"];
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
void Drawable::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    _highlighted = false;
    update();
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
