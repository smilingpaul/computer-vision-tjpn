#ifndef HANDCRAFTEDSIFT_H
#define HANDCRAFTEDSIFT_H

#define SIGMA_ANTIALIAS			0.5
#define SIGMA_PREBLUR			1.0
#define CURVATURE_THRESHOLD		5.0
#define CONTRAST_THRESHOLD		0.03		// in terms of 255
#define NUM_BINS				36
#define MAX_KERNEL_SIZE			20
#define FEATURE_WINDOW_SIZE		16
#define DESC_NUM_BINS			8
#define FVSIZE					128
#define	FV_THRESHOLD			0.2

// #warning
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

class HandcraftedSIFT
{
public:
	HandcraftedSIFT(void);
	virtual ~HandcraftedSIFT(void);

	void sift(int octavesCount = 4, int intervalsCount = 5); ///< suggested default values by david g. lowe
	void allocateMemory();
	void buildScaleSpaces();
	void detectExtrema();
	void assignOrientations();
	void extractKeypointDescriptors();

private:
	cv::Mat m_image;

	int m_octavesCount;
	int m_intervalsCount;
	int m_keypointsCount;

	cv::Mat*** m_gaussians; ///< array to hold all the octaves and intervals
	cv::Mat*** m_dogs;      ///< array to hold the "difference of gaussian" images between the intervals of each octave
	cv::Mat*** m_extrema;   ///< array to hold binary images with extrema information (true or false)
	double**   m_sigmas;    ///< array to hold the sigma used to blur a particular image

	//vector<cv::KeyPoint>   m_keypoints;           ///< vector to hold every calculated keypoint
	//vector<cv::Desc> m_keypointDescriptors; ///< vector to hold every keypoints descriptor

	//vector<cv::Mat> images;
	//vector<vector<cv::KeyPoint> > keypointsCollection;
	//vector<cv::Mat> descriptorsCollection;
};

#endif // HANDCRAFTEDSIFT_H