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
#include <qnamespace.h>

#include "../Utils/Geometry.h"

class QPainter;

DECLARE_CLASS_PTR(Layout);
DECLARE_CLASS_PTR(Drawable);

class Drawable : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    
public:
    
    Drawable();
    Drawable(const lua::Ref& style);
    virtual ~Drawable();
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    virtual void draw(QPainter *painter, const QRectF& bounds) {}
    virtual QRectF boundingRect() const = 0;
    virtual void findTextBoundaries(size_t& left, size_t& right) const = 0;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    void setParent(const LayoutPtr& parent) { _parent = parent; }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    const RectInset& getContentInset() const { return _contentInset; }
    LayoutPtr getParent() const { return _parent.lock(); }
    bool isHighlighted() const { return _highlighted; }
    bool isSelected() const { return _selected; }
    
signals:
    
    void onElementClicked(Qt::MouseButton button);
    
protected:
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) final;
    
    size_t findLeftOffset() const;
    
    virtual size_t calculateTextLenght() const = 0;
    
private:
    
    RectInset _contentInset;
    
    LayoutWPtr _parent;
    
    bool _highlighted;
    bool _selected;
    
    boost::optional<QColor> _backgroundColor;
    qreal _backgroundRadius;
    
    void drawBackground(QPainter *painter, const QRectF& bounds);
    
    size_t calculateLeftOffset(size_t elementIndex) const;
};
