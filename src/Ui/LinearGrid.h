#pragma once

#include "pch.h"

#include <QGraphicsLinearLayout>

#include "BaseGrid.h"

DECLARE_CLASS_PTR(LinearGrid)

class LinearGrid : 
	public BaseGrid,
	public QGraphicsLinearLayout
{
public:
	LinearGrid();
	LinearGrid(Qt::Orientation orientation);
	~LinearGrid();

	void insertElement(const GraphicElement& element, size_t index) override;

private:

	QRectF boundingRect() const override final;
	
};
