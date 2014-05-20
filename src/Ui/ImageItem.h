//
//  ImageItem.h
//  Grapedit
//
//  Created by Simon Mikuda on 04/05/14.
//
//

#pragma once

#include "pch.h"

#include "./Item.h"

DECLARE_CLASS_PTR(QFont);
DECLARE_CLASS_PTR(TextItem);

class QImage;

/// Grafický prvok na zobrazenie obrázku
class ImageItem : public Item
{
public:
    /// Konštruktor, očakáva štýl s jedným prvkom "image"
    ///
    /// @param style    Inicializovaná referencia na pole "value" uzla AST
    /// @param basePath Relatívna/Absolútna k priečinku s gramatikou a teda aj obrázku
	ImageItem(const lua::Value& style, const std::string& basePath);
	
    /// Nastaví obrázok
    ///
    /// @param imagePath Relatívna/Absolútna cesta k obrázku
	void setImage(const std::string& imagePath);
    
protected:
    
	QSizeF measureSize(const boost::optional<QSize>& maxSize) const override;
	void draw(QPainter *painter, const QRectF& bounds) override;
    
private:
    
    typedef Item super;
    
    /// Obrázok, ktorý sa nakreslí
    QImage* _image;
};
