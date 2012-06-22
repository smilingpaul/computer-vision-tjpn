#include "StdAfx.h"
#include "featureprovider.h"

//mSift = cv::SIFT();
//mMatcher = cv::FlannBasedMatcher(new cv::flann::CompositeIndexParams(), new cv::flann::SearchParams());
//mDetector = cv::SiftFeatureDetector(mSift);
//mExtractor = cv::SiftDescriptorExtractor(mSift);


FeatureProvider::FeatureProvider()
{

	//mConfigurationMap.insert("SIFT",);
}

FeatureProvider::~FeatureProvider()
{

}

cv::FeatureDetector* FeatureProvider::provideDetector()
{	
	cv::FeatureDetector *detector = nullptr;

	switch (mCurrentDetector)
	{
	case 1: ///< (1) ORB Detector

		detector = new cv::OrbFeatureDetector(
			  int _nfeatures = 500
			, float _scaleFactor = 1.2f
			, int _nlevels = 8
			, int _edgeThreshold = 31
			, int _firstLevel = 0
			, int _WTA_K=2
			, int _scoreType=0
			, int _patchSize=31
		);

		break;

	case 2: ///< (2) SURF Detector

		detector = new cv::SurfFeatureDetector(
			  double _hessianThreshold
			, int _nOctaves=4
			, int _nOctaveLayers=2
			, bool _extended = true
			, bool _upright = false
		);

		break;

	case 3: ///< (3) SIFT Detector

		detector = new cv::SiftFeatureDetector(
			  int _nfeatures=0
			, int _nOctaveLayers=3
			, double _contrastThreshold=0.04
			, double _edgeThreshold=10
			, double _sigma=1.6
		);

		break;

	case 4: ///< (4) DENSE Detector

		detector = new cv::DenseFeatureDetector(
			  float _initFeatureScale=1.f
			, int _featureScaleLevels=1
			, float _featureScaleMul=0.1f
			, int _initXyStep=6
			, int _initImgBound=0
			, bool _varyXyStepWithScale=true
			, bool _varyImgBoundWithScale=false
		);
		
		break;

	case 5: ///< (5) FAST Detector

		detector = new cv::FastFeatureDetector(
			  int _threshold=10
			, bool nonmaxSuppression=true
		);

		break;

	case 6: ///< (6) GFTT Detector

		detector = new cv::GFTTDetector(
			  int _maxCorners=1000
			, double _qualityLevel=0.01
			, double _minDistance=1
			, int _blockSize=3
			, bool _useHarrisDetector=false
			, double _k=0.04 
		);

		break;

	case 7: ///< (7) MSER Detector

		detector = new cv::MserFeatureDetector(
			  int _delta=5
			, int _min_area=60
			, int _max_area=14400
			, double _max_variation=0.25
			, double _min_diversity=.2
			, int _max_evolution=200
			, double _area_threshold=1.01
			, double _min_margin=0.003
			, int _edge_blur_size=5
		);

		break;

	case 8: ///< (8) STAR Detector

		detector = new cv::StarFeatureDetector(
			  int _maxSize=45
			, int _responseThreshold=30
			, int _lineThresholdProjected=10
			, int _lineThresholdBinarized=8
			, int _suppressNonmaxSize=5
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
	/// (1) ORB
	//cv::OrbDescriptorExtractor(int nfeatures = 500, float scaleFactor = 1.2f, int nlevels = 8, int edgeThreshold = 31,
	//	int firstLevel = 0, int WTA_K=2, int scoreType=0, int patchSize=31 );


	/// (2) SURF
	//cv::SurfDescriptorExtractor(double _hessianThreshold,
	//int _nOctaves=4, int _nOctaveLayers=2,
	//bool _extended=true, bool _upright=false);

	/// (3) SIFT
	//cv::SiftDescriptorExtractor( int _nfeatures=0, int _nOctaveLayers=3,
	//	double _contrastThreshold=0.04, double _edgeThreshold=10,
	//	double _sigma=1.6);


	/// (4) BRIEF
	//cv::BriefDescriptorExtractor( int bytes = 32 )


















	//// BONUS!!!!
	//cv::BOWImgDescriptorExtractor( const Ptr<DescriptorExtractor>& dextractor,
	//	const Ptr<DescriptorMatcher>& dmatcher );

}

//cv::DescriptorMatcher* FeatureProvider::provideMatcher()
//{
//
//}
