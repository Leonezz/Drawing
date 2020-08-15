#pragma once
#include <qwidget.h>
#include <QImageReader>
#include <QPainter>
#include <QPixmap>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
class ImageDisplayWidget :
	public QWidget
{
	Q_OBJECT
public:
	explicit ImageDisplayWidget(QWidget* parent = nullptr);
	~ImageDisplayWidget() {};
	const QSize getSize() const;
public slots:
	void loadImageFromFile(const QString& fileName);

protected:
	void paintEvent(QPaintEvent*);
private:
	QImage image_;
	QImageReader reader_;
};

