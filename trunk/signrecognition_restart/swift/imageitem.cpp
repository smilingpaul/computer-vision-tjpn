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

void ImageItem::computeBOW(cv::BOWImgDescriptorExtractor &extractor)
{
	cv::Mat tmp = extractor.getVocabulary();
	extractor.setVocabulary(mVocabulary);

	extractor.compute(
		mMat
		, mKeyPoints
		, mDescriptors
		, &mBOWPointIndicesOfClusters
		, &mBOWDescriptors
		);
}

void ImageItem::precalculation(cv::FeatureDetector &detector, cv::DescriptorExtractor &extractor)
{
	calculateKeypoints(detector);
	calculateDescriptors(extractor);
}

void ImageItem::calculateKeypoints(cv::FeatureDetector &detector)
{
	detector.detect(mMat,mKeyPoints);
}

void ImageItem::calculateDescriptors(cv::DescriptorExtractor &extractor)
{
	extractor.compute(mMat,mKeyPoints,mDescriptors);
}

//////////////////////////////////////////////////////////////////////////

cv::Mat ImageItem::getImage() { return mMat; }
cv::Mat ImageItem::getDescriptors() { return mDescriptors; }
std::vector<cv::KeyPoint> ImageItem::getKeyPoints() { return mKeyPoints; }