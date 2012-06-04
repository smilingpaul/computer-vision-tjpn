/// \file swiftitem.h
///
///
///
#ifndef SWIFTITEM_H
#define SWIFTITEM_H


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp> //resize & interpolation methods
#include <opencv2/highgui/highgui.hpp> //debug
//#include <opencv2/features2d/features2d.hpp>
//#include "opencv2\core\types_c.h"
#include "sandboxsift.h"
#include "help.h"

class SwiftItem
{
public:
	SwiftItem(const QString &path);
	~SwiftItem();

	// roles
	QString path() const;
	QImage thumbnail() const;
	QImage image() const;
	// end of roles

	cv::Mat descriptors() const;
	std::vector<cv::KeyPoint> keypoints() const;

private:
	QString mPath;
	QImage mThumbnail;
	cv::Mat mImage;

	cv::Mat mDescriptors;
	std::vector<cv::KeyPoint> mKeypoints;

	void generateThumbnail();
	void detectFeatures();
	void extractDescriptors();
};

#endif // MODEL_H