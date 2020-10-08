#include "..\header\RectAreaSelector.h"

RectAreaSelector::RectAreaSelector(QWidget* parent)
{
}

void RectAreaSelector::adjustRect(const QPoint& leftTop, const QPoint& rightBottom)
{
	this->rectArea_ = QRect(leftTop, rightBottom);
	this->update();
}

QRectF RectAreaSelector::boundingRect() const
{
	return QRectF(10,10,10,10);
}

void RectAreaSelector::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(Qt::DashLine);
	painter->drawRect(this->rectArea_);
}

void RectAreaSelector::paintEvent(QPaintEvent* event)
{

}


