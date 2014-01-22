#include "./LinearGrid.h"

#include "BaseItem.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearGrid::LinearGrid(Qt::Orientation orientation) : QGraphicsLinearLayout(orientation)
{
	setContentsMargins(10, 10, 10, 10);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearGrid::~LinearGrid()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearGrid::insertElement(const GraphicElement& element) 
{
	Q_ASSERT(count() >= 0 && static_cast<size_t>(count()) >= element.getIndex());

	switch (element.getType()) {

	case GraphicElement::Type::Grid: {
		QGraphicsLayout* layout = element.getGrid()->getLayout();
		layout->setParentLayoutItem(this);
		QGraphicsLinearLayout::insertItem(element.getIndex(), layout);
		break;
	}
	case GraphicElement::Type::Item:
		QGraphicsLinearLayout::insertItem(element.getIndex(), element.getItem().get());
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearGrid::removeElement(const GraphicElement& element) 
{
	switch (element.getType()) {

	case GraphicElement::Type::Grid: {
		QGraphicsLinearLayout::removeItem(element.getGrid()->getLayout());
		break;
	}
	case GraphicElement::Type::Item:
		QGraphicsLinearLayout::removeItem(element.getItem().get());
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF LinearGrid::boundingRect() const 
{
	return geometry();
}