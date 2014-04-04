//
//  Drawable.h
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#pragma once

#include "pch.h"

#include <QGraphicsItem>
#include <QRectF>

class QPainter;

DECLARE_CLASS_PTR(Layout);
DECLARE_CLASS_PTR(Drawable);

class Drawable : public QGraphicsItem
{
public:
    Drawable();
    Drawable(const lua::Ref& style);
    virtual ~Drawable();
    
    virtual void draw(QPainter *painter, const QRectF& bounds) {}
    
    virtual QRectF boundingRect() const = 0;
    
    LayoutPtr getParent() const {
        return _parent.lock();
    }
    void setParent(const LayoutPtr& parent) {
        _parent = parent;
    }
    
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
    
    bool isHighlighted() const { return _highlighted; }
    bool isSelected() const { return _selected; }
    
protected:
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) final;
    
private:
    
    LayoutWPtr _parent;
    
    boost::optional<QColor> _backgroundColor;
    
    bool _highlighted;
    bool _selected;
};
