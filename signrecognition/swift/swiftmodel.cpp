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
	mSift = cv::SIFT();
	mMatcher = cv::FlannBasedMatcher(new cv::flann::CompositeIndexParams(), new cv::flann::SearchParams());
	mDetector = cv::SiftFeatureDetector(mSift);
	mExtractor = cv::SiftDescriptorExtractor(mSift);

	QHash<int,QByteArray> roles;
	roles[PathRole] = "path";
	roles[ThumbnailRole] = "thumbnail";
	roles[TrainRole] = "train";
	setRoleNames(roles);

	//roles[ImageRole] = "image";
	//roles[DescriptorsRole] = "descriptors";
	//roles[KeypointsRole] = "keypoints";
}

SwiftModel::~SwiftModel()
{
}

//////////////////////////////////////////////////////////////////////////

int SwiftModel::rowCount(const QModelIndex &parent) const
{
	return mList.size();
}

QVariant SwiftModel::data(const QModelIndex &index, int role) const
{
	//if (!index.isValid())
	//	return QVariant();

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
	case TrainRole:
		return swiftItem.train();
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
		mList.append(SwiftItem(newImagePaths[i],mDetector,mExtractor,mMatcher));
		mList[i].detectFeatures();
		mList[i].extractDescriptors();
	}
}

//void SwiftItem::trainDB()
//{
//	std::vector<cv::Mat> train;
//	train.push_back(mDescriptors);
//	mMatcher.add(train);
//	//mMatcher.train();
//}

void SwiftModel::trainFiles()
{
	std::vector<cv::Mat> train;
	for (unsigned int i = 0; mList.size(); i++)
	{
		if (mList[i].train())
			train.push_back(mList[i].descriptors());
	}
	mMatcher.add(train);
	mMatcher.train();
}

//////////////////////////////////////////////////////////////////////////

//QModelIndex SwiftModel::parent(const QModelIndex &child) const
//{
//	return child;
//}

//int SwiftModel::columnCount(const QModelIndex &parent) const
//{
//	return 0;
//}

//bool SwiftModel::hasChildren(const QModelIndex &parent) const
//{
//	return true;
//}