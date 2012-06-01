#include "StdAfx.h"
#include "handcraftedsift.h"

/// Handcrafted SIFT
//////////////////////////////////////////////////////////////////////////

HandcraftedSIFT::HandcraftedSIFT(void)
{
}

HandcraftedSIFT::~HandcraftedSIFT(void)
{
	unsigned int i, j;

	for( i = 0; i < m_octavesCount; i++ )
	{
		// release all mats in that particular octave
		for( j = 0; j < m_intervalsCount + 3; j++ )
		{
			~*m_gaussians[i][j];
		}

		for( j = 0; j < m_intervalsCount + 2; j++ )
		{
			~*m_dogs[i][j];
		}

		for( j = 0; j < m_intervalsCount; j++ )
		{
			~*m_extrema[i][j];
		}

		// delete the particular octave
		delete [] m_gaussians[i];
		delete [] m_dogs[i];
		delete [] m_extrema[i];
		delete [] m_sigmas[i];
	}

	// delete the arrays
	delete [] m_gaussians;
	delete [] m_dogs;
	delete [] m_extrema;
	delete [] m_sigmas;
}

/// fantastic port
void HandcraftedSIFT::sift(int octavesCount /* = 4 */, int intervalsCount /* = 5 */)
{
	//not needed
	//m_octavesCount   = octavesCount;
	//m_intervalsCount = intervalsCount;

	//allocateMemory();
	//buildScaleSpaces();
	//detectExtrema();
	//assignOrientations();
	//extractKeypointDescriptors();

	
}

//CreateInitImg()
//need to double size and convert colored image in gray image
cv::Mat HandcraftedSIFT::createInitImg()
	{
		cv::Mat grayImage, doubleSizeImage;

		if (m_image.channels() == 3 || m_image.channels() == 4)
			cvtColor(m_image, grayImage, CV_BGR2GRAY);
		
		else m_image.copyTo(grayImage);

	
		//now blur image and double its size
     float sig_diff;

        sig_diff = sqrtf( (SIGMA * SIGMA - SIGMA_ANTIALIAS * SIGMA_ANTIALIAS * 4, 0.01f) );	//cv::max() funkioniert nicht->warum?
        resize(grayImage, doubleSizeImage, cvSize(grayImage.cols*2, grayImage.rows*2), 0, 0, cv::INTER_LINEAR);
        GaussianBlur(doubleSizeImage, doubleSizeImage, cvSize(doubleSizeImage.cols, doubleSizeImage.rows), sig_diff, sig_diff);
        return doubleSizeImage;

	}

void HandcraftedSIFT::buildScaleSpaces(const cv::Mat& base, cv::vector<cv::Mat>& m_pyr, int m_octavesCount ) const
{
	printf("Generating scale space...\n");

	cv::vector<double> m_sigmas(m_octavesCount + 3);
	m_pyr.resize(m_octavesCount*(m_octavesCount + 3));

	// precompute Gaussian sigmas using the following formula:
    //  \sigma_{total}^2 = \sigma_{i}^2 + \sigma_{i-1}^2
	
	m_sigmas[0] = sigma;
	double k = pow(2., 1./ m_octavesCount);

	for ( int i = 1; i < m_octavesCount + 3; i++)
	{
		double sig_prev = pow(k, (double)(i-1))*sigma;
		double sig_total = sig_prev*k;
		m_sigmas[i] = cv::sqrt(sig_total - sig_prev*sig_prev);
	}

	for ( int o = 0; o < m_octavesCount + 3; o++)
	{
		for ( int i = 0; i < m_octavesCount + 3; i++)
		{
			cv::Mat& dest = m_pyr[o*(m_octavesCount + 3) + i];
			if ( o == 0 && i == 0 )
				dest = base;
			//base of new octave is halved image from end of prevoius octave
			else if ( i == 0 )
			{
				const cv::Mat& src = m_pyr[(o-1)*(m_octavesCount + 3) + m_octavesCount];
				resize(src, dest, cvSize(src.cols/2, src.rows/2), 
					0, 0, cv::INTER_NEAREST);		//INTER_NEAREST - nearest neighbour interpolation
			}
			else
			{
				const cv::Mat& src = m_pyr[o*(m_octavesCount + 3) + i-1];
				GaussianBlur(src, dest, cvSize(), m_sigmas[i], m_sigmas[i]);
			}
		}
	}
}


void HandcraftedSIFT::buildDoGPyramid( const cv::vector<cv::Mat>& gaussPyr, cv::vector<cv::Mat>& dogPyr ) const
{
	printf("Generating DoG-Pyramid...\n");

    int nOctaves = (int)gaussPyr.size()/(m_octavesCount + 3);
    dogPyr.resize( nOctaves*(m_octavesCount + 2) );
    
    for( int o = 0; o < nOctaves; o++ )
    {
        for( int i = 0; i < m_octavesCount + 2; i++ )
        {
            const cv::Mat& src1 = gaussPyr[o*(m_octavesCount + 3) + i];
            const cv::Mat& src2 = gaussPyr[o*(m_octavesCount + 3) + i + 1];
            cv::Mat& dest = dogPyr[o*(m_octavesCount + 2) + i];
            subtract(src2, src1, dest, cv::noArray(), CV_16S);
        }
    }
}


// find extrema in DoG
void HandcraftedSIFT::detectExtrema()
{
	printf("Detecting extrema...\n");



}



// not needed
//void HandcraftedSIFT::allocateMemory()
//{
//	//nicht fragmentiert
//	//m_gaussians = new cv::Mat*[m_octavesCount*(m_intervalsCount + 3)];
//
//	m_gaussians = new cv::Mat**[m_octavesCount];
//	m_dogs      = new cv::Mat**[m_octavesCount];
//	m_extrema   = new cv::Mat**[m_octavesCount];
//	m_sigmas    = new double*  [m_octavesCount];
//
//	for( unsigned int i = 0; i < m_octavesCount; i++ )
//	{
//		/// why +3?
//		m_gaussians[i] = new cv::Mat*[m_intervalsCount + 3];
//		/// why +2?
//		m_dogs[i]      = new cv::Mat*[m_intervalsCount + 2];
//		/// why +0?
//		m_extrema[i]   = new cv::Mat*[m_intervalsCount];
//		/// why +3?
//		m_sigmas[i]    = new double  [m_intervalsCount + 3];
//	}
//}

//////////////////////////////////////////////////////////////////////////

//// This function generates all the blurred out images for each octave
//// and also the DoG images
//void SwiftItem::buildScaleSpaces()
//{
//	printf("Generating scale space...\n");
//	// For loops
//	unsigned int i,j;
//
//	// floating point grayscale image
//	IplImage* imgGray = cvCreateImage(cvGetSize(m_srcImage), IPL_DEPTH_32F , 1);
//	IplImage* imgTemp = cvCreateImage(cvGetSize(m_srcImage), 8 , 1);
//
//	//Mat imgTemp;
//	//cvtColor(m_image,im_gray,CV_RGB2GRAY);
//
//	// Create a duplicate. We don't want to mess the original
//	// If the image is colour, it is converted to grayscale
//	if(m_srcImage->nChannels==3)
//	{
//		cvCvtColor(m_srcImage, imgTemp, CV_BGR2GRAY);
//	}
//	else
//	{
//		cvCopy(m_srcImage, imgTemp);
//	}
//
//	// Finally, generate the floating point image... convert 0..255 range into 0..1
//	for(int x=0;x<imgTemp->width;x++)
//	{
//		for(int y=0;y<imgTemp->height;y++)
//		{
//			cvSetReal2D(imgGray, y, x, cvGetReal2D(imgTemp, y, x)/255.0);
//		}
//	}
//
//	// Lowe claims blur the image with a sigma of 0.5 and double it's dimensions
//	// to increase the number of stable keypoints
//	cvSmooth(imgGray, imgGray, CV_GAUSSIAN, 0, 0, SIGMA_ANTIALIAS);
//
//	// Create an image double the dimensions, resize imgGray and store it in m_gaussians[0][0]
//	m_gaussians[0][0] = cvCreateImage(cvSize(imgGray->width*2, imgGray->height*2), IPL_DEPTH_32F , 1);
//	cvPyrUp(imgGray, m_gaussians[0][0]);
//
//	// Preblur this base image
//	cvSmooth(m_gaussians[0][0], m_gaussians[0][0], CV_GAUSSIAN, 0, 0, SIGMA_PREBLUR);
//
//	// SaveFloatingPointImage("C:\\SIFT Test\\Gaussian\\g_octave_0_scale_0.jpg", m_gaussians[0][0]);
//
//	double initSigma = sqrt(2.0f);
//
//	// Keep a track of the sigmas
//	m_sigmas[0][0] = initSigma * 0.5;
//
//	// Now for the actual image generation
//	for(i=0;i<m_octavesCount;i++)
//	{
//		// Reset sigma for each octave
//		double sigma = initSigma;
//		CvSize currentSize = cvGetSize(m_gaussians[i][0]);
//
//		for(j=1;j<m_intervalsCount+3;j++)
//		{
//			// Allocate memory
//			m_gaussians[i][j] = cvCreateImage(currentSize, 32, 1);
//
//			// Calculate a sigma to blur the current image to get the next one
//			double sigma_f = sqrt(pow(2.0,2.0/m_intervalsCount)-1) * sigma;
//            sigma = pow(2.0,1.0/m_intervalsCount) * sigma;
//
//			// Store sigma values (to be used later on)
//			m_sigmas[i][j] = sigma * 0.5 * pow(2.0f, (float)i);
//
//			// Apply gaussian smoothing)
//			cvSmooth(m_gaussians[i][j-1], m_gaussians[i][j], CV_GAUSSIAN, 0, 0, sigma_f);
//
//			// Calculate the DoG image
//			m_dogs[i][j-1] = cvCreateImage(currentSize, 32, 1);
//			cvSub(m_gaussians[i][j-1], m_gaussians[i][j], m_dogs[i][j-1]);
//
//			// Save the images generated for fun :)
//			/*char* filename = new char[200];
//			sprintf(filename, "C:\\SIFT Test\\Gaussian\\g_octave_%d_scale_%d.jpg", i, j);
//			SaveFloatingPointImage(filename, m_gaussians[i][j]);
//
//			sprintf(filename, "C:\\SIFT TEST\\DOG\\dog_octave_%d_scale_%d.jpg", i, j-1);
//			SaveFloatingPointImage(filename, m_dogs[i][j-1]);*/
//		}
//
//		// If we're not at the last octave
//		if(i<m_octavesCount-1)
//		{
//			// Reduce size to half
//			currentSize.width/=2;
//			currentSize.height/=2;
//
//			// Allocate memory and resample the image
//			m_gaussians[i+1][0] = cvCreateImage(currentSize, 32, 1);
//			cvPyrDown(m_gaussians[i][0], m_gaussians[i+1][0]);
//			//resize(m_gaussians[i][0], m_gaussians[i+1][0], m_gaussians[i+1][0].size(), 0, 0, INTER_LINEAR);
//			m_sigmas[i+1][0] = m_sigmas[i][m_intervalsCount];
//
//			// Store the image
//			/*char* filename = new char[200];
//			sprintf(filename, "C:\\SIFT Test\\Gaussian\\g_octave_%d_scale_0.jpg", i+1);
//			SaveFloatingPointImage(filename, m_gaussians[i+1][0]);*/
//		}
//	}
//}
//
//// Locates extreme points (maxima and minima)
//// Relatively simple stuff
//void SwiftItem::detectExtrema()
//{
//	printf("Detecting extrema...\n");
//
//	// Looping variables
//	unsigned int i, j, xi, yi;
//
//	// Some variables we'll use later on
//	double curvature_ratio, curvature_threshold;
//	IplImage *middle, *up, *down;
//	int scale;
//	double dxx, dyy, dxy, trH, detH;
//
//	unsigned int num=0;				// Number of keypoins detected
//	unsigned int numRemoved=0;		// The number of key points rejected because they failed a test
//
//	curvature_threshold = (CURVATURE_THRESHOLD+1)*(CURVATURE_THRESHOLD+1)/CURVATURE_THRESHOLD;
//
//	// Detect extrema in the DoG images
//	for(i=0;i<m_octavesCount;i++)
//	{
//		scale = (int)pow(2.0, (double)i);
//
//		for(j=1;j<m_intervalsCount+1;j++)
//		{
//			// Allocate memory and set all points to zero ("not key point")
//			m_extrema[i][j-1] = cvCreateImage(cvGetSize(m_dogs[i][0]), 8, 1);
//			cvZero(m_extrema[i][j-1]);
//
//			// Images just above and below, in the current octave
//			middle = m_dogs[i][j];
//			up = m_dogs[i][j+1];
//			down = m_dogs[i][j-1];
//
//			for(xi=1;xi<m_dogs[i][j]->width-1;xi++)
//			{
//				for(yi=1;yi<m_dogs[i][j]->height-1;yi++)
//				{
//					// true if a keypoint is a maxima/minima
//					// but needs to be tested for contrast/edge thingy
//					bool justSet = false;
//
//					double currentPixel = cvGetReal2D(middle, yi, xi);
//
//					// Check for a maximum
//					if (currentPixel > cvGetReal2D(middle, yi-1, xi  )	&&
//                        currentPixel > cvGetReal2D(middle, yi+1, xi  )  &&
//                        currentPixel > cvGetReal2D(middle, yi  , xi-1)  &&
//                        currentPixel > cvGetReal2D(middle, yi  , xi+1)  &&
//                        currentPixel > cvGetReal2D(middle, yi-1, xi-1)	&&
//                        currentPixel > cvGetReal2D(middle, yi-1, xi+1)	&&
//                        currentPixel > cvGetReal2D(middle, yi+1, xi+1)	&&
//                        currentPixel > cvGetReal2D(middle, yi+1, xi-1)	&&
//                        currentPixel > cvGetReal2D(up, yi  , xi  )      &&
//                        currentPixel > cvGetReal2D(up, yi-1, xi  )      &&
//                        currentPixel > cvGetReal2D(up, yi+1, xi  )      &&
//                        currentPixel > cvGetReal2D(up, yi  , xi-1)      &&
//                        currentPixel > cvGetReal2D(up, yi  , xi+1)      &&
//                        currentPixel > cvGetReal2D(up, yi-1, xi-1)		&&
//                        currentPixel > cvGetReal2D(up, yi-1, xi+1)		&&
//                        currentPixel > cvGetReal2D(up, yi+1, xi+1)		&&
//                        currentPixel > cvGetReal2D(up, yi+1, xi-1)		&&
//                        currentPixel > cvGetReal2D(down, yi  , xi  )    &&
//                        currentPixel > cvGetReal2D(down, yi-1, xi  )    &&
//                        currentPixel > cvGetReal2D(down, yi+1, xi  )    &&
//                        currentPixel > cvGetReal2D(down, yi  , xi-1)    &&
//                        currentPixel > cvGetReal2D(down, yi  , xi+1)    &&
//                        currentPixel > cvGetReal2D(down, yi-1, xi-1)	&&
//                        currentPixel > cvGetReal2D(down, yi-1, xi+1)	&&
//                        currentPixel > cvGetReal2D(down, yi+1, xi+1)	&&
//                        currentPixel > cvGetReal2D(down, yi+1, xi-1)   )
//					{
//						cvSetReal2D(m_extrema[i][j-1], yi, xi, 255);
//						num++;
//						justSet = true;
//					}
//					// Check if it's a minimum
//					else if (currentPixel < cvGetReal2D(middle, yi-1, xi  )	&&
//                        currentPixel < cvGetReal2D(middle, yi+1, xi  )  &&
//                        currentPixel < cvGetReal2D(middle, yi  , xi-1)  &&
//                        currentPixel < cvGetReal2D(middle, yi  , xi+1)  &&
//                        currentPixel < cvGetReal2D(middle, yi-1, xi-1)	&&
//                        currentPixel < cvGetReal2D(middle, yi-1, xi+1)	&&
//                        currentPixel < cvGetReal2D(middle, yi+1, xi+1)	&&
//                        currentPixel < cvGetReal2D(middle, yi+1, xi-1)	&&
//                        currentPixel < cvGetReal2D(up, yi  , xi  )      &&
//                        currentPixel < cvGetReal2D(up, yi-1, xi  )      &&
//                        currentPixel < cvGetReal2D(up, yi+1, xi  )      &&
//                        currentPixel < cvGetReal2D(up, yi  , xi-1)      &&
//                        currentPixel < cvGetReal2D(up, yi  , xi+1)      &&
//                        currentPixel < cvGetReal2D(up, yi-1, xi-1)		&&
//                        currentPixel < cvGetReal2D(up, yi-1, xi+1)		&&
//                        currentPixel < cvGetReal2D(up, yi+1, xi+1)		&&
//                        currentPixel < cvGetReal2D(up, yi+1, xi-1)		&&
//                        currentPixel < cvGetReal2D(down, yi  , xi  )    &&
//                        currentPixel < cvGetReal2D(down, yi-1, xi  )    &&
//                        currentPixel < cvGetReal2D(down, yi+1, xi  )    &&
//                        currentPixel < cvGetReal2D(down, yi  , xi-1)    &&
//                        currentPixel < cvGetReal2D(down, yi  , xi+1)    &&
//                        currentPixel < cvGetReal2D(down, yi-1, xi-1)	&&
//                        currentPixel < cvGetReal2D(down, yi-1, xi+1)	&&
//                        currentPixel < cvGetReal2D(down, yi+1, xi+1)	&&
//                        currentPixel < cvGetReal2D(down, yi+1, xi-1)   )
//					{
//						cvSetReal2D(m_extrema[i][j-1], yi, xi, 255);
//						num++;
//						justSet = true;
//					}
//
//					// The contrast check
//					if(justSet && fabs(cvGetReal2D(middle, yi, xi)) < CONTRAST_THRESHOLD)
//					{
//						cvSetReal2D(m_extrema[i][j-1], yi, xi, 0);
//						num--;
//						numRemoved++;
//
//						justSet=false;
//					}
//
//					// The edge check
//					if(justSet)
//					{
//						dxx = (cvGetReal2D(middle, yi-1, xi) +
//							  cvGetReal2D(middle, yi+1, xi) -
//							  2.0*cvGetReal2D(middle, yi, xi));
//
//						dyy = (cvGetReal2D(middle, yi, xi-1) +
//							  cvGetReal2D(middle, yi, xi+1) -
//							  2.0*cvGetReal2D(middle, yi, xi));
//
//						dxy = (cvGetReal2D(middle, yi-1, xi-1) +
//							  cvGetReal2D(middle, yi+1, xi+1) -
//							  cvGetReal2D(middle, yi+1, xi-1) -
//							  cvGetReal2D(middle, yi-1, xi+1)) / 4.0;
//
//						trH = dxx + dyy;
//						detH = dxx*dyy - dxy*dxy;
//
//						curvature_ratio = trH*trH/detH;
//						//printf("Threshold: %f - Ratio: %f\n", curvature_threshold, curvature_ratio);
//						if(detH<0 || curvature_ratio>curvature_threshold)
//						{
//							cvSetReal2D(m_extrema[i][j-1], yi, xi, 0);
//							num--;
//							numRemoved++;
//
//							justSet=false;
//						}
//					}
//				}
//			}
//
//			// Save the image
//			/*char* filename = new char[200];
//			sprintf(filename, "C:\\SIFT Test\\Extrema\\extrema_oct_%d_scale_%d.jpg", i, j-1);
//			cvSaveImage(filename, m_extrema[i][j-1]);*/
//		}
//	}
//
//	m_keypointsCount = num;
//	printf("Found %d keypoints\n", num);
//	printf("Rejected %d keypoints\n", numRemoved);
//}
//
//// For all the key points, generate an orientation.
//void SwiftItem::assignOrientations()
//{
//	printf("Assigning orientations...\n");
//	unsigned int i, j, k, xi, yi;
//	int kk, tt;
//
//	// These images hold the magnitude and direction of gradient
//	// for all blurred out images
//	IplImage*** magnitude = new IplImage**[m_octavesCount];
//	IplImage*** orientation = new IplImage**[m_octavesCount];
//
//	// Allocate some memory
//	for(i=0;i<m_octavesCount;i++)
//	{
//		magnitude[i] = new IplImage*[m_intervalsCount];
//		orientation[i] = new IplImage*[m_intervalsCount];
//	}
//
//	// These two loops are to calculate the magnitude and orientation of gradients
//	// through all octaces once and for all. We don't run around calculating things
//	// again and again that way.
//
//	// Iterate through all octaves
//	for(i=0;i<m_octavesCount;i++)
//	{
//		// Iterate through all scales
//		for(j=1;j<m_intervalsCount+1;j++)
//		{
//			magnitude[i][j-1] = cvCreateImage(cvGetSize(m_gaussians[i][j]), 32, 1);
//			orientation[i][j-1] = cvCreateImage(cvGetSize(m_gaussians[i][j]), 32, 1);
//
//			cvZero(magnitude[i][j-1]);
//			cvZero(orientation[i][j-1]);
//
//			// Iterate over the gaussian image with the current octave and interval
//			for(xi=1;xi<m_gaussians[i][j]->width-1;xi++)
//			{
//				for(yi=1;yi<m_gaussians[i][j]->height-1;yi++)
//				{
//					// Calculate gradient
//					double dx = cvGetReal2D(m_gaussians[i][j], yi, xi+1) - cvGetReal2D(m_gaussians[i][j], yi, xi-1);
//					double dy = cvGetReal2D(m_gaussians[i][j], yi+1, xi) - cvGetReal2D(m_gaussians[i][j], yi-1, xi);
//
//					// Store magnitude
//					cvSetReal2D(magnitude[i][j-1], yi, xi, sqrt(dx*dx + dy*dy));
//
//					// Store orientation as radians
//					double ori=atan(dy/dx);
//					cvSet2D(orientation[i][j-1], yi, xi, cvScalar(ori));
//				}
//			}
//
//			// Save these images for fun
//			/*char* filename = new char[200];
//			sprintf(filename, "C:\\SIFT Test\\Mag\\mag_oct_%d_scl_%d.jpg", i, j-1);
//			cvSaveImage(filename, magnitude[i][j-1]);
//
//			sprintf(filename, "C:\\SIFT Test\\Ori\\ori_oct_%d_scl_%d.jpg", i, j-1);
//			cvSaveImage(filename, orientation[i][j-1]);*/
//		}
//	}
//
//	// The histogram with 8 bins
//	double* hist_orient = new double[NUM_BINS];
//
//	// Go through all octaves
//	for(i=0;i<m_octavesCount;i++)
//	{
//		// Store current scale, width and height
//		unsigned int scale = (unsigned int)pow(2.0, (double)i);
//		unsigned int width = m_gaussians[i][0]->width;
//		unsigned int height= m_gaussians[i][0]->height;
//
//		// Go through all intervals in the current scale
//		for(j=1;j<m_intervalsCount+1;j++)
//		{
//			double abs_sigma = m_sigmas[i][j];
//
//			// This is used for magnitudes
//			IplImage* imgWeight = cvCreateImage(cvSize(width, height), 32, 1);
//			cvSmooth(magnitude[i][j-1], imgWeight, CV_GAUSSIAN, 0, 0, 1.5*abs_sigma);
//
//			// Get the kernel size for the Guassian blur
//			int hfsz = GetKernelSize(1.5*abs_sigma)/2;
//
//			// Temporarily used to generate a mask of region used to calculate
//			// the orientations
//			IplImage* imgMask = cvCreateImage(cvSize(width, height), 8, 1);
//			cvZero(imgMask);
//			gpyr()
//			// Iterate through all points at this octave and interval
//			for(xi=0;xi<width;xi++)
//			{
//				for(yi=0;yi<height;yi++)
//				{
//					// We're at a keypoint
//					if(cvGetReal2D(m_extrema[i][j-1], yi, xi)!=0)
//					{
//						// Reset the histogram thingy
//						for(k=0;k<NUM_BINS;k++)
//							hist_orient[k]=0.0;
//
//						// Go through all pixels in the window around the extrema
//						for(kk=-hfsz;kk<=hfsz;kk++)
//						{
//							for(tt=-hfsz;tt<=hfsz;tt++)
//							{
//								// Ensure we're within the image
//								if(xi+kk<0 || xi+kk>=width || yi+tt<0 || yi+tt>=height)
//									continue;
//
// 								double sampleOrient = cvGetReal2D(orientation[i][j-1], yi+tt, xi+kk);
//
//								if(sampleOrient <=-M_PI || sampleOrient>M_PI)
//									printf("Bad Orientation: %f\n", sampleOrient);
//
//								sampleOrient+=M_PI;
//
//								// Convert to degrees
//								unsigned int sampleOrientDegrees = sampleOrient * 180 / M_PI;
//								hist_orient[(int)sampleOrientDegrees / (360/NUM_BINS)] += cvGetReal2D(imgWeight, yi+tt, xi+kk);
//								cvSetReal2D(imgMask, yi+tt, xi+kk, 255);
//							}
//						}
//
//						// We've computed the histogram. Now check for the maximum
//						double max_peak = hist_orient[0];
//						unsigned int max_peak_index = 0;
//						for(k=1;k<NUM_BINS;k++)
//						{
//							if(hist_orient[k]>max_peak)
//							{
//								max_peak=hist_orient[k];
//								max_peak_index = k;
//							}
//						}
//
//						// List of magnitudes and orientations at the current extrema
//						vector<double> orien;
//						vector<double> mag;
//						for(k=0;k<NUM_BINS;k++)
//						{
//							// Do we have a good peak?
//							if(hist_orient[k]> 0.8*max_peak)
//							{
//								// Three points. (x2,y2) is the peak and (x1,y1)
//								// and (x3,y3) are the neigbours to the left and right.
//								// If the peak occurs at the extreme left, the "left
//								// neighbour" is equal to the right most. Similarly for
//								// the other case (peak is rightmost)
//								double x1 = k-1;
//								double y1;
//								double x2 = k;
//								double y2 = hist_orient[k];
//								double x3 = k+1;
//								double y3;
//
//								if(k==0)
//								{
//									y1 = hist_orient[NUM_BINS-1];
//									y3 = hist_orient[1];
//								}
//								else if(k==NUM_BINS-1)
//								{
//									y1 = hist_orient[NUM_BINS-1];
//									y3 = hist_orient[0];
//								}
//								else
//								{
//									y1 = hist_orient[k-1];
//									y3 = hist_orient[k+1];
//								}
//
//								// Next we fit a downward parabola aound
//								// these three points for better accuracy
//
//								// A downward parabola has the general form
//								//
//                                // y = a * x^2 + bx + c
//                                // Now the three equations stem from the three points
//                                // (x1,y1) (x2,y2) (x3.y3) are
//								//
//                                // y1 = a * x1^2 + b * x1 + c
//                                // y2 = a * x2^2 + b * x2 + c
//                                // y3 = a * x3^2 + b * x3 + c
//								//
//                                // in Matrix notation, this is y = Xb, where
//                                // y = (y1 y2 y3)' b = (a b c)' and
//                                //
//                                //     x1^2 x1 1
//                                // X = x2^2 x2 1
//                                //     x3^2 x3 1
//                                //
//                                // OK, we need to solve this equation for b
//                                // this is done by inverse the matrix X
//                                //
//                                // b = inv(X) Y
//
//								double *b = new double[3];
//								CvMat *X = cvCreateMat(3, 3, CV_32FC1);
//								CvMat *matInv = cvCreateMat(3, 3, CV_32FC1);
//
//								cvSetReal2D(X, 0, 0, x1*x1);
//								cvSetReal2D(X, 1, 0, x1);
//								cvSetReal2D(X, 2, 0, 1);
//
//								cvSetReal2D(X, 0, 1, x2*x2);
//								cvSetReal2D(X, 1, 1, x2);
//								cvSetReal2D(X, 2, 1, 1);
//
//								cvSetReal2D(X, 0, 2, x3*x3);
//								cvSetReal2D(X, 1, 2, x3);
//								cvSetReal2D(X, 2, 2, 1);
//
//								// Invert the matrix
//								cvInv(X, matInv);
//
//								b[0] = cvGetReal2D(matInv, 0, 0)*y1 + cvGetReal2D(matInv, 1, 0)*y2 + cvGetReal2D(matInv, 2, 0)*y3;
//								b[1] = cvGetReal2D(matInv, 0, 1)*y1 + cvGetReal2D(matInv, 1, 1)*y2 + cvGetReal2D(matInv, 2, 1)*y3;
//								b[2] = cvGetReal2D(matInv, 0, 2)*y1 + cvGetReal2D(matInv, 1, 2)*y2 + cvGetReal2D(matInv, 2, 2)*y3;
//
//								double x0 = -b[1]/(2*b[0]);
//
//								// Anomalous situation
//								if(fabs(x0)>2*NUM_BINS)
//									x0=x2;
//
//								while(x0<0)
//									x0 += NUM_BINS;
//								while(x0>= NUM_BINS)
//									x0-= NUM_BINS;
//
//								// Normalize it
//								double x0_n = x0*(2*M_PI/NUM_BINS);
//
//								assert(x0_n>=0 && x0_n<2*M_PI);
//								x0_n -= M_PI;
//								assert(x0_n>=-M_PI && x0_n<M_PI);
//
//								orien.push_back(x0_n);
//								mag.push_back(hist_orient[k]);
//							}
//						}
//
//						// Save this keypoint into the list
//						m_keypoints.push_back(Keypoint(xi*scale/2, yi*scale/2, mag, orien, i*m_intervalsCount+j-1));
//					}
//				}
//			}
//
//			// Save the regions!
//			/*char* filename = new char[200];
//			sprintf(filename, "C:\\SIFT Test\\Orientation Region\\ori_region_oct_%d_scl_%d.jpg", i, j-1);
//			cvSaveImage(filename, imgMask);*/
//			cvReleaseImage(&imgMask);
//		}
//	}
//
//	// Finally, we're done with all the magnitude and orientation images.
//	// Erase them from RAM
//	assert(m_keypoints.size() == m_keypointsCount);
//	for(i=0;i<m_octavesCount;i++)
//	{
//		for(j=1;j<m_intervalsCount+1;j++)
//		{
//			cvReleaseImage(&magnitude[i][j-1]);
//			cvReleaseImage(&orientation[i][j-1]);
//		}
//
//		delete [] magnitude[i];
//		delete [] orientation[i];
//	}
//
//	delete [] magnitude;
//	delete [] orientation;
//}
//
//// Generates a unique descriptor for each keypoint descriptor
//void SwiftItem::extractKeypointDescriptors()
//{
//	printf("Extract keypoint descriptors...\n");
//
//	// For loops
//	unsigned int i, j;
//
//	// Interpolated thingy. We're dealing with "inbetween" gradient
//	// magnitudes and orientations
//	IplImage*** imgInterpolatedMagnitude = new IplImage**[m_octavesCount];
//	IplImage*** imgInterpolatedOrientation = new IplImage**[m_octavesCount];
//	for(i=0;i<m_octavesCount;i++)
//	{
//		imgInterpolatedMagnitude[i] = new IplImage*[m_intervalsCount];
//		imgInterpolatedOrientation[i] = new IplImage*[m_intervalsCount];
//	}
//
//	// These two loops calculate the interpolated thingy for all octaves
//	// and subimages
//	for(i=0;i<m_octavesCount;i++)
//	{
//		for(j=1;j<m_intervalsCount+1;j++)
//		{
//			unsigned int width = m_gaussians[i][j]->width;
//			unsigned int height =m_gaussians[i][j]->height;
//
//			// Create an image and zero it out.
//			IplImage* imgTemp = cvCreateImage(cvSize(width*2, height*2), 32, 1);
//			cvZero(imgTemp);
//
//			// Scale it up. This will give us "access" to in betweens
//			cvPyrUp(m_gaussians[i][j], imgTemp);
//
//			// Allocate memory and zero them
//			imgInterpolatedMagnitude[i][j-1] = cvCreateImage(cvSize(width+1, height+1), 32, 1);
//			imgInterpolatedOrientation[i][j-1] = cvCreateImage(cvSize(width+1, height+1), 32, 1);
//			cvZero(imgInterpolatedMagnitude[i][j-1]);
//			cvZero(imgInterpolatedOrientation[i][j-1]);
//
//			// Do the calculations
//			for(float ii=1.5;ii<width-1.5;ii++)
//			{
//				for(float jj=1.5;jj<height-1.5;jj++)
//				{
//					// "inbetween" change
//					double dx = (cvGetReal2D(m_gaussians[i][j], jj, ii+1.5) + cvGetReal2D(m_gaussians[i][j], jj, ii+0.5))/2 - (cvGetReal2D(m_gaussians[i][j], jj, ii-1.5) + cvGetReal2D(m_gaussians[i][j], jj, ii-0.5))/2;
//					double dy = (cvGetReal2D(m_gaussians[i][j], jj+1.5, ii) + cvGetReal2D(m_gaussians[i][j], jj+0.5, ii))/2 - (cvGetReal2D(m_gaussians[i][j], jj-1.5, ii) + cvGetReal2D(m_gaussians[i][j], jj-0.5, ii))/2;
//
//					unsigned int iii = ii+1;
//					unsigned int jjj = jj+1;
//					assert(iii<=width && jjj<=height);
//
//					// Set the magnitude and orientation
//					cvSetReal2D(imgInterpolatedMagnitude[i][j-1], jjj, iii, sqrt(dx*dx + dy*dy));
//					cvSetReal2D(imgInterpolatedOrientation[i][j-1], jjj, iii, (atan2(dy,dx)==M_PI)? -M_PI:atan2(dy,dx) );
//				}
//			}
//
//			// Pad the edges with zeros
//			for(unsigned int iii=0;iii<width+1;iii++)
//			{
//				cvSetReal2D(imgInterpolatedMagnitude[i][j-1], 0, iii, 0);
//				cvSetReal2D(imgInterpolatedMagnitude[i][j-1], height, iii, 0);
//				cvSetReal2D(imgInterpolatedOrientation[i][j-1], 0, iii, 0);
//				cvSetReal2D(imgInterpolatedOrientation[i][j-1], height, iii, 0);
//			}
//
//			for(unsigned int jjj=0;jjj<height+1;jjj++)
//			{
//				cvSetReal2D(imgInterpolatedMagnitude[i][j-1], jjj, 0, 0);
//				cvSetReal2D(imgInterpolatedMagnitude[i][j-1], jjj, width, 0);
//				cvSetReal2D(imgInterpolatedOrientation[i][j-1], jjj, 0, 0);
//				cvSetReal2D(imgInterpolatedOrientation[i][j-1], jjj, width, 0);
//			}
//
//			// Now we have the imgInterpolated* ready. Store and get started
//			/*char* filename = new char[200];
//			sprintf(filename, "C:\\SIFT Test\\Interpolated Mag\\intmag_oct_%d_scl_%d.jpg", i, j-1);
//			cvSaveImage(filename, imgInterpolatedMagnitude[i][j-1]);
//
//			sprintf(filename, "C:\\SIFT Test\\Interpolated Ori\\intori_oct_%d_scl_%d.jpg", i, j-1);
//			cvSaveImage(filename, imgInterpolatedOrientation[i][j-1]);*/
//
//			cvReleaseImage(&imgTemp);
//
//		}
//	}
//
//	// Build an Interpolated Gaussian Table of size FEATURE_WINDOW_SIZE
//	// Lowe suggests sigma should be half the window size
//	// This is used to construct the "circular gaussian window" to weight
//	// magnitudes
//	CvMat *G = BuildInterpolatedGaussianTable(FEATURE_WINDOW_SIZE, 0.5*FEATURE_WINDOW_SIZE);
//
//	vector<double> hist(DESC_NUM_BINS);
//
//	// Loop over all keypoints
//	for(unsigned int ikp = 0;ikp<m_keypointsCount;ikp++)
//	{
//		unsigned int scale = m_keypoints[ikp].scale;
//		float kpxi = m_keypoints[ikp].xi;
//		float kpyi = m_keypoints[ikp].yi;
//
//		float descxi = kpxi;
//		float descyi = kpyi;
//
//		unsigned int ii = (unsigned int)(kpxi*2) / (unsigned int)(pow(2.0, (double)scale/m_intervalsCount));
//		unsigned int jj = (unsigned int)(kpyi*2) / (unsigned int)(pow(2.0, (double)scale/m_intervalsCount));
//
//		unsigned int width = m_gaussians[scale/m_intervalsCount][0]->width;
//		unsigned int height = m_gaussians[scale/m_intervalsCount][0]->height;
//
//		vector<double> orien = m_keypoints[ikp].orien;
//		vector<double> mag = m_keypoints[ikp].mag;
//
//		// Find the orientation and magnitude that have the maximum impact
//		// on the feature
//		double main_mag = mag[0];
//		double main_orien = orien[0];
//		for(unsigned int orient_count=1;orient_count<mag.size();orient_count++)
//		{
//			if(mag[orient_count]>main_mag)
//			{
//				main_orien = orien[orient_count];
//				main_mag = mag[orient_count];
//			}
//		}
//
//		unsigned int hfsz = FEATURE_WINDOW_SIZE/2;
//		CvMat *weight = cvCreateMat(FEATURE_WINDOW_SIZE, FEATURE_WINDOW_SIZE, CV_32FC1);
//		vector<double> fv(FVSIZE);
//
//		for(i=0;i<FEATURE_WINDOW_SIZE;i++)
//		{
//			for(j=0;j<FEATURE_WINDOW_SIZE;j++)
//			{
//				if(ii+i+1<hfsz || ii+i+1>width+hfsz || jj+j+1<hfsz || jj+j+1>height+hfsz)
//                    cvSetReal2D(weight, j, i, 0);
//				else
//					cvSetReal2D(weight, j, i, cvGetReal2D(G, j, i)*cvGetReal2D(imgInterpolatedMagnitude[scale/m_intervalsCount][scale%m_intervalsCount], jj+j+1-hfsz, ii+i+1-hfsz));
//			}
//		}
//
//		// Now that we've weighted the required magnitudes, we proceed to generating
//		// the feature vector
//
//		// The next two two loops are for splitting the 16x16 window
//		// into sixteen 4x4 blocks
//		for(i=0;i<FEATURE_WINDOW_SIZE/4;i++)			// 4x4 thingy
//		{
//			for(j=0;j<FEATURE_WINDOW_SIZE/4;j++)
//			{
//				// Clear the histograms
//				for(unsigned int t=0;t<DESC_NUM_BINS;t++)
//					hist[t]=0.0;
//
//				// Calculate the coordinates of the 4x4 block
//				int starti = (int)ii - (int)hfsz + 1 + (int)(hfsz/2*i);
//				int startj = (int)jj - (int)hfsz + 1 + (int)(hfsz/2*j);
//				int limiti = (int)ii + (int)(hfsz/2)*((int)(i)-1);
//				int limitj = (int)jj + (int)(hfsz/2)*((int)(j)-1);
//
//				// Go though this 4x4 block and do the thingy :D
//				for(int k=starti;k<=limiti;k++)
//				{
//					for(int t=startj;t<=limitj;t++)
//					{
//						if(k<0 || k>width || t<0 || t>height)
//							continue;
//
//						// This is where rotation invariance is done
//						double sample_orien = cvGetReal2D(imgInterpolatedOrientation[scale/m_intervalsCount][scale%m_intervalsCount], t, k);
//						sample_orien -= main_orien;
//
//						while(sample_orien<0)
//							sample_orien+=2*M_PI;
//
//						while(sample_orien>2*M_PI)
//							sample_orien-=2*M_PI;
//
//						// This should never happen
//						if(!(sample_orien>=0 && sample_orien<2*M_PI))
//							printf("BAD: %f\n", sample_orien);
//						assert(sample_orien>=0 && sample_orien<2*M_PI);
//
//						unsigned int sample_orien_d = sample_orien*180/M_PI;
//						assert(sample_orien_d<360);
//
//						unsigned int bin = sample_orien_d/(360/DESC_NUM_BINS);					// The bin
//						double bin_f = (double)sample_orien_d/(double)(360/DESC_NUM_BINS);		// The actual entry
//
//						assert(bin<DESC_NUM_BINS);
//						assert(k+hfsz-1-ii<FEATURE_WINDOW_SIZE && t+hfsz-1-jj<FEATURE_WINDOW_SIZE);
//
//						// Add to the bin
//						hist[bin]+=(1-fabs(bin_f-(bin+0.5))) * cvGetReal2D(weight, t+hfsz-1-jj, k+hfsz-1-ii);
//					}
//				}
//
//				// Keep adding these numbers to the feature vector
//				for(unsigned int t=0;t<DESC_NUM_BINS;t++)
//				{
//					fv[(i*FEATURE_WINDOW_SIZE/4+j)*DESC_NUM_BINS+t] = hist[t];
//				}
//			}
//		}
//
//		// Now, normalize the feature vector to ensure illumination independence
//		double norm=0;
//		for(unsigned int t=0;t<FVSIZE;t++)
//			norm+=pow(fv[t], 2.0);
//		norm = sqrt(norm);
//
//		for(unsigned int t=0;t<FVSIZE;t++)
//			fv[t]/=norm;
//
//		// Now, threshold the vector
//		for(unsigned int t=0;t<FVSIZE;t++)
//			if(fv[t]>FV_THRESHOLD)
//				fv[t] = FV_THRESHOLD;
//
//		// Normalize yet again
//		norm=0;
//		for(unsigned int t=0;t<FVSIZE;t++)
//			norm+=pow(fv[t], 2.0);
//		norm = sqrt(norm);
//
//		for(unsigned int t=0;t<FVSIZE;t++)
//			fv[t]/=norm;
//
//		// We're done with this descriptor. Store it into a list
//		m_keypointDescriptors.push_back(Descriptor(descxi, descyi, fv));
//	}
//
//	assert(m_keypointDescriptors.size()==m_keypointsCount);
//
//	// Get rid of memory we don't need anylonger
//	for(i=0;i<m_octavesCount;i++)
//	{
//		for(j=1;j<m_intervalsCount+1;j++)
//		{
//			cvReleaseImage(&imgInterpolatedMagnitude[i][j-1]);
//			cvReleaseImage(&imgInterpolatedOrientation[i][j-1]);
//		}
//
//		delete [] imgInterpolatedMagnitude[i];
//		delete [] imgInterpolatedOrientation[i];
//	}
//
//	delete [] imgInterpolatedMagnitude;
//	delete [] imgInterpolatedOrientation;
//}
//
//// Returns the size of the kernal for the Gaussian blur given the sigma and
//// cutoff value.
//unsigned int SIFT::GetKernelSize(double sigma, double cut_off)
//{
//    unsigned int i;
//    for (i=0;i<MAX_KERNEL_SIZE;i++)
//        if (exp(-((double)(i*i))/(2.0*sigma*sigma))<cut_off)
//            break;
//    unsigned int size = 2*i-1;
//    return size;
//}
//
//// This function actually generates the bell curve like image for the weighted
//// addition earlier.
//CvMat* SIFT::BuildInterpolatedGaussianTable(unsigned int size, double sigma)
//{
//	unsigned int i, j;
//	double half_kernel_size = size/2 - 0.5;
//
//	double sog=0;
//	CvMat* ret = cvCreateMat(size, size, CV_32FC1);
//
//	assert(size%2==0);
//
//	double temp=0;
//	for(i=0;i<size;i++)
//	{
//		for(j=0;j<size;j++)
//		{
//			temp = gaussian2D(i-half_kernel_size, j-half_kernel_size, sigma);
//			cvSetReal2D(ret, j, i, temp);
//			sog+=temp;
//		}
//	}
//
//	for(i=0;i<size;i++)
//	{
//		for(j=0;j<size;j++)
//		{
//			cvSetReal2D(ret, j, i, 1.0/sog * cvGetReal2D(ret, j, i));
//		}
//	}
//
//	return ret;
//}
//
//// Returns the value of the bell curve at a (x,y) for a given sigma
//double SIFT::gaussian2D(double x, double y, double sigma)
//{
//	double ret = 1.0/(2*M_PI*sigma*sigma) * exp(-(x*x+y*y)/(2.0*sigma*sigma));
//
//
//	return ret;
//}
//
//// Displays keypoints as calculated by the algorithm
//void SIFT::ShowKeypoints()
//{
//	IplImage* img = cvCloneImage(m_srcImage);
//
//	for(int i=0;i<m_keypointsCount;i++)
//	{
//		Keypoint kp = m_keyPoints[i];
//
//		cvLine(img, cvPoint(kp.xi, kp.yi), cvPoint(kp.xi, kp.yi), CV_RGB(255,255,255), 3);
//		cvLine(img, cvPoint(kp.xi, kp.yi), cvPoint(kp.xi+10*cos(kp.orien[0]), kp.yi+10*sin((double)kp.orien[0])), CV_RGB(255,255,255), 1);
//	}
//
//	cvNamedWindow("Keypoints");
//	cvShowImage("Keypoints", img);
//}
//
//// This function shows the sigmas used for various images.
//void SIFT::ShowAbsSigma()
//{
//	for(int i=0;i<m_octavesCount;i++)
//	{
//		for(int j=1;j<m_intervalsCount+4;j++)
//		{
//			printf("%f\t", m_sigmas[i][j-1]);
//		}
//		printf("\n");
//	}
//}

//////////////////////////////////////////////////////////////////////////