#ifndef SANDBOXSIFT_H
#define SANDBOXSIFT_H

#include "opencv2\core\core.hpp" ///< basic datastructures and functions
#include "opencv2\imgproc\imgproc.hpp" ///< image processing such as cv::resize
#include "opencv2\core\types_c.h" ///< cvround & datatype convenience definitions
#include "opencv2\features2d\features2d.hpp" ///< cv::keypoint & more

class SandboxSIFT
{
public:
	SandboxSIFT(int _nfeatures, int _nOctaveLayers, double _contrastThreshold, double _edgeThreshold, double _sigma);
	void SandboxSIFT::operator()(cv::InputArray _image, cv::InputArray _mask, cv::vector<cv::KeyPoint>& _keypoints, cv::OutputArray _descriptors, bool _useProvidedKeypoints) const;
	~SandboxSIFT();

	//static cv::Mat createInitialImage( const cv::Mat& img, bool doubleImageSize, float sigma );
	void SandboxSIFT::buildGaussianPyramid( const cv::Mat& base, cv::vector<cv::Mat>& pyr, int nOctaves ) const;
	void SandboxSIFT::buildDoGPyramid( const cv::vector<cv::Mat>& gpyr, cv::vector<cv::Mat>& dogpyr ) const;
	//static float calcOrientationHist( const cv::Mat& img, cv::Point pt, int radius, float sigma, float* hist, int n );
	//static bool adjustLocalExtrema( const cv::vector<cv::Mat>& dog_pyr, cv::KeyPoint& kpt, int octv, int& layer, int& r, int& c, int nOctaveLayers, float contrastThreshold, float edgeThreshold, float sigma );
	void SandboxSIFT::findScaleSpaceExtrema( const cv::vector<cv::Mat>& gauss_pyr, const cv::vector<cv::Mat>& dog_pyr, cv::vector<cv::KeyPoint>& keypoints ) const;
	//static void calcSIFTDescriptor( const cv::Mat& img, cv::Point2f ptf, float ori, float scl, int d, int n, float* dst )
	//static void calcDescriptors(const vector<cv::Mat>& gpyr, const cv::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors, int nOctaveLayers )

	int SandboxSIFT::descriptorSize() const;
	int SandboxSIFT::descriptorType() const;

	void SandboxSIFT::detectImpl( const cv::Mat& image, cv::vector<cv::KeyPoint>& keypoints, const cv::Mat& mask) const;
	void SandboxSIFT::computeImpl( const cv::Mat& image, cv::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors) const;

private:
	int nfeatures;
	int nOctaveLayers;
	double contrastThreshold;
	double edgeThreshold;
	double sigma;
};

#endif // SANDBOXSIFT_H