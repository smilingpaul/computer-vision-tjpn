#include "StdAfx.h"
#include "imageitemexplore.h"

ImageItemExplore::ImageItemExplore(QString &path) : ImageItem(path)
{
}

ImageItemExplore::~ImageItemExplore()
{
}

void ImageItemExplore::match(cv::DescriptorMatcher &matcher)
{
	matcher.match(mDescriptors, mMatches);

	mMatchesByIndex.clear();

	for (unsigned int i = 0; i < matcher.getTrainDescriptors().size(); i++)
		mMatchesByIndex.push_back(std::vector<cv::DMatch>());

	for (unsigned int i = 0; i < mMatches.size(); i++)
	{
		mMatchesByIndex[mMatches[i].imgIdx].push_back(mMatches[i]);
	}

	//matcher.knnMatch(mDescriptors,mMatchesByIndex,4);
	//matcher.radiusMatch(mDescriptors,mMatchesByIndex,200);
}

//////////////////////////////////////////////////////////////////////////

std::vector<cv::DMatch> ImageItemExplore::getMatches() { return mMatches; }
std::vector<cv::DMatch> ImageItemExplore::getMatchesByIndex(int index) { return mMatchesByIndex[index]; }