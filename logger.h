#pragma once
#include <qobject.h>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QDateTime>
class logFile :public QObject
{
public:
	logFile(const QString&& fileName);
	~logFile();
	void write(const QString& msg);
private:
	QFile* file_;
};

class Logger :
	public QObject
{
public:
	static Logger& getInstance();
	void info(const QString& msg);
	void debug(const QString& msg);
	void warning(const QString& msg);
	void error(const QString& msg);
	void fatal(const QString& msg);
private:
	explicit Logger(const QString& fileName);
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	void processMsgLine(const QString&& type, const QString&& msg);
	logFile* lFile;
	const QString format = "[%1][Time: %2]MSG: %3";
};


