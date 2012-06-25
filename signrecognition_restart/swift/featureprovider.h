#ifndef FEATUREPROVIDER_H
#define FEATUREPROVIDER_H

#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

class FeatureProvider
{
public:
	FeatureProvider();
	~FeatureProvider();

	QStringList mDetectorList;
	QStringList mExtractorList;

	cv::FeatureDetector* provideDetector();
	cv::DescriptorExtractor* provideExtractor();

	//// BONUS
	//cv::DescriptorMatcher* provideMatcher();

	void setDefault();
	void setCurrent(int detector = -1, int extractor = -1);
	void setCurrent(QString detector = QString(), QString extractor = QString());

	template<typename T> void insert(QString key, T input);
	template<typename T> T value(QString key);

private:
	void initialize();

	//bool mConfigurationChanged;
	QMap<QString, QVariant> mConfiguration;
	QMap<QString, QVariant> mConfigurationDefault;

	QString mCurrentDetector;
	QString mCurrentExtractor;
	QString mCurrentPrefix;
};

#endif // FEATUREPROVIDER_H