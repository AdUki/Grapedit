//
//  LuaReader.cpp
//  Grapedit
//
//  Created by Simon Mikuda on 05/04/14.
//
//

#include "./LuaReader.h"

using namespace lua;

//////////////////////////////////////////////////////////////////////////////////////////////////
bool lua::readColor(const Ref& colorRef, QColor& color) {
    
    if (colorRef.is<Table>()) {
        
        if (colorRef[1].is<Number>())
        if (colorRef[2].is<Number>())
        if (colorRef[3].is<Number>())
            color.setRgbF(colorRef[1], colorRef[2], colorRef[3]);
    
        if (colorRef[4].is<Number>())
            color.setAlphaF(colorRef[4]);
        else
            color.setAlphaF(1);
        
        return true;
    }
    else if (colorRef.is<String>()) {
        color.setNamedColor(String(colorRef));
        return true;
    }
    else if (colorRef.is<Number>()) {
        Number whiteIntensity = colorRef;
        color.setRgbF(whiteIntensity, whiteIntensity, whiteIntensity);
        return true;
    }
    
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool lua::readInset(const Ref& insetRef, RectInset& inset) {
    
    if (insetRef.is<Table>()) {
        if (insetRef["top"].is<Number>())
            inset.top = Number(insetRef["top"]);
        
        if (insetRef["bottom"].is<Number>())
            inset.bottom = Number(insetRef["bottom"]);
        
        if (insetRef["left"].is<Number>())
            inset.left = Number(insetRef["left"]);
        
        if (insetRef["right"].is<Number>())
            inset.right = Number(insetRef["right"]);
    }
    else if (insetRef.is<Number>()) {
        Number insetSize = insetRef;
        inset.top = insetSize;
        inset.bottom = insetSize;
        inset.left = insetSize;
        inset.right = insetSize;
    }
    
    return false;
}