#pragma once
#include <qgraphicsitem.h>
#include <QPen>
#include <QCursor>
#include <QMap>
#include <QGraphicsSceneMouseEvent>
enum MouseState {
	HoverTopLeft,
	HoverTopRight,
	HoverBottomLeft,
	HoverBottomRight,
	HoverTop,
	HoverBottom,
	HoverLeft,
	HoverRight,
	Empty
};
class BorderLine : public QObject,public QGraphicsItemGroup
{
	Q_OBJECT
public:
	BorderLine(QObject* parent = nullptr);
	~BorderLine() = default;
signals:
	void changeCursorType(const Qt::CursorShape type);
public:
	void setBorderSize(const QSize& size);
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
	void mousePressEvent(QGraphicsSceneMouseEvent* e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
	void hoverEnterEvent(QGraphicsSceneHoverEvent* e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* e);
	void hoverMoveEvent(QGraphicsSceneHoverEvent* e);
private:
	void setCursorIcon(MouseState state);
	void determinCursorType();
	void updateItems();
private:
	QGraphicsRectItem* m_border;
	QGraphicsRectItem* m_topLeftCorner;
	QGraphicsRectItem* m_topRightCorner;
	QGraphicsRectItem* m_bottomLeftCorner;
	QGraphicsRectItem* m_bottomRightCorner;
	QGraphicsRectItem* m_top;
	QGraphicsRectItem* m_bottom;
	QGraphicsRectItem* m_left;
	QGraphicsRectItem* m_right;

	QPointF m_bottomRightPos;

	MouseState m_mouseState;
	bool m_leftKeyPressed;
	static constexpr int CornerSize = 7;
	static constexpr int Padding = 10;
};

