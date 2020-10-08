#pragma once
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <qgraphicssceneevent.h>
#include <qcursor.h>
#include "BorderLine.h"
class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit GraphicsScene();
	~GraphicsScene() = default;
signals:
	void resizeBorderLine(const QSize& size);
	void mainWindowChangeCursorShape(const Qt::CursorShape shape);
	void sendStatusInfo(const double scaleRatio, const QPointF mousePos,const QSize imageSize);
public slots:
	void loadPixmapFromFile(const QString& fileName);
	void setPixmapScale(const double scale);
	void changeCursorSlot(const Qt::CursorShape shape);
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
	void mousePressEvent(QGraphicsSceneMouseEvent* e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
	void wheelEvent(QGraphicsSceneWheelEvent* e);
private:
	void myUpdate();
	QGraphicsPixmapItem* m_pixmapItem;
	BorderLine* m_pixmapBorder;
	QPixmap m_pixmap;
	double m_scaleRatio;
	QPointF m_mousePos;
	QSize m_paintSize;
	static constexpr int Padding = 10;
	static constexpr double ScaleLowLimit = 0.1;
	static constexpr double ScaleHighLimit = 20;
	static constexpr int WheelStepDiv = 360;
};

