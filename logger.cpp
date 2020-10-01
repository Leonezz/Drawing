#include "logger.h"

logFile::logFile(const QString&& fileName)
{
	file_ = new QFile(fileName);
	file_->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	assert(file_->isOpen());
}

logFile::~logFile()
{
	file_->close();
	file_->deleteLater();
}

void logFile::write(const QString& msg)
{
	QTextStream out(file_);
	out << msg << endl;
}

Logger::Logger(const QString& fileName)
{
	this->lFile = new logFile(std::move(fileName));
}

Logger& Logger::getInstance()
{
	static Logger logger("./LOG/LOG.log");
	return logger;
}

void Logger::info(const QString& msg)
{
	processMsgLine("INFO   ", std::move(msg));
}

void Logger::debug(const QString& msg)
{
	processMsgLine("DEBUG  ", std::move(msg));
}

void Logger::warning(const QString& msg)
{
	processMsgLine("WARNING", std::move(msg));
}

void Logger::error(const QString& msg)
{
	processMsgLine("ERROR  ", std::move(msg));
}

void Logger::fatal(const QString& msg)
{
	processMsgLine("FATAL  ", std::move(msg));
}

void Logger::processMsgLine(const QString&& type, const QString&& msg)
{
	QString line = this->format.arg(type).arg(QDateTime::currentDateTime().toString()).arg(msg);
	this->lFile->write(line);
}
