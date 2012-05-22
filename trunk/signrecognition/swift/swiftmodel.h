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
		PathRole = Qt::UserRole + 1
	};

	SwiftModel(QObject *parent = 0);
	~SwiftModel();

	void addImageItem(const SwiftItem &swiftItem);

	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
	QList<SwiftItem> m_list;
	
};

#endif // SWIFTMODEL_H
