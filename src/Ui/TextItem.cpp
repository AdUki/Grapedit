#include "./TextItem.h"

#include <QPainter>

#include "../Utils/LuaReader.h"

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
    
    lua::readColor(textStyle["color"], _textColor);
    
    if (textStyle["bold"].is<lua::Boolean>())
        _font->setBold(textStyle["bold"]);
    else
        _font->setBold(false);
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
void TextItem::draw(QPainter *painter, const QRectF& bounds)
{
    super::draw(painter, bounds);
    
    painter->setPen(_textColor);
	painter->setFont(*_font);
	painter->drawText(bounds, QString::fromStdString(getText()));
}