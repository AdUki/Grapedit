//
//  LinearLayout.h
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#pragma once

#include "pch.h"

#include "./Layout.h"

DECLARE_CLASS_PTR(LinearLayout);

class LinearLayout : public Layout
{
public:
    LinearLayout(Qt::Orientation orientation);
    LinearLayout(Qt::Orientation orientation, const lua::Ref& style);
    ~LinearLayout();
    
    void insertItem(size_t index, const ItemPtr& item) override;
    void insertLayout(size_t index, const LayoutPtr& layout) override;
    
private:
    typedef Layout super;
};
