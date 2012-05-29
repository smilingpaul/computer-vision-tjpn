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
	roles[ThumbnailRole] = "thumbnail";
	roles[ImageRole] = "image";
	//roles[DescriptorsRole] = "descriptors";
	//roles[KeypointsRole] = "keypoints";
	setRoleNames(roles);
}

SwiftModel::~SwiftModel()
{
}

//void SwiftModel::addImageItem(const SwiftItem &swiftItem)
//{
//	beginInsertRows(QModelIndex(),rowCount(),rowCount());
//	m_list << swiftItem;
//	endInsertRows();
//}

int SwiftModel::rowCount(const QModelIndex &parent) const
{
	return mList.count();
}

QVariant SwiftModel::data(const QModelIndex &index, int role) const
{
	//if (!index.isValid())
	//	return QVariant();

	if (index.row() < 0 || index.row() > mList.count())
		return QVariant();

	const SwiftItem &swiftItem = mList[index.row()];

	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case PathRole:
			return swiftItem.path();
		case ThumbnailRole:
			return swiftItem.thumbnail();
		case ImageRole:
			return swiftItem.image();
		//case DescriptorsRole:
		//	return swiftItem.descriptors();
		//case KeypointsRole:
		//	return swiftItem.keypoints();
		}
	//case Qt::DecorationRole:
	//case Qt::EditRole:
	//case Qt::ToolTipRole:
	//case Qt::StatusTipRole:
	//case Qt::WhatsThisRole:
	//case Qt::SizeHintRole:
	//...
	}

	//if (role == PathRole)
	//	return swiftItem.path();
	//else if (role == SizeRole)
	//	return image.size();

	return QVariant();
}