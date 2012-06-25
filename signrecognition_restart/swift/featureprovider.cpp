#include "StdAfx.h"
#include "featureprovider.h"

//mSift = cv::SIFT();
//mMatcher = cv::FlannBasedMatcher(new cv::flann::CompositeIndexParams(), new cv::flann::SearchParams());
//mDetector = cv::SiftFeatureDetector(mSift);
//mExtractor = cv::SiftDescriptorExtractor(mSift);

FeatureProvider::FeatureProvider()
{
}

FeatureProvider::~FeatureProvider()
{
}

void FeatureProvider::initialize()
{
	mDetectorList
		<< "ORB"
		<< "SURF"
		<< "SIFT"
		<< "DENSE"
		<< "FAST"
		<< "GFTT"
		<< "MSER"
		<< "STAR";

	mExtractorList
		<< "ORB"
		<< "SURF"
		<< "SIFT"
		<< "BRIEF";

	setDefault();
	setCurrent(0,0);
}

cv::FeatureDetector* FeatureProvider::provideDetector()
{
	cv::FeatureDetector *detector = nullptr;

	switch (mDetectorList.indexOf(mCurrentDetector))
	{
	case 0: ///< (0) ORB Detector

		detector = new cv::OrbFeatureDetector(
			  value<int>("nfeatures")
			, value<float>("scaleFactor")
			, value<int>("nlevels")
			, value<int>("edgeThreshold")
			, value<int>("firstLevel")
			, value<int>("WTA_K")
			, value<int>("scoreType")
			, value<int>("patchSize")
		);

		break;

	case 1: ///< (1) SURF Detector

		detector = new cv::SurfFeatureDetector(
			  value<double>("hessianThreshold")
			, value<int>("nOctaves")
			, value<int>("nOctaveLayers")
			, value<bool>("extended")
			, value<bool>("upright")
		);

		break;

	case 2: ///< (2) SIFT Detector

		detector = new cv::SiftFeatureDetector(
			  value<int>("nfeatures")
			, value<int>("nOctaveLayers")
			, value<double>("contrastThreshold")
			, value<double>("edgeThreshold")
			, value<double>("sigma")
		);

		break;

	case 3: ///< (3) DENSE Detector

		detector = new cv::DenseFeatureDetector(
			  value<float>("initFeatureScale")
			, value<int>("featureScaleLevels")
			, value<float>("featureScaleMul")
			, value<int>("initXyStep")
			, value<int>("initImgBound")
			, value<bool>("varyXyStepWithScale")
			, value<bool>("varyImgBoundWithScale")
		);

		break;

	case 4: ///< (4) FAST Detector

		detector = new cv::FastFeatureDetector(
			  value<int>("threshold")
			, value<bool>("nonmaxSuppression")
		);

		break;

	case 5: ///< (5) GFTT Detector

		detector = new cv::GFTTDetector(
			  value<int>("maxCorners")
			, value<double>("qualityLevel")
			, value<double>("minDistance")
			, value<int>("blockSize")
			, value<bool>("useHarrisDetector")
			, value<double>("_k")
		);

		break;

	case 6: ///< (6) MSER Detector

		detector = new cv::MserFeatureDetector(
			  value<int>("delta")
			, value<int>("min_area")
			, value<int>("max_area")
			, value<double>("max_variation")
			, value<double>("min_diversity")
			, value<int>("max_evolution")
			, value<double>("area_threshold")
			, value<double>("min_margin")
			, value<int>("edge_blur_size")
		);

		break;

	case 7: ///< (7) STAR Detector

		detector = new cv::StarFeatureDetector(
			  value<int>("maxSize")
			, value<int>("responseThreshold")
			, value<int>("lineThresholdProjected")
			, value<int>("lineThresholdBinarized")
			, value<int>("suppressNonmaxSize")
		);

		break;
	}

	//// BONUS!!!
	//cv::PyramidAdaptedFeatureDetector( const Ptr<FeatureDetector>& detector, int maxLevel=2 );
	//
	//cv::GridAdaptedFeatureDetector( const Ptr<FeatureDetector>& detector,
	//	int maxTotalKeypoints=1000,
	//	int gridRows=4, int gridCols=4 );

	return detector;
}

cv::DescriptorExtractor* FeatureProvider::provideExtractor()
{
	cv::DescriptorExtractor *extractor = nullptr;

	switch (mExtractorList.indexOf(mCurrentDetector))
	{
	case 0: ///< (0) ORB Extractor

		extractor = new cv::OrbFeatureDetector(
			  value<int>("nfeatures")
			, value<float>("scaleFactor")
			, value<int>("nlevels")
			, value<int>("edgeThreshold")
			, value<int>("firstLevel")
			, value<int>("WTA_K")
			, value<int>("scoreType")
			, value<int>("patchSize")
			);

		break;

	case 1: ///< (1) SURF Extractor

		extractor = new cv::SurfFeatureDetector(
			  value<double>("hessianThreshold")
			, value<int>("nOctaves")
			, value<int>("nOctaveLayers")
			, value<bool>("extended")
			, value<bool>("upright")
			);

		break;

	case 2: ///< (2) SIFT Extractor

		extractor = new cv::SiftFeatureDetector(
			  value<int>("nfeatures")
			, value<int>("nOctaveLayers")
			, value<double>("contrastThreshold")
			, value<double>("edgeThreshold")
			, value<double>("sigma")
			);

		break;

	case 3: ///< (3) BRIEF Extractor

		extractor = new cv::BriefDescriptorExtractor(
			  value<int>("bytes")
			);

		break;
	}

	//// BONUS!!!!
	//cv::BOWImgDescriptorExtractor( const Ptr<DescriptorExtractor>& dextractor,
	//	const Ptr<DescriptorMatcher>& dmatcher );

	return extractor;
}

//cv::DescriptorMatcher* FeatureProvider::provideMatcher()
//{
//
//}
//}
//}
//}

template<typename T> void FeatureProvider::insert(QString key, T input)
{
	mConfiguration.insert(key,QVariant(input));
}

template<typename T> T FeatureProvider::value(QString key)
{
	return mConfiguration[mCurrentPrefix+"_"+key].value<T>();
}

void FeatureProvider::setDefault()
{
	mConfigurationDefault.clear();

	mConfigurationDefault.insert("ORB_nfeatures",QVariant(int(500)));
	mConfigurationDefault.insert("ORB_scaleFactor",QVariant(float(1.2)));
	mConfigurationDefault.insert("ORB_nlevels",QVariant(int(8)));
	mConfigurationDefault.insert("ORB_edgeThreshold",QVariant(int(31)));
	mConfigurationDefault.insert("ORB_firstLevel",QVariant(int(0)));
	mConfigurationDefault.insert("ORB_WTA_K",QVariant(int(2)));
	mConfigurationDefault.insert("ORB_scoreType",QVariant(int(0)));
	mConfigurationDefault.insert("ORB_patchSize",QVariant(int(31)));

	mConfigurationDefault.insert("SURF_hessianThreshold",QVariant(double(600)));
	mConfigurationDefault.insert("SURF_nOctaves",QVariant(int(4)));
	mConfigurationDefault.insert("SURF_nOctaveLayers",QVariant(int(2)));
	mConfigurationDefault.insert("SURF_extended",QVariant(bool(true)));
	mConfigurationDefault.insert("SURF_upright",QVariant(bool(false)));

	mConfigurationDefault.insert("SIFT_nfeatures",QVariant(int(0)));
	mConfigurationDefault.insert("SIFT_nOctaveLayers",QVariant(int(3)));
	mConfigurationDefault.insert("SIFT_contrastThreshold",QVariant(double(0.04)));
	mConfigurationDefault.insert("SIFT_edgeThreshold",QVariant(double(10)));
	mConfigurationDefault.insert("SIFT_sigma",QVariant(double(1.6)));

	mConfigurationDefault.insert("DENSE_initFeatureScale",QVariant(float(1)));
	mConfigurationDefault.insert("DENSE_featureScaleLevels",QVariant(int(1)));
	mConfigurationDefault.insert("DENSE_featureScaleMul",QVariant(float(0.1)));
	mConfigurationDefault.insert("DENSE_initXyStep",QVariant(int(6)));
	mConfigurationDefault.insert("DENSE_initImgBound",QVariant(int(0)));
	mConfigurationDefault.insert("DENSE_varyXyStepWithScale",QVariant(bool(true)));
	mConfigurationDefault.insert("DENSE_varyImgBoundWithScale",QVariant(bool(false)));

	mConfigurationDefault.insert("FAST_threshold",QVariant(int(10)));
	mConfigurationDefault.insert("FAST_nonmaxSuppression",QVariant(bool(true)));

	mConfigurationDefault.insert("GFTT_maxCorners",QVariant(int(1000)));
	mConfigurationDefault.insert("GFTT_qualityLevel",QVariant(double(0.01)));
	mConfigurationDefault.insert("GFTT_minDistance",QVariant(double(1)));
	mConfigurationDefault.insert("GFTT_blockSize",QVariant(int(3)));
	mConfigurationDefault.insert("GFTT_useHarrisDetector",QVariant(bool(false)));
	mConfigurationDefault.insert("GFTT_k",QVariant(double(0.04)));

	mConfigurationDefault.insert("MSER_delta",QVariant(int(5)));
	mConfigurationDefault.insert("MSER_min_area",QVariant(int(60)));
	mConfigurationDefault.insert("MSER_max_area",QVariant(int(14400)));
	mConfigurationDefault.insert("MSER_max_variation",QVariant(double(0.25)));
	mConfigurationDefault.insert("MSER_min_diversity",QVariant(double(0.2)));
	mConfigurationDefault.insert("MSER_max_evolution",QVariant(int(200)));
	mConfigurationDefault.insert("MSER_area_threshold",QVariant(double(1.01)));
	mConfigurationDefault.insert("MSER_min_margin",QVariant(double(0.03)));
	mConfigurationDefault.insert("MSER_edge_blur_size",QVariant(int(5)));

	mConfigurationDefault.insert("STAR_maxSize",QVariant(int(45)));
	mConfigurationDefault.insert("STAR_responseThreshold",QVariant(int(30)));
	mConfigurationDefault.insert("STAR_lineThresholdProjected",QVariant(int(10)));
	mConfigurationDefault.insert("STAR_lineThresholdBinarized",QVariant(int(8)));
	mConfigurationDefault.insert("STAR_suppressNonmaxSize",QVariant(int(5)));

	mConfigurationDefault.insert("BRIEF_bytes",QVariant(int(32)));

	mConfiguration.clear();

	mConfiguration = QMap<QString, QVariant>(mConfigurationDefault);
}

void FeatureProvider::setCurrent(int detector, int extractor)
{
	if (detector > -1 && detector < mDetectorList.size())
	{
		mCurrentDetector = mDetectorList[detector];
		mCurrentPrefix = mCurrentDetector;
	}

	if (extractor > -1 && extractor < mExtractorList.size())
	{
		mCurrentExtractor = mExtractorList[extractor];
		mCurrentPrefix = mCurrentDetector;
	}
}

void FeatureProvider::setCurrent(QString detector, QString extractor)
{
	if (!detector.isEmpty() && mDetectorList.contains(detector))
	{
		mCurrentDetector = detector;
		mCurrentPrefix = mCurrentDetector;
	}

	if (!extractor.isEmpty() && mExtractorList.contains(extractor))
	{
		mCurrentExtractor = extractor;
		mCurrentPrefix = mCurrentDetector;
	}
}