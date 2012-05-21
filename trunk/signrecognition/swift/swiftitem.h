/// \file swiftitem.h
///
/// 
///
#ifndef SWIFTITEM_H
#define SWIFTITEM_H

class SwiftItem
{
public:
	SwiftItem(const QString &path);

	QString path() const;

private:
	QString m_path;
};

#endif // MODEL_H
