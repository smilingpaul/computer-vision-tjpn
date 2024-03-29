#include "imageitem.h"

class ImageItemExplore : public ImageItem
{
public:
	ImageItemExplore(QString &path);
	~ImageItemExplore();

	void match(cv::DescriptorMatcher &matcher);
	void knnmatch(cv::DescriptorMatcher &matcher);

	void evaluate();

	std::vector<cv::DMatch> getMatches();
	std::vector<cv::DMatch> getMatchesByIndex(int index);
	std::vector<cv::DMatch> getGoodMatchesByIndex(int index);
	std::vector<cv::Point2f> getFinalPointsByIndex(int index);

private:
	std::vector<cv::DMatch> mMatches;
	std::vector<std::vector<cv::DMatch>> mMatchesByIndex; ///< one vector for each train image
	std::vector<std::vector<cv::DMatch>> mGoodMatchesByIndex; ///< one vector for each train image
	std::vector<std::vector<cv::Point2f>> mFinalPoints;
};