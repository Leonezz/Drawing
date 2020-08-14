#pragma once
#include <qobject.h>
#include <QJsonObject>
class PreLoader :
	public QObject
{
	Q_OBJECT
public:
	explicit PreLoader(const QString& type);
private:

};

