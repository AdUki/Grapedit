#include "./TextItem.h"

#include <QPainter>

//////////////////////////////////////////////////////////////////////////////////////////////////
TextItem::TextItem(const lua::Ref& style)
: Item(style)
, _textColor(Qt::black)
{
    _font = std::make_shared<QFont>();
    _font->setPointSize(20);
    
    lua::Ref textStyle = style["text"];
    if (!textStyle.is<lua::Table>())
        return;
    
    _font = std::make_shared<QFont>();
    if (textStyle["size"].is<lua::Number>())
        _font->setPointSize(textStyle["size"]);
    
    if (textStyle["color"].is<lua::Table>()) {
        lua::Ref color = textStyle["color"];
        if (color[1].is<lua::Number>())
            _textColor.setRedF(color[1]);
        
        if (color[2].is<lua::Number>())
            _textColor.setGreenF(color[2]);
        
        if (color[3].is<lua::Number>())
            _textColor.setBlueF(color[3]);
        
        if (color[4].is<lua::Number>())
            _textColor.setAlphaF(color[4]);
    }
    
    if (textStyle["bold"].is<lua::Boolean>())
        _font->setBold(textStyle["bold"]);
    else
        _font->setBold(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TextItem::~TextItem()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QSizeF TextItem::measureSize() const 
{
	QFontMetrics fm(*_font);

	// NOTE: flag Qt::TextSingleLine ignores newline characters.
	return fm.size(Qt::TextExpandTabs, QString::fromStdString(getText()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void TextItem::setFont(const QFontPtr& font)
{
	_font = font;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void TextItem::drawContent(QPainter *painter, const QRectF& bounds)
{
    painter->setPen(_textColor);
	painter->setFont(*_font);
	painter->drawText(bounds, QString::fromStdString(getText()));
}