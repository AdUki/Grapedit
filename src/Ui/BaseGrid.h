#pragma once

#include "pch.h"

#include <QGraphicsLayout>

#include "BaseElement.h"

#include "../Data/GraphicElement.h"

DECLARE_CLASS_PTR(BaseGrid);

class BaseGrid : 
	public BaseElement
{
public:
	BaseGrid();
	virtual ~BaseGrid();

	QGraphicsLayout* getLayout();

	virtual void insertElement(const GraphicElement& element) = 0;
	virtual void removeElement(const GraphicElement& element) = 0;

protected:

private:

};
