//
//  Item.h
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#pragma once

#include "pch.h"

#include <QGraphicsLayoutItem>

#include "./Drawable.h"

DECLARE_CLASS_PTR(Item);

class Item : public Drawable, public QGraphicsLayoutItem
{
public:
    
    Item(const lua::Ref& style);
    
    enum class VerticalAlignment { Center, Top, Bottom};
	enum class HorizontalAlignment { Center, Left, Right};
    
    void setText(const std::string& text);
    const std::string& getText() const { return _text; }
    
    virtual QSizeF measureSize() const = 0;
    void draw(QPainter *painter, const QRectF& bounds) final;
    
    virtual void drawBackground(QPainter *painter, const QRectF& bounds) {}
    virtual void drawContent(QPainter *painter, const QRectF& bounds) {}
    
protected:
    
    QRectF boundingRect() const final;
    
    void setGeometry(const QRectF &rect) final;
    void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const final;
    void updateGeometry() final;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const final;
    
private:
    
	VerticalAlignment _vAlignment;
	HorizontalAlignment _hAlignment;
    
    std::string _text;
    
    boost::optional<QColor> _contentColor;
};
