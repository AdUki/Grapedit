#pragma once

#include "pch.h"

#include <QGraphicsItem>

class BaseElement : public QGraphicsItem
{
public:
	enum class VerticalAlignment { Center, Top, Bottom};
	enum class HorizontalAlignment { Center, Left, Right};

	BaseElement();
	~BaseElement();

protected:

	virtual void draw(QPainter *painter, const QRectF& bounds) {}

	// QGraphicsRectItem functions

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;

private:


#ifndef NDEBUG
	QColor _borderColor;
#endif
};
