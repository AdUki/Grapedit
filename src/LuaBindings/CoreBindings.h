#pragma once

#include "pch.h"

/// Funkcia nabinduje do lua_State základné funkcie na farebny výstup do konzoly.
///
/// @param L Otvorený Lua interpterter s natiahnutými základnými systémovými knižnicami
void setupCoreBindings(lua_State* L);