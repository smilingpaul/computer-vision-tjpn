/// \file swiftmodel.h
///
///
///
#ifndef SWIFTMODEL_H
#define SWIFTMODEL_H

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
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	// bonus stuff
	//void addImageItem(const SwiftItem &swiftItem);
private:
	QList<SwiftItem> m_list;
};

#endif // SWIFTMODEL_H