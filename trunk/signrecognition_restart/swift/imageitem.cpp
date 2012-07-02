#include "StdAfx.h"
#include "imageitem.h"

ImageItem::ImageItem(QString &path)
	: mPath(path)
{
	//imread flags
	//>0 the loaded image is forced to be a 3-channel color image
	//=0 the loaded image is forced to be grayscale
	//<0 the loaded image will be loaded as-is (note that in the current implementation the alpha channel, if any, is stripped from the output image, e.g. 4-channel RGBA image will be loaded as RGB if flags >= 0 ).
	mMat = cv::imread(path.toStdString(), 1);
}

ImageItem::~ImageItem()
{

}

void ImageItem::precalculation(cv::FeatureDetector &detector, cv::DescriptorExtractor &extractor)
{
	calculateKeypoints(detector);
	calculateDescriptors(extractor);
}

void ImageItem::calculateKeypoints(cv::FeatureDetector &detector)
{
	detector.detect(mMat,mKeypoints);
}

void ImageItem::calculateDescriptors(cv::DescriptorExtractor &extractor)
{
	extractor.compute(mMat,mKeypoints,mDescriptors);
}

void ImageItem::match(cv::DescriptorMatcher &matcher, QList<ImageItem> &list)
{

}

void ImageItem::match(cv::DescriptorMatcher &matcher, ImageItem &item)
{

}

void ImageItem::train(cv::DescriptorMatcher &matcher)
{
	std::vector<cv::Mat> train;
	train.push_back(mDescriptors);

	matcher.add(train);
	matcher.train();
}

//////////////////////////////////////////////////////////////////////////

cv::Mat ImageItem::getImage() { return mMat; }
cv::Mat ImageItem::getDescriptors() { return mDescriptors; }
std::vector<cv::KeyPoint> ImageItem::getKeyPoints() { return mKeypoints; }