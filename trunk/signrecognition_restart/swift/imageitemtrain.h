#include "imageitem.h"

class ImageItemTrain : public ImageItem
{
public:
	ImageItemTrain(QString &path);
	~ImageItemTrain();

	void train(cv::DescriptorMatcher &matcher);

	int getTrainIndex();

private:
	int mTrainIndex;
};