//
//  LinearLayout.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#include "LinearLayout.h"

#include <QGraphicsLinearLayout>

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearLayout::LinearLayout(Qt::Orientation orientation)
{
    _layout = new QGraphicsLinearLayout(orientation);
    static_cast<QGraphicsLinearLayout*>(_layout)->setSpacing(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearLayout::~LinearLayout()
{
    delete _layout;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearLayout::insertItem(size_t index, const ItemPtr& item)
{
    super::insertItem(index, item);
    static_cast<QGraphicsLinearLayout*>(_layout)->insertItem(index, item.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearLayout::insertLayout(size_t index, const LayoutPtr& layout)
{
    super::insertLayout(index, layout);
    static_cast<QGraphicsLinearLayout*>(_layout)->insertItem(index, layout->getQtLayout());
}

