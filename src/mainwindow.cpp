#include "../header/mainwindow.h"
#include "../header/logger.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->setWindowTitle("Drawing");
	this->m_preLoader = PreLoader::getInstance("zh_CN");
	QObject::connect(this->m_preLoader, &PreLoader::fileMissingSignal,
		this, &MainWindow::fileMissingSlot);
	QObject::connect(this->m_preLoader, &PreLoader::jsonErrorSignal,
		this, &MainWindow::jsonErrorSlot);

	this->m_scene = new GraphicsScene;
	this->m_view = new QGraphicsView(m_scene);
	this->setCentralWidget(m_view);
	this->m_view->setMouseTracking(true);
	this->m_view->showMaximized();
	
	QObject::connect(this, &MainWindow::loadImageSignal,
		this->m_scene, &GraphicsScene::loadPixmapFromFile);
	QObject::connect(this->m_scene, &GraphicsScene::mainWindowChangeCursorShape,
		this, &MainWindow::changeCursorShape);
	QObject::connect(this->m_scene, &GraphicsScene::sendStatusInfo,
		this, &MainWindow::updateStatusBar);
	this->m_fileMenu = new QMenu();
	this->m_openFileAction = new QAction();
	this->m_saveAction = new QAction();
	this->m_saveAsAction = new QAction();
	this->m_fileMenu->addActions({ this->m_openFileAction,this->m_saveAction,this->m_saveAsAction });
	this->menuBar()->addMenu(m_fileMenu);
	this->m_helpAction = new QAction();
	this->m_donateAction = new QAction();
	this->m_languageAction = new QAction();
	this->menuBar()->addActions({ this->m_helpAction,this->m_donateAction ,this->m_languageAction });

	this->m_labelScale = new QLabel;
	this->m_labelScaleRatioNum = new QLabel;
	this->m_labelRotation = new QLabel;
	this->m_labelRotationNum = new QLabel;
	this->m_labelPixmapSize = new QLabel;
	this->m_labelPixmapSizeNum = new QLabel;
	this->m_labelCursorPos = new QLabel;
	this->m_labelCursorPosNum = new QLabel;
	this->statusBar()->addWidget(m_labelCursorPos);
	this->statusBar()->addWidget(m_labelCursorPosNum);
	this->statusBar()->addWidget(m_labelPixmapSize);
	this->statusBar()->addWidget(m_labelPixmapSizeNum);
	this->statusBar()->addWidget(m_labelScale);
	this->statusBar()->addWidget(m_labelScaleRatioNum);
	this->statusBar()->addWidget(m_labelRotation);
	this->statusBar()->addWidget(m_labelRotationNum);

	QObject::connect(this->m_openFileAction, &QAction::triggered,
		this, &MainWindow::openFileActionTriggered);
	QObject::connect(this->m_languageAction, &QAction::triggered,
		this, &MainWindow::languageActionTriggered);
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
	const QString language = this->m_languageAction->text();
	if (language == "����")
		this->renderUi("en");
	else this->renderUi("zh_CN");
}

void MainWindow::updateStatusBar(const double scale, const QPointF mousePos, const QSize imageSize)
{
	this->m_labelScaleRatioNum->setText(QString::number(scale));
	this->m_labelCursorPosNum->setText(QString::number(mousePos.x())
		+ "," + QString::number(mousePos.y()));
	this->m_labelPixmapSizeNum->setText(QString::number(imageSize.width())
		+ "x" + QString::number(imageSize.height()));
}

void MainWindow::changeCursorShape(const Qt::CursorShape shape)
{
	this->setCursor(shape);
}



void MainWindow::renderUi(const QString& type)
{
	this->m_preLoader = PreLoader::getInstance(type);
	this->m_fileMenu->setTitle(this->m_preLoader->querry("menu->file->value"));
	this->m_openFileAction->setText(this->m_preLoader->querry("menu->file->open"));
	this->m_saveAction->setText(this->m_preLoader->querry("menu->file->save"));
	this->m_saveAsAction->setText(this->m_preLoader->querry("menu->file->saveAs"));
	this->m_helpAction->setText(this->m_preLoader->querry("menu->help"));
	this->m_donateAction->setText(this->m_preLoader->querry("menu->donate"));
	this->m_languageAction->setText(this->m_preLoader->querry("menu->language"));

	this->m_labelScale->setText(this->m_preLoader->querry("status->scale"));
	this->m_labelRotation->setText(this->m_preLoader->querry("status->rotate"));
	this->m_labelPixmapSize->setText(this->m_preLoader->querry("status->size"));
	this->m_labelCursorPos->setText(this->m_preLoader->querry("status->cursor"));
}

