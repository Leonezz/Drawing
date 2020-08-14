#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
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
	this->renderUi("zh_CN");

}

void MainWindow::fileMissingSlot(const QString& missingFile)
{
	QMessageBox::critical(this, "Error", missingFile,
		QMessageBox::Button::Close);
	QApplication::exit();
}

void MainWindow::jsonErrorSlot(const QString& error)
{
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



void MainWindow::renderUi(const QString& type)
{
	this->preLoader_ = PreLoader::getInstance(type);
	this->fileMenu_ = new QMenu();
	this->fileMenu_->setTitle(this->preLoader_->querry("menu->file->value"));
	this->openFileAction_ = new QAction();
	this->openFileAction_->setText(this->preLoader_->querry("menu->file->open"));
	this->saveAction_ = new QAction();
	this->saveAction_->setText(this->preLoader_->querry("menu->file->save"));
	this->saveAsAction_ = new QAction();
	this->saveAsAction_->setText(this->preLoader_->querry("menu->file->saveAs"));
	this->fileMenu_->addActions({ this->openFileAction_,this->saveAction_,this->saveAsAction_ });
	this->menuBar()->addMenu(fileMenu_);

	this->helpAction_ = new QAction();
	this->helpAction_->setText(this->preLoader_->querry("menu->help"));
	this->donateAction_ = new QAction();
	this->donateAction_->setText(this->preLoader_->querry("menu->donate"));
	this->menuBar()->addActions({ this->helpAction_,this->donateAction_ });

	QObject::connect(this->openFileAction_, &QAction::triggered,
		this, &MainWindow::openFileActionTriggered);
}

