#include "../header/preloader.h"
const QString PreLoader::querry(const QString& key) const
{
	QStringList keys = key.split("->");
	QJsonObject obj = *preSetting_;
	for (int i = 0; i < keys.length() - 1;++i)
	{
		QString k = keys[i];
		if (!obj.keys().contains(k)) 
		{
			emit jsonErrorSignal(QString("key %1 not contained").arg(k));
			break;
		}
		obj = obj.value(k).toObject();
	}
	return obj.value(keys.last()).toString();
}
PreLoader::PreLoader()
{
	getLanguage();
	preSetting_ = new QJsonObject();
	const QString fileName = QString(":/ui_prefile/asserts/json/%1.json").arg(this->type_);
	QFile jsonFile(fileName);
	if (!jsonFile.open(QIODevice::ReadOnly))
		emit fileMissingSignal(fileName);
	else 
	{
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
void PreLoader::getLanguage()
{
	QFile file("./asserts/json/setting.json");
	if (!file.open(QIODevice::ReadOnly))
		type_ = "zh_CN";
	else
	{
		QByteArray data = file.readAll();
		file.close();
		QJsonParseError error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &error));
		if (error.error != QJsonParseError::NoError)
			type_ = "zh_CN";
		else
			type_ = jsonDoc.object().value("language").toString();
	}
}
void PreLoader::setLanguage()
{
	QFile file("./asserts/json/setting.json");
	if (!file.open(QIODevice::ReadWrite))
		return;
	else
	{
		QByteArray data = file.readAll();
		QJsonParseError error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &error));
		if (error.error != QJsonParseError::NoError)
			return;
		else 
		{
			QJsonObject obj = jsonDoc.object();
			obj.insert(u"language", type_);
			file.close();
			file.open(QIODevice::WriteOnly);
			file.write(QJsonDocument(obj).toJson());
		}
		file.close();
	}
}
PreLoader* PreLoader::getInstance(const QString& type)
{
	if (type != type_) 
	{
		type_ = type;
		setLanguage();
		delete instance_;
		instance_ = new PreLoader();
	}
	else if (instance_ == nullptr) 
		instance_ = new PreLoader();
	return instance_;
}

QString PreLoader::type_ = "zh_CN";
PreLoader* PreLoader::instance_ = nullptr;