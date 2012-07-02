#include "StdAfx.h"
#include "imageitemtrain.h"

ImageItemTrain::ImageItemTrain(QString &path) : ImageItem(path)
{
	cv::Mat tmp1 = mMat;
	cv::Mat tmp2;
	cv::GaussianBlur(tmp1,tmp2,cv::Size(), (double)0.5, (double)0.5);
	resize(tmp2, mMat, cv::Size(), (double)2, (double)2, cv::INTER_LINEAR);
}

ImageItemTrain::~ImageItemTrain()
{
}

void ImageItemTrain::train(cv::DescriptorMatcher &matcher)
{
	std::vector<cv::Mat> train;
	train.push_back(mDescriptors);

	mTrainIndex = matcher.getTrainDescriptors().size();

	matcher.add(train);
	matcher.train();
}

//////////////////////////////////////////////////////////////////////////

int ImageItemTrain::getTrainIndex() { return mTrainIndex; }