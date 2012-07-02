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
	mGoodMatchesByIndex.clear();

	for (unsigned int i = 0; i < matcher.getTrainDescriptors().size(); i++)
	{
		mMatchesByIndex.push_back(std::vector<cv::DMatch>());
		mGoodMatchesByIndex.push_back(std::vector<cv::DMatch>());
	}

	for (unsigned int i = 0; i < mMatches.size(); i++)
	{
		mMatchesByIndex[mMatches[i].imgIdx].push_back(mMatches[i]);
	}

	matcher.knnMatch(mDescriptors,mMatchesByIndex,1);
	//matcher.radiusMatch(mDescriptors,mMatchesByIndex,200);

	// iterate through the vector collection
	for (unsigned int i = 0; i < mMatchesByIndex.size(); i++)
	{
		// iterate through one vector
		for (unsigned int j = 0; j < mMatchesByIndex[i].size(); j++)
		{
			if (mMatchesByIndex[i][j].distance < 200)
				mGoodMatchesByIndex[i].push_back(mMatchesByIndex[i][j]);
		}
	}


}

//////////////////////////////////////////////////////////////////////////

std::vector<cv::DMatch> ImageItemExplore::getMatches() { return mMatches; }
std::vector<cv::DMatch> ImageItemExplore::getMatchesByIndex(int index) { return mMatchesByIndex[index]; }
std::vector<cv::DMatch> ImageItemExplore::getGoodMatchesByIndex(int index) { return mGoodMatchesByIndex[index]; }