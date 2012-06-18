#include "StdAfx.h"
#include "settings.h"

QMap<QString, QVariant> Settings::mDefaultParameters;
QMap<QString, QVariant> Settings::mParameters;

/// \class SwiftModel
/// \brief
///
///
///
void Settings::loadSettings(const QString &inputpath, QByteArray *geometry)
{
	QString path = inputpath;

	if (inputpath.isEmpty())
	{
		path = QString("%1/%2").arg(QDir::currentPath(),"config.ini");
	}

	QSettings file(path, QSettings::IniFormat);

	QVariant data;
	for(QMap<QString, QVariant>::const_iterator it = mDefaultParameters.begin(); it != mDefaultParameters.end(); ++it)
	{
		const QString &key = it.key();
		data = file.value(key);
		if(data.isValid())
		{
			setParameter(key, data);
		}
	}

	if(geometry)
	{
		data = file.value("geometry");
		if(data.isValid())
		{
			*geometry = data.toByteArray();
		}
	}

	printf("Settings loaded from %s\n", inputpath.toStdString().c_str());
}

void Settings::saveSettings(const QString &inputpath, const QByteArray &geometry)
{
	QString path = inputpath;

	if (inputpath.isEmpty())
	{
		path = QString("%1/%2").arg(QDir::currentPath(),"config.ini");
	}

	QSettings file(path, QSettings::IniFormat);

	for(QMap<QString, QVariant>::const_iterator it = mParameters.begin(); it != mParameters.end(); ++it)
	{
		file.setValue(it.key(), it.value());
	}

	if(!geometry.isEmpty())
	{
		file.setValue("geometry", geometry);
	}

	printf("Settings saved to %s\n", inputpath.toStdString().c_str());
}