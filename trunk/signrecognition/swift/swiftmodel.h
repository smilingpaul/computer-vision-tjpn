/// \file swiftmodel.h
///
///
///
#ifndef SWIFTMODEL_H
#define SWIFTMODEL_H

#include <QtGui/QMainWindow>
#include <QAbstractListModel>
#include "swiftitem.h"

class SwiftModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum SwiftItemRoles {
		PathRole = Qt::UserRole + 1,
		ThumbnailRole,
		ImageRole,
		DescriptorsRole,
		KeypointsRole
	};

	SwiftModel(QObject *parent = 0);
	~SwiftModel();

	// needs to be implemented
	// return row count for given parent
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	// return data, based on current index and requested role
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	void loadFiles(QStringList newImagePaths);

	// bonus stuff
	//void addImageItem(const SwiftItem &swiftItem);
private:
	QList<SwiftItem> mList;
};

#endif // SWIFTMODEL_H