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
	virtual ~ImageItem();

	void precalculation(cv::FeatureDetector &detector, cv::DescriptorExtractor &extractor);

	cv::Mat getImage();
	cv::Mat getDescriptors();
	std::vector<cv::KeyPoint> getKeyPoints();

protected:
	QString mPath;
	cv::Mat mMat;
	std::vector<cv::KeyPoint> mKeyPoints;
	cv::Mat mDescriptors;

private:
	void calculateKeypoints(cv::FeatureDetector &detector);
	void calculateDescriptors(cv::DescriptorExtractor &extractor);
};

#endif // IMAGEITEM_H