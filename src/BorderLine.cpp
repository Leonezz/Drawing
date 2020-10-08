#include "../header/BorderLine.h"

BorderLine::BorderLine(QObject* parent)
	:m_border(new QGraphicsRectItem),
	m_topLeftCorner(new QGraphicsRectItem),
	m_topRightCorner(new QGraphicsRectItem),
	m_bottomLeftCorner(new QGraphicsRectItem),
	m_bottomRightCorner(new QGraphicsRectItem),
	m_top(new QGraphicsRectItem),
	m_bottom(new QGraphicsRectItem),
	m_left(new QGraphicsRectItem),
	m_right(new QGraphicsRectItem),
	m_bottomRightPos({0, 0}),
	m_mouseState(MouseState::Empty),
	m_leftKeyPressed(false)
{
	this->addToGroup(m_border);
	this->addToGroup(m_topLeftCorner);
	this->addToGroup(m_topRightCorner);
	this->addToGroup(m_bottomLeftCorner);
	this->addToGroup(m_bottomRightCorner);
	this->addToGroup(m_top);
	this->addToGroup(m_bottom);
	this->addToGroup(m_left);
	this->addToGroup(m_right);
	QRectF cornerSize(0, 0, CornerSize, CornerSize);
	m_border->setRect(0, 0, 0, 0);
	m_topLeftCorner->setRect(cornerSize);
	m_topRightCorner->setRect(cornerSize);
	m_bottomLeftCorner->setRect(cornerSize);
	m_bottomRightCorner->setRect(cornerSize);
	m_top->setRect(cornerSize);
	m_bottom->setRect(cornerSize);
	m_left->setRect(cornerSize);
	m_right->setRect(cornerSize);

	m_border->setPen(Qt::DashLine);
	this->m_topLeftCorner->setAcceptHoverEvents(true);
	this->m_topRightCorner->setAcceptHoverEvents(true);
	this->m_bottomLeftCorner->setAcceptHoverEvents(true);
	this->m_bottomRightCorner->setAcceptHoverEvents(true);
	this->m_top->setAcceptHoverEvents(true);
	this->m_bottom->setAcceptHoverEvents(true);
	this->m_left->setAcceptHoverEvents(true);
	this->m_right->setAcceptHoverEvents(true);

	this->setCacheMode(QGraphicsItem::CacheMode::NoCache);
}

void BorderLine::setBorderSize(const QSize& size)
{
	m_bottomRightPos.setX(size.width());
	m_bottomRightPos.setY(size.height());
	this->updateItems();
}

// if left button pressed and the mouse is moving,the selected corner will move.
// and the border rect will scale with the mouse cursor.
void BorderLine::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
	if (!m_leftKeyPressed)return;
	if (m_mouseState == MouseState::Empty)return;
	const QPointF mousePos = e->pos();
	qreal deltaX = 0;
	qreal deltaY = 0;
	switch (m_mouseState)
	{
	case MouseState::HoverBottom:
		deltaY = mousePos.y() - m_bottomRightPos.y();
		break;
	case MouseState::HoverTop:
		deltaY = - mousePos.y();
		break;
	case MouseState::HoverLeft:
		deltaX = - mousePos.x();
		break;
	case MouseState::HoverRight:
		deltaX = mousePos.x() - m_bottomRightPos.x();
		break;
	case MouseState::HoverBottomLeft:
		deltaX = - mousePos.x();
		deltaY = mousePos.y() - m_bottomRightPos.y();
		break;
	case MouseState::HoverTopLeft:
		deltaX = - mousePos.x();
		deltaY = - mousePos.y();
		break;
	case MouseState::HoverBottomRight:
		deltaX = mousePos.x() - m_bottomRightPos.x();
		deltaY = mousePos.y() - m_bottomRightPos.y();
		break;
	case MouseState::HoverTopRight:
		deltaX = m_bottomRightPos.x() - mousePos.x();
		deltaY = mousePos.y();
		break;
	default:
		// pragm never run here.
		break;
	}
	m_bottomRightPos.setX(m_bottomRightPos.x() + deltaX < 0 ? 
		0 : m_bottomRightPos.x() + deltaX);
	m_bottomRightPos.setY(m_bottomRightPos.y() + deltaY < 0 ? 
		0 : m_bottomRightPos.y() + deltaY);
	updateItems();
	QGraphicsItemGroup::mouseMoveEvent(e);
}

void BorderLine::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	if (e->button() == Qt::MouseButton::LeftButton) {
		m_leftKeyPressed = true;
	}
	e->accept();
}

void BorderLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	if (e->button() == Qt::MouseButton::LeftButton) {
		m_leftKeyPressed = false;
	}
	QGraphicsItemGroup::mouseReleaseEvent(e);
}

void BorderLine::hoverEnterEvent(QGraphicsSceneHoverEvent* e)
{
	this->determinCursorType();
}

void BorderLine::hoverLeaveEvent(QGraphicsSceneHoverEvent* e)
{
	m_mouseState = MouseState::Empty;
	determinCursorType();
}

void BorderLine::hoverMoveEvent(QGraphicsSceneHoverEvent* e)
{
	this->determinCursorType();
}

void BorderLine::setCursorIcon(MouseState state)
{
	const static QMap<MouseState, Qt::CursorShape> map{
		{MouseState::Empty,Qt::CursorShape::ArrowCursor},
	{MouseState::HoverTopLeft,Qt::CursorShape::SizeFDiagCursor},// *\*
	{MouseState::HoverTopRight,Qt::CursorShape::SizeBDiagCursor},// */*
	{MouseState::HoverBottomLeft,Qt::CursorShape::SizeBDiagCursor},// */*
	{MouseState::HoverBottomRight,Qt::CursorShape::SizeFDiagCursor},
	{MouseState::HoverTop,Qt::CursorShape::SizeVerCursor},
	{MouseState::HoverBottom,Qt::CursorShape::SizeVerCursor},
	{MouseState::HoverLeft,Qt::CursorShape::SizeHorCursor},
	{MouseState::HoverRight,Qt::CursorShape::SizeHorCursor},
	};
	emit changeCursorType(map[state]);
	//setCursor(map[state]);
}

void BorderLine::determinCursorType()
{
	m_mouseState = MouseState::Empty;
	if (m_topLeftCorner->isUnderMouse())
		m_mouseState = MouseState::HoverTopLeft;
	else if (m_topRightCorner->isUnderMouse())
		m_mouseState = MouseState::HoverTopRight;
	else if (m_bottomLeftCorner->isUnderMouse())
		m_mouseState = MouseState::HoverBottomLeft;
	else if (m_bottomRightCorner->isUnderMouse())
		m_mouseState = MouseState::HoverBottomRight;
	else if (m_top->isUnderMouse())
		m_mouseState = MouseState::HoverTop;
	else if (m_left->isUnderMouse())
		m_mouseState = MouseState::HoverLeft;
	else if (m_right->isUnderMouse())
		m_mouseState = MouseState::HoverRight;
	else if (m_bottom->isUnderMouse())
		m_mouseState = MouseState::HoverBottom;
	setCursorIcon(m_mouseState);
}

void BorderLine::updateItems()
{
	m_topLeftCorner->setPos(0, 0);
	m_topRightCorner->setPos(m_bottomRightPos.x(), 0);
	m_top->setPos(m_bottomRightPos.x() / 2, 0);
	m_left->setPos(0, m_bottomRightPos.y() / 2);
	m_right->setPos(m_bottomRightPos.x(), m_bottomRightPos.y() / 2);
	m_bottomLeftCorner->setPos(0, m_bottomRightPos.y());
	m_bottomRightCorner->setPos(m_bottomRightPos);
	m_bottom->setPos(m_bottomRightPos.x() / 2, m_bottomRightPos.y());
	m_border->setRect(0, 0, m_bottomRightPos.x(), m_bottomRightPos.y());
	this->update();
}
