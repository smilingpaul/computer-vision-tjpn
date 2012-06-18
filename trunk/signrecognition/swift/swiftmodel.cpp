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
	//mSift = cv::SIFT();
	//mMatcher = cv::FlannBasedMatcher(new cv::flann::CompositeIndexParams(), new cv::flann::SearchParams());
	//mDetector = cv::SiftFeatureDetector(mSift);
	//mExtractor = cv::SiftDescriptorExtractor(mSift);

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
	////#warning hardcode
	////////////////////////////////////////////////////////////////////////////

	///// training the images to search!
	//for (unsigned int j = 0; j < newImagePaths.size(); j++)
	//{
	//	mList.append(SwiftItem(newImagePaths[j]));
	//	mList[j].detectFeatures();
	//	mList[j].extractDescriptors();
	//}

	//trainFiles();

	///// querying for signs
	//QStringList queryPaths;
	//queryPaths << "..\\swift-build\\TestData\\signs\\black_30.png"
	//	<< "..\\swift-build\\TestData\\signs\\black_60.png"
	//	<< "..\\swift-build\\TestData\\signs\\black_120.png"
	//	<< "..\\swift-build\\TestData\\signs\\black_danger.png"
	//	<< "..\\swift-build\\TestData\\signs\\black_stop.png"
	//	<< "..\\swift-build\\TestData\\signs\\black_yield.png";

	//for (unsigned int i = 0; i < queryPaths.size(); i++)
	//{
	//	mList.append(SwiftItem(queryPaths[i]));
	//	mList[i].detectFeatures();
	//	mList[i].extractDescriptors();
	//}

	//queryFiles();

	//for (unsigned int i = newImagePaths.size(); i < mList.size(); i++)
	//{
	//	//#warning bug - safe vector of cvmat descriptors in items or try to merge all train item descriptors
	//	mList[i].queryDB(mList[i].descriptors());
	//}

	//SwiftItem searchWith = SwiftItem();
	//SwiftItem beeingSearched = SwiftItem();
	//cv::Mat out;
	//std::vector<char> mask;
	//unsigned int searchWithI;
	//unsigned int beeingSearchedI;

	//for (beeingSearchedI = 0; beeingSearchedI < newImagePaths.size(); beeingSearchedI++)
	//{
	//	for (searchWithI = newImagePaths.size(); searchWithI < mList.size(); searchWithI++)
	//	{
	//		searchWith = mList[searchWithI];
	//		beeingSearched = mList[beeingSearchedI];

	//		// mask by index
	//		mask.resize(beeingSearched.matches().size());
	//		std::fill(mask.begin(), mask.end(), 0);
	//		for(unsigned int i = 0; i < beeingSearched.matches().size(); i++)
	//		{
	//			if(beeingSearched.matches()[i].imgIdx == searchWithI) {mask[i] = 1;}
	//		}

	//		cv::drawMatches(searchWith.cvmat()
	//			,searchWith.keypoints()
	//			,beeingSearched.cvmat()
	//			,beeingSearched.keypoints()
	//			,beeingSearched.matches()
	//			,out
	//			,cv::Scalar::all(-1)
	//			,cv::Scalar::all(-1)
	//			,mask);

	//		cv::imshow("Image"+searchWithI,out);
	//	}
	//}
}

void SwiftModel::trainFiles()
{
	//std::vector<cv::Mat> train;
	//for (unsigned int k = 0; k < mList.size(); k++)
	//{
	//	//if (mList[k].train())
	//	train.push_back(mList[k].descriptors());
	//}
	//mMatcher.add(train);
	//mMatcher.train();
}

void SwiftModel::queryFiles()
{
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