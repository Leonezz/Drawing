#pragma once
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsItem>
class RectAreaSelector : public QGraphicsItem
{

public:
	RectAreaSelector(QWidget* parent = nullptr);
	void adjustRect(const QPoint& leftTop, const QPoint& rightBottom);
	QRectF boundingRect()const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
protected:
	void paintEvent(QPaintEvent* event);

private:
	QRect rectArea_;
	QWidget* parent_;
};

