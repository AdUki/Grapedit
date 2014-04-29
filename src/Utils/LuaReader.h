//
//  LuaReader.h
//  Grapedit
//
//  Created by Simon Mikuda on 05/04/14.
//
//

#pragma once

#include "pch.h"

#include "./Geometry.h"

namespace lua {

    bool readColor(const lua::CValue& colorRef, QColor& color);
    
    bool readInset(const lua::CValue& insetRef, RectInset& inset);
}

