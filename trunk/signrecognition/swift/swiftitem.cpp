#include "StdAfx.h"
#include "swiftitem.h"

/// \class SwiftItem
/// \brief
///
///
///
SwiftItem::SwiftItem(const QString &path)
	: mPath(path)
{
	mImage = cv::imread(path.toStdString());

	generateThumbnail();

	// #warning
	cv::namedWindow("DEBUG - Input Image");
	cv::imshow("DEBUG - Input Image",mImage);
}

SwiftItem::~SwiftItem()
{
}

void SwiftItem::generateThumbnail()
{
	cv::Mat result;

	//cv::Mat dest;
	//cv::cvtColor(mThumbnail, dest, CV_BGR2RGB);
	//return QImage((const unsigned char*)(dest.data), dest.cols, dest.rows, QImage::Format_RGB888);
}

void SwiftItem::detectFeatures()
{

}

void SwiftItem::extractDescriptors()
{

}


/// properties
QString SwiftItem::path() const
{
	return mPath;
}

QImage SwiftItem::thumbnail() const
{
	return mThumbnail;
}

cv::Mat SwiftItem::image() const
{
	return mImage;
}

cv::Mat SwiftItem::descriptors() const
{
	return mDescriptors;
}

std::vector<cv::KeyPoint> SwiftItem::keypoints() const
{
	return mKeypoints;
}