//
//  ImageItem.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 04/05/14.
//
//

#include "ImageItem.h"

#include <QImage>
#include <QPainter>

#include "../Utils/LuaReader.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
ImageItem::ImageItem(const lua::Value& style, const std::string& basePath)
: Item(style)
, _image(nullptr)
{
    lua::String image = style["image"];
    setImage(basePath + image);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void ImageItem::setImage(const std::string& imagePath)
{
    if (_image != nullptr)
        delete _image;

    _image = new QImage(QString::fromStdString(imagePath));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QSizeF ImageItem::measureSize(const boost::optional<QSize>& maxSize) const
{
    assert(_image != nullptr);
    
    QSizeF size = _image->size();
    return size;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void ImageItem::draw(QPainter *painter, const QRectF& bounds)
{
    super::draw(painter, bounds);
    
    painter->drawImage(QPoint(bounds.x(), bounds.y()), *_image);
}
