/// \file swiftitem.h
///
///
///
#ifndef SWIFTITEM_H
#define SWIFTITEM_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp> //resize & interpolation methods
#include <opencv2/highgui/highgui.hpp> //debug
#include <opencv2/nonfree/nonfree.hpp>
#include "sandboxsift.h"
#include "help.h"

class SwiftItem
{
public:
	SwiftItem(const QString &path
		, cv::SiftFeatureDetector &detector
		, cv::SiftDescriptorExtractor &extractor
		, cv::FlannBasedMatcher &matcher);
	~SwiftItem();

	// roles
	QString path() const;
	QImage thumbnail() const;
	QImage image() const;
	// end of roles

	cv::Mat descriptors() const;
	std::vector<cv::KeyPoint> keypoints() const;

	void detectFeatures();
	void extractDescriptors();
	//void trainDB();
	void queryDB();

private:
	QString mPath;
	QImage mThumbnail;
	cv::Mat mImage;

	cv::SIFT mSift;
	cv::SiftFeatureDetector mDetector;
	cv::SiftDescriptorExtractor mExtractor;
	cv::FlannBasedMatcher mMatcher;

	cv::vector<cv::KeyPoint> mKeypoints;
	cv::Mat mDescriptors;
	cv::vector<cv::DMatch> matches; ///< vector of descriptor indices and the corresponding db image indices

	void generateThumbnail();
};

#endif // MODEL_H