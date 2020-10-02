#include "../header/mainwindow.h"
#include "../header/logger.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->setWindowTitle("Drawing");

	this->preLoader_ = PreLoader::getInstance("zh_CN");
	this->imageWidget_ = new ImageDisplayWidget();
	this->ui.scrollArea->setWidget(imageWidget_);
	this->ui.scrollArea->setWidgetResizable(true);
	QObject::connect(this, &MainWindow::loadImageSignal,
		this->imageWidget_, &ImageDisplayWidget::loadImageFromFile);
	QObject::connect(this->preLoader_, &PreLoader::fileMissingSignal,
		this, &MainWindow::fileMissingSlot);
	QObject::connect(this->preLoader_, &PreLoader::jsonErrorSignal,
		this, &MainWindow::jsonErrorSlot);
	QObject::connect(this->imageWidget_, &ImageDisplayWidget::updateHorScrollBar,
		this, &MainWindow::updateHorScrollBar);
	QObject::connect(this->imageWidget_, &ImageDisplayWidget::updateVerScrollBar,
		this, &MainWindow::updateVerScrollBar);


	this->fileMenu_ = new QMenu();
	this->openFileAction_ = new QAction();
	this->saveAction_ = new QAction();
	this->saveAsAction_ = new QAction();
	this->fileMenu_->addActions({ this->openFileAction_,this->saveAction_,this->saveAsAction_ });
	this->menuBar()->addMenu(fileMenu_);
	this->helpAction_ = new QAction();
	this->donateAction_ = new QAction();
	this->languageAction_ = new QAction();
	this->menuBar()->addActions({ this->helpAction_,this->donateAction_ ,this->languageAction_ });

	this->labelScale_ = new QLabel;
	this->labelScaleRatioNum_ = new QLabel;
	this->labelRotation_ = new QLabel;
	this->labelRotationNum_ = new QLabel;
	this->labelPixmapSize_ = new QLabel;
	this->labelPixmapSizeNum_ = new QLabel;
	this->labelCursorPos_ = new QLabel;
	this->labelCursorPosNum_ = new QLabel;
	this->statusBar()->addWidget(labelCursorPos_);
	this->statusBar()->addWidget(labelCursorPosNum_);
	this->statusBar()->addWidget(labelPixmapSize_);
	this->statusBar()->addWidget(labelPixmapSizeNum_);
	this->statusBar()->addWidget(labelScale_);
	this->statusBar()->addWidget(labelScaleRatioNum_);
	this->statusBar()->addWidget(labelRotation_);
	this->statusBar()->addWidget(labelRotationNum_);

	//set mouse tracking so the mouseMoveEvent can work.

	QObject::connect(this->openFileAction_, &QAction::triggered,
		this, &MainWindow::openFileActionTriggered);
	QObject::connect(this->languageAction_, &QAction::triggered,
		this, &MainWindow::languageActionTriggered);
	QObject::connect(this->imageWidget_, &ImageDisplayWidget::updateStatusBar,
		this, &MainWindow::updateStatusBar);
	this->renderUi("zh_CN");
}

void MainWindow::fileMissingSlot(const QString& missingFile)
{
	Logger::getInstance().fatal("setting file missed.");
	QMessageBox::critical(this, "Error", missingFile,
		QMessageBox::Button::Close);
	QApplication::exit();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
}

void MainWindow::jsonErrorSlot(const QString& error)
{
	Logger::getInstance().fatal("json error at the setting file.");
	QMessageBox::critical(this, "Error", error,
		QMessageBox::Button::Close);
	QApplication::exit();
}

void MainWindow::openFileActionTriggered()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("open file"),
		".", tr("Images (*.png);\n\
				 (*.jpg)\n\
				(*.jpeg)"));
	if (filePath.isEmpty())
		QMessageBox::warning(this, tr("OpenFile"), tr("Empty File Path."), QMessageBox::Yes);
	else
		emit loadImageSignal(filePath);
}

void MainWindow::languageActionTriggered()
{
	const QString language = this->languageAction_->text();
	if (language == "中文")
		this->renderUi("en");
	else this->renderUi("zh_CN");
}

void MainWindow::updateStatusBar()
{
	this->labelScaleRatioNum_->setText(QString::number(this->imageWidget_->getScale()));
	this->labelRotationNum_->setText(QString::number(this->imageWidget_->getRotationAngle()));
	const QSize&& imgSize = this->imageWidget_->getSize();
	QString sizeNumStr = QString::number(imgSize.width()) +
		" x " + QString::number(imgSize.height());
	this->labelPixmapSizeNum_->setText(sizeNumStr);

	const QPoint& mousePos = this->imageWidget_->getMousePos();
	QString mousePosNumStr = QString::number(mousePos.x()) +
		" , " + QString::number(mousePos.y());
	this->labelCursorPosNum_->setText(mousePosNumStr);
}

void MainWindow::updateVerScrollBar(const int val)
{
	int value = ui.scrollArea->verticalScrollBar()->value() - val;
	this->ui.scrollArea->verticalScrollBar()->setValue(value);
}

void MainWindow::updateHorScrollBar(const int val)
{
	int value = ui.scrollArea->horizontalScrollBar()->value() - val;
	this->ui.scrollArea->horizontalScrollBar()->setValue(value);
}



void MainWindow::renderUi(const QString& type)
{
	this->preLoader_ = PreLoader::getInstance(type);
	this->fileMenu_->setTitle(this->preLoader_->querry("menu->file->value"));
	this->openFileAction_->setText(this->preLoader_->querry("menu->file->open"));
	this->saveAction_->setText(this->preLoader_->querry("menu->file->save"));
	this->saveAsAction_->setText(this->preLoader_->querry("menu->file->saveAs"));
	this->helpAction_->setText(this->preLoader_->querry("menu->help"));
	this->donateAction_->setText(this->preLoader_->querry("menu->donate"));
	this->languageAction_->setText(this->preLoader_->querry("menu->language"));

	this->labelScale_->setText(this->preLoader_->querry("status->scale"));
	this->labelRotation_->setText(this->preLoader_->querry("status->rotate"));
	this->labelPixmapSize_->setText(this->preLoader_->querry("status->size"));
	this->labelCursorPos_->setText(this->preLoader_->querry("status->cursor"));
}

