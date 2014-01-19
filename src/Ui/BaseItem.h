#pragma once

#include "pch.h"

#include <QGraphicsLayoutItem>

#include "BaseElement.h"

DECLARE_CLASS_PTR(BaseItem)

class BaseItem : 
	public BaseElement,
	public QGraphicsLayoutItem
{
public:
	BaseItem();
	~BaseItem();

	const std::string& getText() const { return _text; }
	void setText(const std::string& text);

protected:

	virtual QSizeF measureSize() const = 0;

	// QGraphicsRectItem functions

	QRectF boundingRect() const override final;

	// QGraphicsLayoutItem functions

	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const override final;
	void setGeometry(const QRectF &rect) override final;

private:
	
	std::string _text;
	
	VerticalAlignment _vAlignment;
	HorizontalAlignment _hAlignment;
};
