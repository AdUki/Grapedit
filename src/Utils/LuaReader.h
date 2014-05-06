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

    /// Funkcia prečíta farbu z tabuľky interprata Lua
    ///
    /// @param colorRef Inicializovaná referencia na pole "color" uzla AST
    /// @param color    Výstupná premenná na farbu
    ///
    /// @return true ak sa podarilo prečítať farbu, false ak nie
    bool readColor(const lua::Value& colorRef, QColor& color);
    
    /// Funkcia prečíta veľkosť okrajov z interpreta Lua
    ///
    /// @param insetRef Inicializovaná referencia na pole "inset" uzla AST
    /// @param inset    Výstupná premenná na veľkosť okrajov
    ///
    /// @return true ak sa podarilo prečítať veľkosť okrajov, false ak nie
    bool readInset(const lua::Value& insetRef, RectInset& inset);
}

