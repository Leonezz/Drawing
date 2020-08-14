#include "preloader.h"
const QString PreLoader::querry(const QString& key) const
{
	QStringList keys = key.split("->");
	QJsonObject obj = *preSetting_;
	for (int i = 0; i < keys.length() - 1;++i) {
		QString k = keys[i];
		if (!obj.keys().contains(k)) {
			emit jsonErrorSignal(QString("key %1 not contained").arg(k));
			break;
		}
		obj = obj.value(k).toObject();
	}
	return obj.value(keys.last()).toString();
}
PreLoader::PreLoader(const QString& type)
{
	this->type_ = type;
	preSetting_ = new QJsonObject();
	const QString fileName = QString(":/ui_prefile/asserts/json/%1.json").arg(this->type_);
	QFile jsonFile(fileName);
	if (!jsonFile.open(QIODevice::ReadOnly))
		emit fileMissingSignal(fileName);
	else {
		QByteArray jsonData = jsonFile.readAll();
		jsonFile.close();
		QJsonParseError jsonError;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData, &jsonError));
		if (jsonError.error == QJsonParseError::NoError)
			*preSetting_ = jsonDoc.object();
		else 
			emit jsonErrorSignal(jsonError.errorString());
	}
}

PreLoader* PreLoader::getInstance(const QString& type)
{
	if (instance_ == nullptr) 
		instance_ = new PreLoader(type_);
	else if (type != type_) {
		delete instance_;
		instance_ = new PreLoader(type_);
	}
	return instance_;
}

QString PreLoader::type_ = "zh_CN";
PreLoader* PreLoader::instance_ = nullptr;