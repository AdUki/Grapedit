//
//  RectInset.h
//  Grapedit
//
//  Created by Simon Mikuda on 05/04/14.
//
//

#pragma once

#include <cstddef>

//#include <QRect>
//#include <QRectF>

//////////////////////////////////////////////////////////////////////////////////////////////////
struct RectInset
{
    size_t left;
    size_t top;
    size_t right;
    size_t bottom;
    
    RectInset()
    : left(0)
    , top(0)
    , right(0)
    , bottom(0)
    {
    }
    
    RectInset(size_t left, size_t top, size_t right, size_t bottom)
    : left(left)
    , top(top)
    , right(right)
    , bottom(bottom)
    {
    }
    
    void deflateRect(QRectF& rect) const {
        rect.setX(rect.x() + left);
        rect.setY(rect.y() + top);
        rect.setWidth(rect.width() - right);
        rect.setHeight(rect.height() - bottom);
    }
    
    void inflateRect(QRectF& rect) const {
        rect.setX(rect.x() - left);
        rect.setY(rect.y() - top);
        rect.setWidth(rect.width() + right);
        rect.setHeight(rect.height() + bottom);
    }
    
    void deflateSize(QSizeF& size) const {
        size.setWidth(size.width() - left - right);
        size.setHeight(size.height() - top - bottom);
    }

    void inflateSize(QSizeF& size) const {
        size.setWidth(size.width() + left + right);
        size.setHeight(size.height() + top + bottom);
    }
    
    RectInset operator+(const RectInset& inset) const {
        return RectInset(left + inset.left, top + inset.top, right + inset.right, bottom + inset.bottom);
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
struct Rect
{
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    
    Rect()
    : x(0)
    , y(0)
    , width(0)
    , height(0)
    {
    }
    
    Rect(size_t x, size_t y, size_t width, size_t height)
    : x(x)
    , y(y)
    , width(width)
    , height(height)
    {
    }
    
    void inflate(size_t left, size_t top, size_t right, size_t bottom) {
        x -= left;
        y -= top;
        width += left + right;
        height += top + bottom;
    }
    void inflate(const RectInset& inset) { inflate(inset.left, inset.top, inset.right, inset.bottom); }
    
    void deflate(size_t left, size_t top, size_t right, size_t bottom) {
        x += left;
        y += top;
        width -= left + right;
        height -= top + bottom;
    }
    void deflate(const RectInset& inset) { inflate(inset.left, inset.top, inset.right, inset.bottom); }
    
    size_t top() const { return y; }
    size_t bottom() const { return y + height; }
    size_t left() const { return x; }
    size_t right() const { return x + width; }
    
};