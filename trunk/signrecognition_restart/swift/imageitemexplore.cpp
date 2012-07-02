#include "StdAfx.h"
#include "imageitemexplore.h"

ImageItemExplore::ImageItemExplore(QString &path) : ImageItem(path)
{
}

ImageItemExplore::~ImageItemExplore()
{
}

void ImageItemExplore::knnmatch(cv::DescriptorMatcher &matcher)
{
	mMatchesByIndex.clear();
	mGoodMatchesByIndex.clear();

	for (unsigned int i = 0; i < matcher.getTrainDescriptors().size(); i++)
	{
		mMatchesByIndex.push_back(std::vector<cv::DMatch>());
		mGoodMatchesByIndex.push_back(std::vector<cv::DMatch>());
	}

	//////////////////////////////////////////////////////////////////////////

	std::vector<std::vector<cv::DMatch>> tmp;
	std::vector<cv::DMatch> tmpGood;

	matcher.knnMatch(mDescriptors,tmp,2);

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i][0].distance < 0.6 * tmp[i][1].distance)
			tmpGood.push_back(tmp[i][0]);
	}

	for (unsigned int i = 0; i < tmpGood.size(); i++)
	{
		mGoodMatchesByIndex[tmpGood[i].imgIdx].push_back(tmpGood[i]);
	}
}

void ImageItemExplore::match(cv::DescriptorMatcher &matcher)
{
	mMatchesByIndex.clear();
	mGoodMatchesByIndex.clear();

	for (unsigned int i = 0; i < matcher.getTrainDescriptors().size(); i++)
	{
		mMatchesByIndex.push_back(std::vector<cv::DMatch>());
		mGoodMatchesByIndex.push_back(std::vector<cv::DMatch>());
	}

	//////////////////////////////////////////////////////////////////////////

	matcher.match(mDescriptors, mMatches);

	for (unsigned int i = 0; i < mMatches.size(); i++)
	{
		mMatchesByIndex[mMatches[i].imgIdx].push_back(mMatches[i]);
	}

	// iterate through the vector collection
	for (unsigned int i = 0; i < mMatchesByIndex.size(); i++)
	{
		// iterate through one vector
		for (unsigned int j = 0; j < mMatchesByIndex[i].size(); j++)
		{
			// simulates a radius match
			if (mMatchesByIndex[i][j].distance < 200)
				mGoodMatchesByIndex[i].push_back(mMatchesByIndex[i][j]);
		}
	}


}

//////////////////////////////////////////////////////////////////////////

std::vector<cv::DMatch> ImageItemExplore::getMatches() { return mMatches; }
std::vector<cv::DMatch> ImageItemExplore::getMatchesByIndex(int index) { return mMatchesByIndex[index]; }
std::vector<cv::DMatch> ImageItemExplore::getGoodMatchesByIndex(int index) { return mGoodMatchesByIndex[index]; }