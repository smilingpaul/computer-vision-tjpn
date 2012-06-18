#include "StdAfx.h"
#include "swiftitem.h"

static const int THUMBNAIL_WIDTH = 128; ///< default thumbnail width

/// \class SwiftItem
/// \brief
///
///
///
SwiftItem::SwiftItem()
{}

SwiftItem::SwiftItem(const QString &path)
	: mPath(path)
	, mTrain(false)
{
	std::string stdpath = path.toStdString();
	mImage = cv::imread(stdpath);

	generateThumbnail();

	//#warning
	//cv::namedWindow("DEBUG - "+stdpath);
	//cv::imshow("DEBUG - "+stdpath,mImage);
}

SwiftItem::~SwiftItem()
{
}

//////////////////////////////////////////////////////////////////////////
void SwiftItem::generateThumbnail()
{
	int rows = cvRound((double)mImage.rows / (double)mImage.cols) * THUMBNAIL_WIDTH;

	cv::Mat result(rows,THUMBNAIL_WIDTH,CV_8UC4);

	// explicitly specify dsize=dst.size(); fx and fy will be computed from that.
	cv::resize(mImage, result, result.size(), 0, 0, cv::INTER_LINEAR);

	mThumbnail = Help::Convert::cvmat2qimage(result);
}

void SwiftItem::detectFeatures(cv::FeatureDetector& mDetector)
{
	mDetector.detect(mImage,mKeypoints);
}

void SwiftItem::extractDescriptors(cv::DescriptorExtractor& mExtractor)
{
	mExtractor.compute(mImage,mKeypoints,mDescriptors);
}

void SwiftItem::queryDB(cv::DescriptorMatcher& mMatcher,std::vector<cv::Mat>& masks)
{
	//mMatcher.match(mDescriptors,mMatches,masks);
}
//////////////////////////////////////////////////////////////////////////

/// properties
QString SwiftItem::path() const
{
	return mPath;
}

QImage SwiftItem::thumbnail() const
{
	return mThumbnail;
}

bool SwiftItem::train() const
{
	return mTrain;
}

QImage SwiftItem::qimage() const
{
	return Help::Convert::cvmat2qimage(mImage);
}

//////////////////////////////////////////////////////////////////////////
/// getters
cv::Mat SwiftItem::getDescriptors() const {return mDescriptors;}
std::vector<cv::KeyPoint> SwiftItem::getKeypoints() const {return mKeypoints;}
//std::vector<cv::DMatch> SwiftItem::getMatches() const {return mMatches;}
/// setters
void SwiftItem::setDescriptors(cv::Mat &descriptors){mDescriptors=descriptors;}
void SwiftItem::setKeypoints(std::vector<cv::KeyPoint> &keypoints){mKeypoints=keypoints;}
//void SwiftItem::setMatches(std::vector<cv::DMatch> &matches){mMatches=matches;}
//////////////////////////////////////////////////////////////////////////
cv::Mat SwiftItem::cvmat() const
{
	return mImage;
}