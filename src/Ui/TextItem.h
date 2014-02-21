#pragma once

#include "pch.h"

#include "BaseItem.h"

DECLARE_CLASS_PTR(QFont);
DECLARE_CLASS_PTR(TextItem);

class TextItem : public BaseItem
{
public:
	TextItem();
	virtual ~TextItem();
	
	void setFont(const QFontPtr& font);

protected:

	QSizeF measureSize() const override;
	void draw(QPainter *painter, const QRectF& bounds) override;

private:

	QFontPtr _font;
};
