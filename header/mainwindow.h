#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QScrollBar>
#include <QMouseEvent>
#include "image_display_widget.h"
#include "preloader.h"
#include "ui_mainwindow.h"

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
	void updateStatusBar();
	void updateVerScrollBar(const int val);
	void updateHorScrollBar(const int val);
signals:
	void loadImageSignal(const QString& fileName);
private:
	void renderUi(const QString& type);
private:
	PreLoader* preLoader_;
	ImageDisplayWidget* imageWidget_;
    Ui::MainWindowClass ui;
	QMenu* fileMenu_;
	QAction* openFileAction_;
	QAction* saveAction_;
	QAction* saveAsAction_;

	QAction* helpAction_;
	QAction* donateAction_;

	QAction* languageAction_;

	QLabel* labelScale_;
	QLabel* labelScaleRatioNum_;
	QLabel* labelRotation_;
	QLabel* labelRotationNum_;
	QLabel* labelPixmapSize_;
	QLabel* labelPixmapSizeNum_;
	QLabel* labelCursorPos_;
	QLabel* labelCursorPosNum_;
};
