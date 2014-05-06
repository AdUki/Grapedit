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

/// Vertikálne a horizontálne usporiadanie pomocou QtLinearLayout
class LinearLayout : public Layout
{
public:
    
    /// Konštruktor pre root prvok, ktorý sa vloží ako prvý do scény ako vertikálne usporiadanie.
    LinearLayout(Qt::Orientation orientation);
    
    /// Konštruktor pre usporiadania, ktoré obsahujú ďalšie usporiadania, alebo prvky
    ///
    /// @param style       Inicializovaná referencia na pole "value" uzla AST
    /// @param orientation Orientácia usporiadania, môže byť vertikálna alebo horizontálna
    LinearLayout(const lua::Value& style, Qt::Orientation orientation);
    
protected:
    
    /// Preťažená metóda na pridávanie prvok do usporiadania. Konkrétne do QtLinearLayout
    ///
    /// @param index Pozícia, na ktorú sa má pridať nový prvok
    /// @param item  Prvok, ktorý sa ide pridať do usporiadania
    void insertItem(size_t index, const ItemPtr& item) override;
    
    /// Preťažená metóda na pridávanie ďalších usporiadaní do usporiadania. Konkrétne do QtLinearLayout
    ///
    /// @param index Pozícia, na ktorú sa má pridať nové usporiadanie
    /// @param item  Usporiadanie, ktoré sa ide pridať do tohto usporiadania
    void insertLayout(size_t index, const LayoutPtr& layout) override;
    
private:
    typedef Layout super;
};
