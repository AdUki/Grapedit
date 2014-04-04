//
//  Item.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#include "Item.h"

#include <QPainter>
#include <QBrush>

//////////////////////////////////////////////////////////////////////////////////////////////////
Item::Item(const lua::Ref& style)
: Drawable(style)
, _vAlignment(VerticalAlignment::Center)
, _hAlignment(HorizontalAlignment::Center)
{
    lua::Ref contentStyle = style["content"];
    if (!contentStyle.is<lua::Table>())
        return;
    
    if (contentStyle["color"].is<lua::Table>()) {
        _contentColor = Qt::white;
        lua::Ref color = contentStyle["color"];
        if (color[1].is<lua::Number>())
            _contentColor->setRedF(color[1]);
        
        if (color[2].is<lua::Number>())
            _contentColor->setGreenF(color[2]);
        
        if (color[3].is<lua::Number>())
            _contentColor->setBlueF(color[3]);
        
        if (color[4].is<lua::Number>())
            _contentColor->setAlphaF(color[4]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::setText(const std::string& text)
{
    if (_text == text)
        return;
    
    _text = text;
    
    QGraphicsItem::update();
    QGraphicsLayoutItem::updateGeometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF Item::boundingRect() const
{
	// Velkost nam udava layout, ak chceme zmenit velkost objektu, tak treba to dat do sizeHint
	return QGraphicsLayoutItem::geometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::setGeometry(const QRectF &rect)
{
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

	QPointF position(-left, -top); // rect sa automaticky posunie o contentMargins
	QSizeF size(measureSize());

	QGraphicsLayoutItem* parent = parentLayoutItem();
	if (parent != nullptr) {
        
		switch (_hAlignment) {
                
            case HorizontalAlignment::Center:
                position.setX((rect.width() - size.width()) / 2 + rect.x() + position.x());
                break;
                
            case HorizontalAlignment::Left:
                position.setX(rect.x() + position.x());
                break;
                
            case HorizontalAlignment::Right:
                position.setX(rect.right() - size.width() + rect.x() + position.x());
                break;
		}
        
		switch (_vAlignment) {
                
            case VerticalAlignment::Center:
                position.setY((rect.height() - size.height()) / 2 + rect.y() + position.y());
                break;
                
            case VerticalAlignment::Top:
                position.setY(rect.y() + position.y());
                break;
                
            case VerticalAlignment::Bottom:
                position.setY(rect.bottom() - size.height() + rect.y() + position.y());
                break;
		}
	}
    
	// Pozor! Musime zavolat super funkciu aby sa nastavila geometry
	QGraphicsLayoutItem::setGeometry(QRectF(position, size));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const
{
    *left = 10;
    *top = 10;
    *right = 10;
    *bottom = 10;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::updateGeometry()
{
    QGraphicsLayoutItem::updateGeometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QSizeF Item::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    switch (which) {
        // TODO: fix pre alignment
//        case Qt::MaximumSize:
//            return QSize(MAXFLOAT, MAXFLOAT);
            
        default: {
            qreal left, top, right, bottom;
            getContentsMargins(&left, &top, &right, &bottom);
            
            QSizeF size = measureSize();
            size.setWidth(size.width() + left + right);
            size.setHeight(size.height() + top + bottom);
            
            return size;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::draw(QPainter *painter, const QRectF &bounds)
{
    drawBackground(painter, bounds);
    
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    QRectF contentRect(bounds.x() + left,
                       bounds.y() + top,
                       bounds.width() - left - right,
                       bounds.height() - top - bottom);
    
    if (_contentColor.is_initialized())
        painter->fillRect(contentRect, QBrush(*_contentColor));
    
    drawContent(painter, contentRect);
}
