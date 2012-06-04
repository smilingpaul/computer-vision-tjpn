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
	//roles[ImageRole] = "image";
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

//////////////////////////////////////////////////////////////////////////

int SwiftModel::rowCount(const QModelIndex &parent) const
{
	return mList.count();
}

QVariant SwiftModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() < 0 || index.row() > mList.size())
		return QVariant();

	const SwiftItem &swiftItem = mList[index.row()];

	switch (role)
	{
	case Qt::DisplayRole:
		return swiftItem.thumbnail();
	case PathRole:
		return swiftItem.path();
	case ThumbnailRole:
		return swiftItem.thumbnail();
	//case Qt::DecorationRole:
	//case Qt::EditRole:
	//case Qt::ToolTipRole:
	//case Qt::StatusTipRole:
	//case Qt::WhatsThisRole:
	//case Qt::SizeHintRole:
	//...
	}

	return QVariant();
}

void SwiftModel::loadFiles(QStringList newImagePaths)
{
	for (unsigned int i = 0; i < newImagePaths.size(); i++)
	{
		mList.append(SwiftItem(newImagePaths[i],mSift,mDetector,mExtractor,mMatcher));
	}

	for (unsigned int j = 0; j < mList.size(); j++)
	{
		mList[j].detectFeatures();
		mList[j].extractDescriptors();
		mList[j].trainDB();
	}


}

//////////////////////////////////////////////////////////////////////////

//QModelIndex SwiftModel::parent(const QModelIndex &child) const
//{
//	return child;
//}
//

//
//int SwiftModel::columnCount(const QModelIndex &parent) const
//{
//	return 0;
//}
//
//bool SwiftModel::hasChildren(const QModelIndex &parent) const
//{
//	return true;
//}