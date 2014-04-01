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
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Drawable::~Drawable()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Drawable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
    painter->fillRect(boundingRect(), QBrush(QColor(0, 0, 0, 15)));
    
    draw(painter, boundingRect());
}