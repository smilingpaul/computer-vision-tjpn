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
	//#warning hardcode
	QStringList trainPaths;
	trainPaths << "D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\signs\\black_30.png"
		<< "D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\signs\\black_60.png"
		<< "D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\signs\\black_120.png"
		<< "D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\signs\\black_danger.png"
		<< "D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\signs\\black_stop.png"
		<< "D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\signs\\black_yield.png";

	for (unsigned int i = 0; i < trainPaths.size(); i++)
	{
		mList.append(SwiftItem(trainPaths[i],mDetector,mExtractor,mMatcher));
		mList[i].detectFeatures();
		mList[i].extractDescriptors();
	}

	trainFiles();

	//////////////////////////////////////////////////////////////////////////

	for (unsigned int j = 0; j < newImagePaths.size(); j++)
	{
		mList.append(SwiftItem(newImagePaths[j],mDetector,mExtractor,mMatcher));
		mList[j].detectFeatures();
		mList[j].extractDescriptors();
	}

	queryFiles();
	for (unsigned int i = 6; i < mList.size()-6; i++)
	{
		//#warning bug - safe vector of cvmat descriptors in items or try to merge all train item descriptors
		mList[i].queryDB(mList[i].descriptors());
	}

	SwiftItem train = SwiftItem();
	SwiftItem explore = SwiftItem();
	cv::Mat out;
	std::vector<char> mask;
	unsigned int traini;
	unsigned int explorei;

	for (traini = 0; traini < mList.size()-(mList.size()-6); traini++)
	{
		for (explorei = 6; explorei < mList.size(); explorei++)
		{
			train = mList[traini];
			explore = mList[explorei];
			
			// mask by index
			mask.resize(explore.matches().size());
			std::fill(mask.begin(), mask.end(), 0);
			for(size_t i = 0; i < explore.matches().size(); i++)
			{
				if(explore.matches()[i].imgIdx == traini) {mask[i] = 1;}
			}

			cv::drawMatches(explore.cvmat()
				,explore.keypoints()
				,train.cvmat()
				,train.keypoints()
				,explore.matches()
				,out
				,cv::Scalar::all(-1)
				,cv::Scalar::all(-1)
				,mask);

			cv::imshow("Image"+traini,out);
		}
	}
}

void SwiftModel::trainFiles()
{
	std::vector<cv::Mat> train;
	for (unsigned int k = 0; k < mList.size(); k++)
	{
		//if (mList[k].train())
		train.push_back(mList[k].descriptors());
	}
	mMatcher.add(train);
	mMatcher.train();
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