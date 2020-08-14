/**
* author : Leonezz
* discription : 
*	this file contains a class that loads strings from json file,strings in that json file
* is writen for rendering mainwindow ui.Such as the main title,menu bar,etc.
**/

#pragma once
#include <qobject.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
class PreLoader :
	public QObject
{
	Q_OBJECT
public:
	static PreLoader* getInstance(const QString& type);
	//get json data.
	const QString querry(const QString& key)const;
signals:
	//emit when error occurs.
	void fileMissingSignal(const QString& fileName);
	void jsonErrorSignal(const QString& error)const;
private:
	static PreLoader* instance_;
	PreLoader(const QString& type);
	QJsonObject* preSetting_;
	static QString type_;
};

