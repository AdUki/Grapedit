#pragma once

#include "pch.h"

/// Funkcia nabinduje do lua_State z�kladn� funkcie na farebny v�stup do konzoly.
///
/// @param L Otvoren� Lua interpterter s natiahnut�mi z�kladn�mi syst�mov�mi kni�nicami
void setupCoreBindings(lua_State* L);