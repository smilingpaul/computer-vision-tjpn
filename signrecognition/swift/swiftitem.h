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
#include <opencv2/features2d/features2d.hpp>
#include "sandboxsift.h"
#include "help.h"

class SwiftItem
{
public:
	SwiftItem();
	SwiftItem(const QString &path);
	~SwiftItem();

	// roles
	QString path() const;
	QImage thumbnail() const;
	bool train() const;
	// end of roles

	QImage qimage() const;
	cv::Mat cvmat() const;

	cv::Mat descriptors() const;
	std::vector<cv::KeyPoint> keypoints() const;
	std::vector<cv::DMatch> matches() const;

	void detectFeatures(cv::FeatureDetector& mDetector);
	void extractDescriptors(cv::DescriptorExtractor& mExtractor);
	void queryDB(cv::DescriptorMatcher& mMatcher,std::vector<cv::Mat>& masks);

private:
	QString mPath;
	QImage mThumbnail;
	cv::Mat mImage;
	bool mTrain;

	//cv::SIFT mSift;
	//cv::SiftFeatureDetector mDetector;
	//cv::SiftDescriptorExtractor mExtractor;
	//cv::FlannBasedMatcher mMatcher;

	cv::vector<cv::KeyPoint> mKeypoints;
	cv::Mat mDescriptors;
	std::vector<cv::DMatch> mMatches; ///< vector of descriptor indices and the corresponding db image indices

	void generateThumbnail();
};

#endif // MODEL_H