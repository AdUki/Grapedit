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
#include <QGraphicsLayout>

#include "./Layout.h"

#include "../Utils/LuaReader.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
Item::Item(const lua::Ref& style)
: Drawable(style)
, _vAlignment(VerticalAlignment::Center)
, _hAlignment(HorizontalAlignment::Center)
{
    if (style["content"].is<lua::Table>()) {
        lua::Ref contentStyle = style["content"];

        QColor color;
        if (lua::readColor(contentStyle["color"], color))
            _contentColor = color;
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
    
    if (getParent() != nullptr)
        getParent()->getQtLayout()->invalidate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF Item::boundingRect() const
{
	// Velkost nam udava layout, ak chceme zmenit velkost objektu, tak treba to dat do sizeHint
	return QGraphicsLayoutItem::geometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::findTextBoundaries(size_t& left, size_t& right) const
{
    left = findLeftOffset();
    right = left + _text.length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::setGeometry(const QRectF &rect)
{
	QPointF position;
	QSizeF size(rect.size());
    
    // Zvacsime size o contentInset
//    getContentInset().inflateSize(size);

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
            QSizeF size = measureSize();
            
            // Zvacsime size o contentInset
            getContentInset().inflateSize(size);
            
            return size;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Item::draw(QPainter *painter, const QRectF &bounds)
{
    if (_contentColor.is_initialized())
        painter->fillRect(bounds, QBrush(*_contentColor));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
size_t Item::calculateTextLenght() const
{
    return _text.length();
}

