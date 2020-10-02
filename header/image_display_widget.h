#pragma once
#include <qwidget.h>
#include <QImageReader>
#include <QPainter>
#include <QPixmap>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QWheelEvent>
#include <QApplication>
class ImageDisplayWidget :
	public QWidget
{
	Q_OBJECT
public:
	explicit ImageDisplayWidget(QWidget* parent = nullptr);
	~ImageDisplayWidget() {};
	const QSize getSize() const;
	const double getScale()noexcept;
	const int getRotationAngle()noexcept;
	const QPoint& getMousePos()noexcept;
public slots:
	void loadImageFromFile(const QString& fileName);
signals:
	void updateStatusBar();
	void updateVerScrollBar(const int val);
	void updateHorScrollBar(const int val);
protected:
	void paintEvent(QPaintEvent*);
	void wheelEvent(QWheelEvent*);
	void mouseMoveEvent(QMouseEvent*);
private:
	QPixmap pixmap_;
	QImageReader reader_;
	QPoint mousePos_;
	double scale_;
	int rotationAngle_;
};

