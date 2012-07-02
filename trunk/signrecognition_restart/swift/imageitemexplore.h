#include "imageitem.h"

class ImageItemExplore : public ImageItem
{
public:
	ImageItemExplore(QString &path);
	~ImageItemExplore();

	void match(cv::DescriptorMatcher &matcher);

	std::vector<cv::DMatch> getMatches();
	std::vector<cv::DMatch> getMatchesByIndex(int index);

private:
	std::vector<cv::DMatch> mMatches;
	std::vector<std::vector<cv::DMatch>> mMatchesByIndex; ///< one vector for each train image
};