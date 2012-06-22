#ifndef FEATUREPROVIDER_H
#define FEATUREPROVIDER_H

#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

class FeatureProvider
{
public:
	FeatureProvider();
	~FeatureProvider();

	enum Detectors {
		ORB_D = 1
		, SURF_D
		, SIFT_D
		, DENSE_D
		, FAST_D
		, GFTT_D
		, MSER_D
		, STAR_D
	};

	enum Extractors {
		ORB_E = 1
		, SURF_E
		, SIFT_E
		, BRIEF_E
	};


	cv::FeatureDetector* provideDetector();
	cv::DescriptorExtractor* provideExtractor();
	
	
	
	//// BONUS
	//cv::DescriptorMatcher* provideMatcher();



private:

	//bool mConfigurationChanged;
	QMap<QString, QVariant> mConfigurationMap;

	enum Detectors mCurrentDetector;
	enum Extractors mCurrentExtractor;


	void insert();
	
};

#endif // FEATUREPROVIDER_H
