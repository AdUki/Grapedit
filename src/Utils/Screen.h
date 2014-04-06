//
//  Screen.h
//  Grapedit
//
//  Created by Simon Mikuda on 06/04/14.
//
//

#pragma once

#include <QWindow>
#include <QScreen>

namespace screen {
    
    static inline float getDevicePixelRatio() {
        static float screenPixelRatio = QWindow().screen()->devicePixelRatio();
        return screenPixelRatio;
    }
    
    template <typename T>
    T scale(T value) { return getDevicePixelRatio() * value; };
}