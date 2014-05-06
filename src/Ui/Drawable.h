//
//  Drawable.h
//  Grapedit
//
//  Created by Simon Mikuda on 01/04/14.
//
//

#pragma once

#include "pch.h"

#include <QGraphicsItem>
#include <QRectF>
#include <qnamespace.h>

#include "../Utils/Geometry.h"

class QPainter;

DECLARE_CLASS_PTR(Layout);
DECLARE_CLASS_PTR(Drawable);

/// Všetky triedy, ktoré sa vykreslia na obrazovke, dedia z tejto triedy
class Drawable : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    
public:
    
    Drawable();
    
    /// Konštruktor nastaví tie štýly, ktoré sú spoločné pre všekty prvky
    ///
    /// @param style Inicializovaná referencia na pole "value" uzla AST
    Drawable(const lua::Value& style);
    
    /// V deštruktore sa iba vypíše debug informácia o zmazaní prvku.
    virtual ~Drawable();
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Metóda, ktorá nakreslí objekt
    ///
    /// @param painter Canvas, na ktorý kreslíme
    /// @param bounds  Obdĺžnik, kam môžme kresliť
    virtual void draw(QPainter *painter, const QRectF& bounds) {}
    
    /// Metóda, ktorá zmeria a vráti veľkosť prvku
    ///
    /// @return Veľkosť prvku
    virtual QRectF boundingRect() const = 0;
    
    /// Metóda slúži na prepojenie medzi AST a obyčajným reťazcom znakov. Vráti do premenných štartovaciu a koncovú pozíciu obsahujúcich znakov v pôvodnom reťazci.
    ///
    /// @param left  Výstupná premenná, ktorá určuje ľavý index v reťazci
    /// @param right Výstupná premenná, ktorá určuje pravý index v reťazci
    virtual void findTextBoundaries(size_t& left, size_t& right) const = 0;
    
    /// Metóda na získanie počtu znakov, ktoré obsahuje daný prvok
    virtual size_t calculateTextLenght() const = 0;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Nastaví rodiča, ktorý obsahuje tento prvok.
    ///
    /// @param parent Rodič obsahujúci tento prvok, môže byť len usporiadanie
    void setParent(const LayoutPtr& parent) { _parent = parent; }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////

    /// Preťažená metóda na odchytenie príchodu kurzora myši ponad objekt
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    
    /// Preťažená metóda na odchytenie odchodu kurzora myši ponad objekt
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    
    /// Preťažená metóda na odchytenie stlačenia tlačidla  myši na objekt
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// Funkcia vráti veľkosť okrajov pre tento prvok.
    const RectInset& getContentInset() const { return _contentInset; }
    
    /// Funkcia vráti rodiča pre tento prvok
    LayoutPtr getParent() const { return _parent.lock(); }
    
    /// Funkcia vráti, či nad týmto objektom je práve kurzor myši
    bool isHighlighted() const { return _highlighted; }
    
    /// Funkcia vráti, či daný prvok je práve vybraný kuzorom myši
    bool isSelected() const { return _selected; }
    
signals:
    
    /// Signál na notifikáciu o kliknutí myšov na prvok.
    void onElementClicked(Qt::MouseButton button);
    
protected:
    
    /// Preťažená metóda na prekreslenie prvku.
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) final;
    
    /// Metóda na nájdenie ľavého indexu v originálnom reťazci. Pravý sa už dopočíta podľa funkcie calculateTextLenght.
    size_t findLeftOffset() const;
    
private:
    
    /// Veľkosť okrajov daného prvku
    RectInset _contentInset;
    
    /// Rodič daného prvku
    LayoutWPtr _parent;
    
    /// Udáva, či nad daným prvok sa práve nachádza kurzor myši
    bool _highlighted;
    
    /// Udáva, či je daný prvok bol vybraný kurzorom myši
    bool _selected;
    
    /// Farba pozadia
    boost::optional<QColor> _backgroundColor;
    
    /// Polomer zaoblenia rohov pozadia
    qreal _backgroundRadius;
    
    /// Metóda na nakreslenie pozadia.
    void drawBackground(QPainter *painter, const QRectF& bounds);
    
    /// Rekurzívna metóda na hľadanie ľavého indexu v originálnom reťazci.
    size_t calculateLeftOffset(size_t elementIndex) const;
};
