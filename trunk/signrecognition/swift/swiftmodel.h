/// \file swiftmodel.h
///
///
///
#ifndef SWIFTMODEL_H
#define SWIFTMODEL_H

#include <QtCore/QtCore>
#include <QtGui/QMainWindow>
#include <QAbstractListModel>
#include "swiftitem.h"

class SwiftModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum SwiftItemRoles {
		PathRole = Qt::UserRole + 1,
		ThumbnailRole
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
	//

private:
	QList<SwiftItem> mList;

	//QModelIndex parent(const QModelIndex &child) const;
	//int columnCount(const QModelIndex &parent) const;
	//bool hasChildren(const QModelIndex &parent) const;
};

#endif // SWIFTMODEL_H