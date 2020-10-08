#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QScrollBar>
#include <QMouseEvent>
#include <QLabel>
#include <QGraphicsView>
#include "preloader.h"
#include "ui_mainwindow.h"
#include "GraphicsScene.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
protected:
	void mouseMoveEvent(QMouseEvent*);
private slots:
	void fileMissingSlot(const QString& missingFile);
	void jsonErrorSlot(const QString& error);
	void openFileActionTriggered();
	void languageActionTriggered();
	void updateStatusBar(const double scale, const QPointF mousePos, const QSize imageSize);
	void changeCursorShape(const Qt::CursorShape shape);
signals:
	void loadImageSignal(const QString& fileName);
private:
	void renderUi(const QString& type);
private:
	PreLoader* m_preLoader;
	QGraphicsView* m_view;
	GraphicsScene* m_scene;
    Ui::MainWindowClass ui;
	QMenu* m_fileMenu;
	QAction* m_openFileAction;
	QAction* m_saveAction;
	QAction* m_saveAsAction;

	QAction* m_helpAction;
	QAction* m_donateAction;

	QAction* m_languageAction;

	QLabel* m_labelScale;
	QLabel* m_labelScaleRatioNum;
	QLabel* m_labelRotation;
	QLabel* m_labelRotationNum;
	QLabel* m_labelPixmapSize;
	QLabel* m_labelPixmapSizeNum;
	QLabel* m_labelCursorPos;
	QLabel* m_labelCursorPosNum;
};
