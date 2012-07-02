#include "imageitem.h"

class ImageItemTrain : public ImageItem
{
public:
	ImageItemTrain(QString &path);
	~ImageItemTrain();

	void train(cv::DescriptorMatcher &matcher);
	void trainBOW(cv::BOWKMeansTrainer &trainer);

	int getTrainIndex();

private:
	int mTrainIndex;

};