#include "StdAfx.h"
#include "swiftitem.h"


static const int THUMBNAIL_WIDTH = 128; ///< default nthumbnail width

/// \class SwiftItem
/// \brief
///
///
///
SwiftItem::SwiftItem(const QString &path)
	: mPath(path)
{
	std::string stdpath = path.toStdString();

	mImage = cv::imread(stdpath);

	generateThumbnail();
	///  http://www.informit.com/articles/article.aspx?p=1405557&seqNum=2

	///
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

void SwiftItem::detectFeatures()
{
}

void SwiftItem::extractDescriptors()
{
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