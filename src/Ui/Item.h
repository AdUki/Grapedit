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

class Item : public QGraphicsLayoutItem, public Drawable
{
public:
    
    Item(const lua::Ref& style);
    
    enum class VerticalAlignment { Center, Top, Bottom};
	enum class HorizontalAlignment { Center, Left, Right};
    
    void setText(const std::string& text);
    const std::string& getText() const { return _text; }
    
    virtual QSizeF measureSize() const = 0;
    
    void draw(QPainter *painter, const QRectF& bounds) override;
    
    QRectF boundingRect() const final;
    void findTextBoundaries(size_t& left, size_t& right) const final;
    
protected:
    
    void setGeometry(const QRectF &rect) final;
    void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const final {};
    void updateGeometry() final;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const final;

    size_t calculateTextLenght() const override;
    
private:
    
	VerticalAlignment _vAlignment;
	HorizontalAlignment _hAlignment;
    
    std::string _text;
    
    boost::optional<QColor> _contentColor;
};
