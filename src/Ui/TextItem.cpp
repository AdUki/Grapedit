#include "./TextItem.h"

#include <QPainter>

#include "../Utils/LuaReader.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
TextItem::TextItem(const lua::Value& style)
: Item(style)
, _textColor(Qt::black)
{
    _font = std::make_shared<QFont>();
    _font->setPointSize(20);
    
    lua::Value textStyle = style["text"];
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
QSizeF TextItem::measureSize(const boost::optional<QSize>& maxSize) const
{
	QFontMetrics fm(*_font);
    
    QString text(QString::fromStdString(getText()));
    text = trim(text);

    if (maxSize.is_initialized()) {
        QRect bounds = fm.boundingRect(0, 0,
                                       maxSize->width() > 0 ? maxSize->width() : std::numeric_limits<int>::max(),
                                       maxSize->height() > 0 ? maxSize->height() : std::numeric_limits<int>::max(),
                                       Qt::TextExpandTabs | Qt::TextWrapAnywhere, text);
        
        return QSizeF(std::max(bounds.size().width(), maxSize->width()),
                      std::max(bounds.size().height(), maxSize->height()));
    }
    else
        return fm.size(Qt::TextExpandTabs, text);
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
    
    QString text(QString::fromStdString(getText()));
    text = trim(text);
    
	painter->drawText(bounds, Qt::TextWrapAnywhere, text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QString TextItem::trim(const QString& text)
{
    int n = text.size() - 1;
    for (; n >= 0; --n) {
        if (!text.at(n).isSpace()) {
            return n == text.size() - 1 ? text : text.left(n + 1).append(' ');
        }
    }
    return "";
}
