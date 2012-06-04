#include "StdAfx.h"
#include "swiftitem.h"

static const int THUMBNAIL_WIDTH = 128; ///< default thumbnail width

/// \class SwiftItem
/// \brief
///
///
///
SwiftItem::SwiftItem(const QString &path
	, cv::SiftFeatureDetector &detector
	, cv::SiftDescriptorExtractor &extractor
	, cv::FlannBasedMatcher &matcher)
	: mPath(path)
	, mDetector(detector)
	, mExtractor(extractor)
	, mMatcher(matcher)
{
	std::string stdpath = path.toStdString();
	mImage = cv::imread(stdpath);

	generateThumbnail();

	// #warning
	cv::namedWindow("DEBUG - "+stdpath);
	cv::imshow("DEBUG - "+stdpath,mImage);
}

SwiftItem::~SwiftItem()
{
}

//////////////////////////////////////////////////////////////////////////

void SwiftItem::generateThumbnail()
{
	double rows = ((double)mImage.rows / (double)mImage.cols) * THUMBNAIL_WIDTH;

	cv::Mat result(cvRound(rows),THUMBNAIL_WIDTH,CV_8UC4);

	// explicitly specify dsize=dst.size(); fx and fy will be computed from that.
	cv::resize(mImage, result, result.size(), 0, 0, cv::INTER_LINEAR);

	mThumbnail = Help::Convert::cvmat2qimage(result);
}

//void SwiftItem::trainDB()
//{
//	std::vector<cv::Mat> train;
//	train.push_back(mDescriptors);
//	mMatcher.add(train);
//	//mMatcher.train();
//}

void SwiftItem::queryDB()
{
	mMatcher.match(mDescriptors,matches);
}

void SwiftItem::detectFeatures()
{
	mDetector.detect(mImage,mKeypoints);
}

void SwiftItem::extractDescriptors()
{
	mExtractor.compute(mImage,mKeypoints,mDescriptors);
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

QImage SwiftItem::image() const
{
	return Help::Convert::cvmat2qimage(mImage);
}

//////////////////////////////////////////////////////////////////////////

cv::Mat SwiftItem::descriptors() const
{
	return mDescriptors;
}

std::vector<cv::KeyPoint> SwiftItem::keypoints() const
{
	return mKeypoints;
}