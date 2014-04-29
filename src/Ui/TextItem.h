#pragma once

#include "pch.h"

#include "./Item.h"

DECLARE_CLASS_PTR(QFont);
DECLARE_CLASS_PTR(TextItem);

class TextItem : public Item
{
public:
	TextItem(const lua::CValue& style);
	
	void setFont(const QFontPtr& font);

protected:

	QSizeF measureSize() const override;
	void draw(QPainter *painter, const QRectF& bounds) override;

private:
    
    typedef Item super;

	QFontPtr _font;
    QColor _textColor;
};
