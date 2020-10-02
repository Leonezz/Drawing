#include "../header/image_display_widget.h"
#include <qpushbutton.h>
#include <qcursor.h>
constexpr int RATIO_TO_DOUBLE = 8 * 360;
ImageDisplayWidget::ImageDisplayWidget(QWidget* parent)
	:QWidget(parent), scale_(1), rotationAngle_(0)
{
	this->pixmap_.scaled(QSize(600, 800));
	this->setMouseTracking(true);
	update();
}

void ImageDisplayWidget::loadImageFromFile(const QString& fileName)
{
	pixmap_.load(fileName);
	update();
}

const QSize ImageDisplayWidget::getSize() const
{
	return this->pixmap_.size() * scale_;
}

const double ImageDisplayWidget::getScale() noexcept
{
	return scale_;
}

const int ImageDisplayWidget::getRotationAngle() noexcept
{
	return rotationAngle_;
}

const QPoint& ImageDisplayWidget::getMousePos() noexcept
{
	return this->mousePos_;
}

void ImageDisplayWidget::paintEvent(QPaintEvent* event)
{
	//rotate the angle
	if (this->rotationAngle_ != 0) {
		QMatrix matrix;
		matrix.rotate(this->rotationAngle_);
		this->pixmap_ = this->pixmap_.transformed(matrix, Qt::FastTransformation);
	}

	QSize paintSize(pixmap_.width() * scale_, pixmap_.height() * scale_);
	QPoint widgetStartPos = this->pos();
	QSize widgetSize = this->size();

	this->setMinimumSize(paintSize);
	QPainter painter(this);
	painter.drawPixmap(QRect(0, 0, paintSize.width(), paintSize.height()), pixmap_);

	emit updateStatusBar();

	QWidget::paintEvent(event);
}

void ImageDisplayWidget::wheelEvent(QWheelEvent* event)
{
	//scale
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		int value = event->delta();
		double scaleRatio = ((double)value) / RATIO_TO_DOUBLE;
		this->scale_ += scaleRatio;
		if (this->scale_ < 0.2)this->scale_ = 0.2;
		this->update();
	}

	//scollbar ver
	else if (QApplication::queryKeyboardModifiers() == Qt::ShiftModifier) {
		int value = event->angleDelta().y();
		emit updateHorScrollBar(value);
	}

	//scrollbar hor
	else {
		int value = event->angleDelta().y();
		emit updateVerScrollBar(value);
	}
	event->accept();
}

void ImageDisplayWidget::mouseMoveEvent(QMouseEvent*event)
{
	mousePos_ = QPoint(event->localPos().x(), event->localPos().y());
	this->update();
	QWidget::mouseMoveEvent(event);
}
