#include "./LinearGrid.h"

#include "BaseItem.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearGrid::LinearGrid() : QGraphicsLinearLayout(Qt::Orientation::Vertical)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearGrid::LinearGrid(Qt::Orientation orientation) : QGraphicsLinearLayout(orientation)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearGrid::~LinearGrid()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearGrid::insertElement(const GraphicElement& element, size_t index) 
{
	Q_ASSERT(QGraphicsLinearLayout::count() >= 0 && static_cast<size_t>(QGraphicsLinearLayout::count()) >= index);

	switch (element.getType()) {

	case GraphicElement::Type::Grid: {
		QGraphicsLayout* layout = element.getGrid()->getLayout();
		layout->setParentLayoutItem(this);
		QGraphicsLinearLayout::insertItem(index, layout);
		break;
	}
	case GraphicElement::Type::Item:
		QGraphicsLinearLayout::insertItem(index, element.getItem().get());

		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF LinearGrid::boundingRect() const 
{
	return geometry();
}