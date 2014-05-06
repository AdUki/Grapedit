#pragma once

#include "pch.h"

#include "./Item.h"

DECLARE_CLASS_PTR(QFont);
DECLARE_CLASS_PTR(TextItem);

/// Grafický prvok na zobrazenie textu. Môžme mu meniť typ písma, veľkosť písma a farbu písma
class TextItem : public Item
{
public:
    
    /// Konštruktor, očakáva štýl s jedným prvkom "text"
    ///
    /// @param style Inicializovaná referencia na pole "value" uzla AST
	TextItem(const lua::Value& style);
	
    /// Nastaví font
    ///
    /// @param font Font, ktorý sa použije pri nakreslení prvku
	void setFont(const QFontPtr& font);

protected:

	QSizeF measureSize() const override;
	void draw(QPainter *painter, const QRectF& bounds) override;

private:
    
    typedef Item super;

    /// Font, ktorý sa použije pri nakreslení prvku
	QFontPtr _font;
    
    /// Farba textu, ktorá sa použije pri nakreslení prvku
    QColor _textColor;
};
