#pragma once

#include "pch.h"

#include <QGraphicsLinearLayout>

#include "BaseGrid.h"

DECLARE_CLASS_PTR(LinearGrid);

class LinearGrid : 
	public BaseGrid,
	public QGraphicsLinearLayout
{
public:
	LinearGrid(Qt::Orientation orientation);
	virtual ~LinearGrid();

	void insertElement(const GraphicElement& element) override;
	void removeElement(const GraphicElement& element) override;

private:

	QRectF boundingRect() const override final;
	
};
