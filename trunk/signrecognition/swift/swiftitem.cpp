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

/// properties
QString SwiftItem::path() const
{
	return mPath;
}

QImage SwiftItem::thumbnail() const
{
	return mThumbnail;
}

/// static methods
/// http://permalink.gmane.org/gmane.comp.lib.opencv/37800
///
static QImage mat2qimage(const cv::Mat& mat)
{
	cv::Mat rgb;
	cv::cvtColor(mat, rgb, CV_BGR2RGB);
	return QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
}

static cv::Mat qimage2mat(const QImage& qimage)
{
	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
	int from_to[] = { 0,0,  1,1,  2,2 };
	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
	return mat2;
}