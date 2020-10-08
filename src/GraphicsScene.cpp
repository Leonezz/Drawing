#include "../header/GraphicsScene.h"

GraphicsScene::GraphicsScene()
	:m_pixmapItem(new QGraphicsPixmapItem), m_pixmapBorder(new BorderLine)
	, m_scaleRatio(1), m_paintSize(QSize(800, 600)), m_mousePos(0, 0)
{
	this->addItem(m_pixmapItem);
	this->addItem(m_pixmapBorder);
	this->setBackgroundBrush(QBrush(Qt::gray));

	m_pixmapItem->show();
	m_pixmapBorder->show();

	QObject::connect(this, &GraphicsScene::resizeBorderLine,
		m_pixmapBorder, &BorderLine::setBorderSize);
	QObject::connect(this->m_pixmapBorder, &BorderLine::changeCursorType,
		this, &GraphicsScene::changeCursorSlot);
}

void GraphicsScene::setPixmapScale(const double scale)
{
	if (scale < ScaleLowLimit || scale > ScaleHighLimit)return;
	this->m_scaleRatio = scale;
	myUpdate();
}

void GraphicsScene::changeCursorSlot(const Qt::CursorShape shape)
{
	emit mainWindowChangeCursorShape(shape);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
	m_mousePos = e->scenePos() - QPointF(Padding, Padding);
	emit sendStatusInfo(m_scaleRatio, m_mousePos, m_pixmap.size());
	QGraphicsScene::mouseMoveEvent(e);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	QGraphicsScene::mousePressEvent(e);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	QGraphicsScene::mouseReleaseEvent(e);
}

void GraphicsScene::wheelEvent(QGraphicsSceneWheelEvent* e)
{
	double value = (double)e->delta() / WheelStepDiv + m_scaleRatio;
	this->setPixmapScale(value);
	emit sendStatusInfo(m_scaleRatio, m_mousePos, m_pixmap.size());
	e->accept();
}

void GraphicsScene::myUpdate()
{
	if (!m_pixmap.isNull()) {
		this->m_paintSize = m_scaleRatio * m_pixmap.size();
	}
	this->m_pixmapItem->setScale(m_scaleRatio);
	emit resizeBorderLine(m_paintSize);
	this->m_pixmapItem->setPos(Padding, Padding);
	this->m_pixmapBorder->setPos(Padding, Padding);
	this->update();
}

void GraphicsScene::loadPixmapFromFile(const QString& fileName)
{
	this->m_pixmap.load(fileName);
	this->m_pixmapItem->setPixmap(m_pixmap);
	m_scaleRatio = 1;
	myUpdate();
}