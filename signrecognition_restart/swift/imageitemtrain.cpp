#include "StdAfx.h"
#include "imageitemtrain.h"

ImageItemTrain::ImageItemTrain(QString &path) : ImageItem(path)
{
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