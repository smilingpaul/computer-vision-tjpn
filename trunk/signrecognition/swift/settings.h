/// \file settings.h
///
///
///
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>
#include <opencv2/features2d/features2d.hpp>

class Settings
{
public:

	~Settings();
	static void loadSettings(const QString &inputpath = QString(), QByteArray *geometry = nullptr);
	static void saveSettings(const QString &inputpath = QString(), const QByteArray &geometry = QByteArray());

	static void setParameter(const QString &key, const QVariant &data) {if(mParameters.contains(key)) mParameters[key] = data;}
	static void resetParameter(const QString &key) {if(mDefaultParameters.contains(key)) mParameters.insert(key, mDefaultParameters.value(key));}
	static QVariant getParameter(const QString &key) {return mParameters.value(key);}

private:
	Settings(){} ///< not instanceable, prevent construction
	Settings(const Settings&){} ///< not copyable, prevent copy construction
	Settings& operator=(const Settings&){}  ///< not assignable, protects the unique instance

	static QMap<QString, QVariant> mDefaultParameters;
	static QMap<QString, QVariant> mParameters;

	//static QMap<QString, QString> parametersType_;

	static Settings *instance;
};

#endif // SETTINGS_H