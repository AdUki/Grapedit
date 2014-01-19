#pragma once

#include "pch.h"

#include <QGraphicsLayout>

#include "BaseElement.h"

#include "../Data/GraphicElement.h"

DECLARE_CLASS_PTR(BaseGrid)

class BaseGrid : 
	public BaseElement
{
public:
	BaseGrid();
	~BaseGrid();

	QGraphicsLayout* getLayout();

	virtual void insertElement(const GraphicElement& element, size_t index) = 0;

protected:

private:

};
