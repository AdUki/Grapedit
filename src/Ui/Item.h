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

/// Trieda pre prvky, ktoré obsahujú text
class Item : public QGraphicsLayoutItem, public Drawable
{
public:
    
    /// Konštruktor pre prvky, ktoré obsahujú text
    ///
    /// @param style Inicializovaná referencia na pole "value" uzla AST
    Item(const lua::Value& style);
    
    enum class VerticalAlignment { Center, Top, Bottom};
	enum class HorizontalAlignment { Center, Left, Right};
    
    /// Funkcia nastaví text podľa operácií ADD a UPDATE z interprata Lua. Po nastavení nového textu, prvok sa automaticky nanovo usporiada a prekreslí.
    ///
    /// @param text Nový text, ktorý patrí tomuto prvku
    void setText(const std::string& text);
    
    /// Funkcia vráti prislúchajúci text.
    const std::string& getText() const { return _text; }
    
    /// Funkcia zmeria veľkosť prvku, ktorá je potrebná na nakreslenie na scénu
    ///
    /// @return Minimálna veľkosť prvku.
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
    
    /// Text, ktorý patrí tomuto prvku
    std::string _text;
    
    /// Farba obsahu
    boost::optional<QColor> _contentColor;
};
