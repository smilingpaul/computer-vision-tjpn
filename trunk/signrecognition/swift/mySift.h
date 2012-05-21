//#pragma once

//#include <cv.h>
#include "swift.h"
#include "stdafx.h"



class mySIFT
{
public:
	mySift(IplImage* img, int octaves, int intervals);
	mySift(const char* filename, int octaves, int intervals);
	~mySift();

	void DoMySift();

	void ShowKeypoints();
	void ShowAbsSigma();

private:
	void GenerateLists();
	void BuildScaleSpace();
	void DetectExtrema();
	void AssignOrientations();
	void ExtractKeypointDescriptors();

	unsigned int GetKernelSize(double sigma, double cut_off=0.001);
	CvMat* BuildInterpolatedGaussianTable(unsigned int size, double sigma);
	double gaussian2D(double x, double y, double sigma);


private:
	IplImage* myImage;			// The image we're working on
	unsigned int myNumOctaves;		// The desired number of octaves
	unsigned int myNumIntervals;	// The desired number of intervals
	unsigned int myNumKeypoints;	// The number of keypoints detected

	IplImage***	myGaussList;		// A 2D array to hold the different gaussian blurred images
	IplImage*** myDogList;		// A 2D array to hold the different DoG images
	IplImage*** myExtrema;		// A 2D array to hold binary images. In the binary image, 1 = extrema, 0 = not extrema
	double**	myAbsSigma;		// A 2D array to hold the sigma used to blur a particular image

//	vector<Keypoint> m_keyPoints;	// Holds each keypoint's basic info
//	vector<Descriptor> m_keyDescs;	// Holds each keypoint's descriptor
};