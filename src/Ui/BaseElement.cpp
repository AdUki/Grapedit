#include "BaseElement.h"

#include <QPainter>

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseElement::BaseElement()
{
	qDebug() << "NEW" << this;

#ifndef NDEBUG	
	static int cycle = 0;
	const static QColor colors[] = {
		(QColor(255,200,200)),
		(QColor(200,255,200)),
		(QColor(200,200,255)),
		(QColor(228,228,200)),
		(QColor(228,200,228)),
		(QColor(200,228,228)),
		(QColor(210,210,210)),
	};
	_borderColor = colors[cycle];
	cycle = (cycle + 1) % (sizeof(colors) / sizeof(QColor));
#endif

}

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseElement::~BaseElement()
{
	qDebug() << "DELETE" << this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void BaseElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	
#ifndef NDEBUG	
	QRectF bounds = boundingRect();
	painter->fillRect(bounds, QBrush(_borderColor));
#endif

	draw(painter, boundingRect());
}