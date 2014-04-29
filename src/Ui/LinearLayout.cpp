//
//  LinearLayout.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#include "LinearLayout.h"

#include <QGraphicsLinearLayout>

#include "../Utils/LuaReader.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearLayout::LinearLayout(Qt::Orientation orientation)
: Layout(new QGraphicsLinearLayout(orientation))
{
    static_cast<QGraphicsLinearLayout*>(getQtLayout())->setSpacing(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
LinearLayout::LinearLayout(const lua::CValue& style, Qt::Orientation orientation)
: Layout(style, new QGraphicsLinearLayout(orientation))
{
    static_cast<QGraphicsLinearLayout*>(getQtLayout())->setSpacing(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearLayout::insertItem(size_t index, const ItemPtr& item)
{
    super::insertItem(index, item);
    static_cast<QGraphicsLinearLayout*>(getQtLayout())->insertItem(index, item.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void LinearLayout::insertLayout(size_t index, const LayoutPtr& layout)
{
    super::insertLayout(index, layout);
    static_cast<QGraphicsLinearLayout*>(getQtLayout())->insertItem(index, layout->getQtLayout());
}

