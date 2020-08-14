#include "image_display_widget.h"
#include <qpushbutton.h>
ImageDisplayWidget::ImageDisplayWidget(QWidget* parent)
	:QWidget(parent)
{
	this->image_.scaled(QSize(600, 800));
	
	update();
}

void ImageDisplayWidget::loadImageFromFile(const QString& fileName)
{
	reader_.setFileName(fileName);
	image_ = reader_.read();
	update();
}

const QSize ImageDisplayWidget::getSize() const
{
	return this->image_.size();
}

void ImageDisplayWidget::paintEvent(QPaintEvent*)
{
	this->setMinimumSize(this->image_.size());
	QPainter painter(this);
	painter.drawPixmap(0, 0, QPixmap::fromImage(image_));
	
}
