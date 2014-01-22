#include "./BaseItem.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseItem::BaseItem()
	: _vAlignment(VerticalAlignment::Center)
	, _hAlignment(HorizontalAlignment::Left)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
BaseItem::~BaseItem()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void BaseItem::setText(const std::string& text)
{
	if (_text == text)
		return;

	_text = text;

	QGraphicsLayoutItem::updateGeometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QRectF BaseItem::boundingRect() const 
{
	// Velkost nam udava layout, ak chceme zmenit velkost objektu, tak treba to dat do sizeHint
	return QGraphicsLayoutItem::geometry();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
QSizeF BaseItem::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const 
{
	Q_UNUSED(which);
	Q_UNUSED(constraint);

	switch (which)
	{
	case Qt::MinimumSize:
	case Qt::PreferredSize:
		return measureSize();

	case Qt::MaximumSize:
		// TODO: doriesit branie do uvahy sirku obrazovky, dalo by sa spravit nieco ako word wrap
		return QSizeF(std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max());

	case Qt::MinimumDescent:
	case Qt::NSizeHints:
	default:
		Q_ASSERT(false);
		return QSizeF(0, 0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void BaseItem::setGeometry(const QRectF &rect) 
{
	QPointF position;
	QSizeF size(measureSize());

	QGraphicsLayoutItem* parent = parentLayoutItem();
	if (parent != nullptr) {

		switch (_hAlignment) {

		case HorizontalAlignment::Center: //-V556
			position.setX((rect.width() - size.width()) / 2 + rect.x());
			break;

		case HorizontalAlignment::Left:
			position.setX(rect.x());
			break;

		case HorizontalAlignment::Right:
			position.setX(rect.right() - size.width() + rect.x());
			break;
		}

		switch (_vAlignment) {

		case VerticalAlignment::Center:
			position.setY((rect.height() - size.height()) / 2 + rect.y());
			break;

		case VerticalAlignment::Top:
			position.setY(rect.y());
			break;

		case VerticalAlignment::Bottom:
			position.setY(rect.bottom() - size.height() + rect.y());
			break;
		}
	}

	// Pozor! Musime zavolat super funkciu aby sa nastavila geometry
	QGraphicsLayoutItem::setGeometry(QRectF(position, size));

	QGraphicsItem::update();
}
