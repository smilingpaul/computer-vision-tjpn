#include "StdAfx.h"
#include "swiftmodel.h"

/// \class SwiftModel
/// \brief 
///
/// 
///
SwiftModel::SwiftModel(QObject *parent)
	: QAbstractListModel(parent)
{
	QHash<int,QByteArray> roles;
	roles[PathRole] = "path";
	setRoleNames(roles);
}

SwiftModel::~SwiftModel()
{

}

void SwiftModel::addImageItem(const SwiftItem &imageItem)
{
	beginInsertRows(QModelIndex(),rowCount(),rowCount());
	m_list << imageItem;
	endInsertRows();
}

int SwiftModel::rowCount(const QModelIndex &parent) const {
	return m_list.count();
}

QVariant SwiftModel::data(const QModelIndex &index, int role) const {
	if (index.row() < 0 || index.row() > m_list.count())
		return QVariant();

	const SwiftItem &imageItem = m_list[index.row()];
	if (role == PathRole)
		return imageItem.path();
	//else if (role == SizeRole)
	//	return image.size();
	return QVariant();
}
