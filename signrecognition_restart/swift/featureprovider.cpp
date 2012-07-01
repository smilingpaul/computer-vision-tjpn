#include "StdAfx.h"
#include "featureprovider.h"

FeatureProvider::FeatureProvider(QObject *parent)
	: QObject(parent)
{
	initialize();
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

	defaults();

	setCurrent(0,0);

	mDetector = provideDetector();
	mExtractor = provideExtractor();
}

cv::FeatureDetector* FeatureProvider::provideDetector()
{
	if (!mCurrentDetectorChanged)
		return mDetector;

	mDetector = nullptr;

	switch (mCurrentDetectorIndex)
	{
	case 0: ///< (0) ORB Detector

		mDetector = new cv::OrbFeatureDetector(
			value<int>("ORB","nfeatures")
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

		mDetector = new cv::SurfFeatureDetector(
			value<double>("SURF","hessianThreshold")
			, value<int>("nOctaves")
			, value<int>("nOctaveLayers")
			, value<bool>("extended")
			, value<bool>("upright")
		);

		break;

	case 2: ///< (2) SIFT Detector

		mDetector = new cv::SiftFeatureDetector(
			value<int>("SIFT","nfeatures")
			, value<int>("nOctaveLayers")
			, value<double>("contrastThreshold")
			, value<double>("edgeThreshold")
			, value<double>("sigma")
		);

		break;

	case 3: ///< (3) DENSE Detector

		mDetector = new cv::DenseFeatureDetector(
			value<float>("DENSE","initFeatureScale")
			, value<int>("featureScaleLevels")
			, value<float>("featureScaleMul")
			, value<int>("initXyStep")
			, value<int>("initImgBound")
			, value<bool>("varyXyStepWithScale")
			, value<bool>("varyImgBoundWithScale")
		);

		break;

	case 4: ///< (4) FAST Detector

		mDetector = new cv::FastFeatureDetector(
			value<int>("FAST","threshold")
			, value<bool>("nonmaxSuppression")
		);

		break;

	case 5: ///< (5) GFTT Detector

		mDetector = new cv::GFTTDetector(
			value<int>("GFTT","maxCorners")
			, value<double>("qualityLevel")
			, value<double>("minDistance")
			, value<int>("blockSize")
			, value<bool>("useHarrisDetector")
			, value<double>("k")
		);

		break;

	case 6: ///< (6) MSER Detector

		mDetector = new cv::MserFeatureDetector(
			value<int>("MSER","delta")
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

		mDetector = new cv::StarFeatureDetector(
			value<int>("STAR","maxSize")
			, value<int>("responseThreshold")
			, value<int>("lineThresholdProjected")
			, value<int>("lineThresholdBinarized")
			, value<int>("suppressNonmaxSize")
		);

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	//// BONUS!!!
	//cv::PyramidAdaptedFeatureDetector( const Ptr<FeatureDetector>& detector, int maxLevel=2 );
	//
	//cv::GridAdaptedFeatureDetector( const Ptr<FeatureDetector>& detector,
	//	int maxTotalKeypoints=1000,
	//	int gridRows=4, int gridCols=4 );

	mCurrentDetectorChanged = false;

	return mDetector;
}

cv::DescriptorExtractor* FeatureProvider::provideExtractor()
{
	if (!mCurrentExtractorChanged)
		return mExtractor;

	mExtractor = nullptr;

	switch (mCurrentExtractorIndex)
	{
	case 0: ///< (0) ORB Extractor

		mExtractor = new cv::OrbFeatureDetector(
			value<int>("ORB","nfeatures")
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

		mExtractor = new cv::SurfFeatureDetector(
			value<double>("SURF","hessianThreshold")
			, value<int>("nOctaves")
			, value<int>("nOctaveLayers")
			, value<bool>("extended")
			, value<bool>("upright")
			);

		break;

	case 2: ///< (2) SIFT Extractor

		mExtractor = new cv::SiftFeatureDetector(
			value<int>("SIFT","nfeatures")
			, value<int>("nOctaveLayers")
			, value<double>("contrastThreshold")
			, value<double>("edgeThreshold")
			, value<double>("sigma")
			);

		break;

	case 3: ///< (3) BRIEF Extractor

		mExtractor = new cv::BriefDescriptorExtractor(
			value<int>("BRIEF","bytes")
			);

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	//// BONUS!!!!
	//cv::BOWImgDescriptorExtractor( const Ptr<DescriptorExtractor>& dextractor,
	//	const Ptr<DescriptorMatcher>& dmatcher );

	mCurrentExtractorChanged = false;

	return mExtractor;
}

//cv::DescriptorMatcher* FeatureProvider::provideMatcher()
//{
////mMatcher = cv::FlannBasedMatcher(new cv::flann::CompositeIndexParams(), new cv::flann::SearchParams());
//}
//}
//}
//}

template<typename T> void FeatureProvider::insert(QString key, T input)
{
	key = mPrefix + ":" + key;

	if (mDetectorList.contains(key))
		mCurrentDetectorChanged = true;

	if (mExtractorList.contains(key))
		mCurrentExtractorChanged = true;

	mConfiguration.insert(key, QVariant::fromValue<T>(input));
	mConfigurationType.insert(key, QString(typeid(input).name()));
}

template<typename T> void FeatureProvider::insert(QString prefix, QString key, T input)
{
	if (mDetectorList.contains(prefix) || mExtractorList.contains(prefix))
		mPrefix = prefix;

	insert<T>(key,input);
}

//////////////////////////////////////////////////////////////////////////
/// specialization if no template is given - just implemented for qt gui

QVariant FeatureProvider::value(QString key)
{
	return mConfiguration[mPrefix + ":" + key].value<QVariant>();
}

QVariant FeatureProvider::value(QString prefix, QString key)
{
	if (mDetectorList.contains(prefix) || mExtractorList.contains(prefix))
		mPrefix = prefix;

	return value(key);
}

//////////////////////////////////////////////////////////////////////////

template<typename T> T FeatureProvider::value(QString key)
{
	return mConfiguration[mPrefix + ":" + key].value<T>();
}

template<typename T> T FeatureProvider::value(QString prefix, QString key)
{
	if (mDetectorList.contains(prefix) || mExtractorList.contains(prefix))
		mPrefix = prefix;

	return value<T>(key);
}

void FeatureProvider::defaults()
{
	insert<int>("ORB","nfeatures",500);
	insert<float>("scaleFactor",1.2);
	insert<int>("nlevels",8);
	insert<int>("edgeThreshold",31);
	insert<int>("firstLevel",0);
	insert<int>("WTA_K",2);
	insert<int>("scoreType",0);
	insert<int>("patchSize",31);

	insert<double>("SURF","hessianThreshold",600);
	insert<int>("nOctaves",4);
	insert<int>("nOctaveLayers",2);
	insert<bool>("extended",true);
	insert<bool>("upright",false);

	insert<int>("SIFT","nfeatures",0);
	insert<int>("nOctaveLayers",3);
	insert<double>("contrastThreshold",0.04);
	insert<double>("edgeThreshold",10);
	insert<double>("sigma",1.6);

	insert<float>("DENSE","initFeatureScale",1);
	insert<int>("featureScaleLevels",1);
	insert<float>("featureScaleMul",0.1);
	insert<int>("initXyStep",6);
	insert<int>("initImgBound",0);
	insert<bool>("varyXyStepWithScale",true);
	insert<bool>("varyImgBoundWithScale",false);

	insert<int>("FAST","threshold",10);
	insert<bool>("nonmaxSuppression",true);

	insert<int>("GFTT","maxCorners",1000);
	insert<double>("qualityLevel",0.01);
	insert<double>("minDistance",1);
	insert<int>("blockSize",3);
	insert<bool>("useHarrisDetector",false);
	insert<double>("k",0.04);

	insert<int>("MSER","delta",5);
	insert<int>("min_area",60);
	insert<int>("max_area",14400);
	insert<double>("max_variation",0.25);
	insert<double>("min_diversity",0.2);
	insert<int>("max_evolution",200);
	insert<double>("area_threshold",1.01);
	insert<double>("min_margin",0.03);
	insert<int>("edge_blur_size",5);

	insert<int>("STAR","maxSize",45);
	insert<int>("responseThreshold",30);
	insert<int>("lineThresholdProjected",10);
	insert<int>("lineThresholdBinarized",8);
	insert<int>("suppressNonmaxSize",5);

	insert<int>("BRIEF","bytes",32);
}

void FeatureProvider::setCurrent(int detector, int extractor)
{
	if (detector > -1 && detector < mDetectorList.size())
	{
		mCurrentDetector = mDetectorList[detector];
		mCurrentDetectorIndex = detector;
		mPrefix = mCurrentDetector;
		mCurrentDetectorChanged = true;
	}

	if (extractor > -1 && extractor < mExtractorList.size())
	{
		mCurrentExtractor = mExtractorList[extractor];
		mCurrentExtractorIndex = extractor;
		mPrefix = mCurrentExtractor;
		mCurrentExtractorChanged = true;
	}
}

void FeatureProvider::setCurrent(QString detector, QString extractor)
{
	int d = mDetectorList.indexOf(detector);
	int e = mExtractorList.indexOf(extractor);

	if (d == -1 && e == -1)
		return;

	setCurrent(d,e);
}

//////////////////////////////////////////////////////////////////////////
QStringList FeatureProvider::getDetectorList() { return mDetectorList; }
QStringList FeatureProvider::getExtractorList() { return mExtractorList; }
QString FeatureProvider::getCurrentDetector() { return mCurrentDetector; }
QString FeatureProvider::getCurrentExtractor() { return mCurrentExtractor; }
int FeatureProvider::getCurrentDetectorIndex() { return mCurrentDetectorIndex; }
int FeatureProvider::getCurrentExtractorIndex() { return mCurrentExtractorIndex; }