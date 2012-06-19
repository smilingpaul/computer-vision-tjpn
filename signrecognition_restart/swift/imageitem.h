#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp> //resize & interpolation methods
#include <opencv2/highgui/highgui.hpp> //debug
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>


class ImageItem
{
public:
	ImageItem(QString &path);
	~ImageItem();

	void precalculation(cv::FeatureDetector &detector, cv::DescriptorExtractor &extractor);
	void match(cv::GenericDescriptorMatcher &matcher, ImageItem &item);


private:
	QString mPath;
	cv::Mat mMat;
	cv::vector<cv::KeyPoint> mKeypoints;
	cv::Mat mDescriptors;

	std::vector<std::vector<cv::DMatch>> mMatches; ///< one vector for each train image
	std::vector<int> mMatchesIndex;

	void calculateKeypoints(cv::FeatureDetector &detector);
	void calculateDescriptors(cv::DescriptorExtractor &extractor);
};

#endif // IMAGEITEM_H
