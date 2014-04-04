#include "./TextItem.h"

#include <QPainter>

//////////////////////////////////////////////////////////////////////////////////////////////////
TextItem::TextItem(const lua::Ref& style)
{
    int fontSize = style["size"];
    
	_font = std::make_shared<QFont>();
	_font->setPointSize(fontSize);
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
    painter->fillRect(bounds, QBrush(QColor(0, 255, 60, 33)));
	painter->setBrush(QBrush(Qt::black));
	painter->setFont(*_font);
	painter->drawText(bounds, QString::fromStdString(getText()));
}