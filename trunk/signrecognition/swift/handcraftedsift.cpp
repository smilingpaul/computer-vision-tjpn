#include "StdAfx.h"
#include "handcraftedsift.h"
#include <opencv2\core\mat.hpp>
#include <math.h>

/// Handcrafted SIFT
//////////////////////////////////////////////////////////////////////////

HandcraftedSIFT::HandcraftedSIFT(void)
{
}

HandcraftedSIFT::~HandcraftedSIFT(void)
{
	unsigned int i, j;

	for( i = 0; i < m_octaveLayers; i++ )
	{
		// release all mats in that particular octave
/*		for( j = 0; j < m_octaveLayers + 3; j++ )
		{
			~*m_gaussians[i][j];
		}*/

		for( j = 0; j < m_octaveLayers + 2; j++ )
		{
			~*dogPyr[j];
		}

		for( j = 0; j < m_octaveLayers; j++ )		//should maybe changed...
		{
			//~*m_extrema[i][j];
			~*m_extrema[j];
		}

		// delete the particular octave
//		delete [] m_gaussians[i];
		delete [] dogPyr[i];
		delete [] m_extrema[i];
		delete [] m_sigmas[i];
	}

	// delete the arrays
//	delete [] m_gaussians;
	delete [] dogPyr;
	delete [] m_extrema;
	delete [] m_sigmas;
}

/// fantastic port
void HandcraftedSIFT::sift(int octavesCount /* = 4 */, int intervalsCount /* = 5 */)
{
	//not needed
	//m_octaveLayers   = octavesCount;
	//m_octaveLayers = intervalsCount;

	allocateMemory();
	createInitImg(m_image);
	//buildScaleSpaces(m_image, m_pyr, m_octaveLayers);
	//buildDoGPyramid(m_pyr, );
	detectExtrema();
	//assignOrientations();
	//extractKeypointDescriptors();

	
}

//CreateInitImg()
//need to double size and convert colored image in gray image
cv::Mat HandcraftedSIFT::createInitImg(cv::Mat& m_image)
	{
		cv::Mat grayImage, doubleSizeImage;

		if (m_image.channels() == 3 || m_image.channels() == 4)
			cvtColor(m_image, grayImage, CV_BGR2GRAY);
		
		else m_image.copyTo(grayImage);

	
		//now blur image and double its size
     float sig_diff;
	 
        sig_diff = sqrtf( (SIGMA * SIGMA - SIGMA_ANTIALIAS * SIGMA_ANTIALIAS * 4, 0.01f) );	//cv::max() funkioniert nicht->warum?
        resize(grayImage, doubleSizeImage, cv::Size(grayImage.cols*2, grayImage.rows*2), 0, 0, cv::INTER_LINEAR);
        GaussianBlur(doubleSizeImage, doubleSizeImage, cv::Size(), sig_diff, sig_diff);
        return doubleSizeImage;

	}

void HandcraftedSIFT::buildScaleSpaces(const cv::Mat& base, cv::vector<cv::Mat>& m_pyr, int m_octaveLayers ) const
{
	printf("Generating scale space...\n");

	cv::vector<double> m_sigmas(m_octaveLayers + 3);
	m_pyr.resize(m_octaveLayers*(m_octaveLayers + 3));

	// precompute Gaussian sigmas using the following formula:
    //  \sigma_{total}^2 = \sigma_{i}^2 + \sigma_{i-1}^2
	
	m_sigmas[0] = sigma;
	double k = pow(2., 1./ m_octaveLayers);

	for ( int i = 1; i < m_octaveLayers + 3; i++)
	{
		double sig_prev = pow(k, (double)(i-1))*sigma;
		double sig_total = sig_prev*k;
		m_sigmas[i] = cv::sqrt(sig_total - sig_prev*sig_prev);
	}

	for ( int o = 0; o < m_octaveLayers + 3; o++)
	{
		for ( int i = 0; i < m_octaveLayers + 3; i++)
		{
			cv::Mat& dest = m_pyr[o*(m_octaveLayers + 3) + i];
			if ( o == 0 && i == 0 )
				dest = base;
			//base of new octave is halved image from end of prevoius octave
			else if ( i == 0 )
			{
				const cv::Mat& src = m_pyr[(o-1)*(m_octaveLayers + 3) + m_octaveLayers];
				resize(src, dest, cv::Size(src.cols/2, src.rows/2), 
					0, 0, cv::INTER_NEAREST);		//INTER_NEAREST - nearest neighbour interpolation
			}
			else
			{
				const cv::Mat& src = m_pyr[o*(m_octaveLayers + 3) + i-1];
				GaussianBlur(src, dest, cv::Size(), m_sigmas[i], m_sigmas[i]);
			}
		}
	}
}


void HandcraftedSIFT::buildDoGPyramid( const cv::vector<cv::Mat>& gaussPyr, cv::vector<cv::Mat>& dogPyr ) const
{
	printf("Generating DoG-Pyramid...\n");

    int nOctaves = (int)gaussPyr.size()/(m_octaveLayers + 3);
    dogPyr.resize( nOctaves*(m_octaveLayers + 2) );
    
    for( int o = 0; o < nOctaves; o++ )
    {
        for( int i = 0; i < m_octaveLayers + 2; i++ )
        {
            const cv::Mat& src1 = gaussPyr[o*(m_octaveLayers + 3) + i];
            const cv::Mat& src2 = gaussPyr[o*(m_octaveLayers + 3) + i + 1];
            cv::Mat& dest = dogPyr[o*(m_octaveLayers + 2) + i];
            subtract(src2, src1, dest, cv::noArray(), CV_16S);

			/*// Save the images generated for fun :)
			char* filename = new char[200];
			sprintf(filename, "C:\\Users\\Patrickson\\Documents\\Studium\\SS2012\\Computer Vision\\SIFT Test\\Gaussian\\g_octave_%d_scale_%d.jpg", o, i);
			SaveFloatingPointImage(filename, gaussPyr[o][i]);

			sprintf(filename, "C:\\Users\\Patrickson\\Documents\\Studium\\SS2012\\Computer Vision\\SIFT TEST\\DOG\\dog_octave_%d_scale_%d.jpg", o, i-1);
			SaveFloatingPointImage(filename, dogPyr[o][i-1]);*/
        }
    }
}


// find extrema in DoG in ONE octave
void HandcraftedSIFT::findScaleSpaceExtrema(const cv::vector<cv::Mat>& gaussPyr, const cv::vector<cv::Mat>& dogPyr,
                                  cv::vector<cv::KeyPoint>& keypoints ) const
{
	printf("Detecting extrema...\n");

//	int nOctaves = (int)gaussPyr.size()/(m_octaveLayers + 3);
//	int treshold = cvFloor(0.5 * CONTRAST_THRESHOLD/m_octaveLayers * 255 * SIFT_FIXPT_SCALE);
//	const int n = SIFT_ORI_HIST_BINS;
	
	cv::KeyPoint kpt;				//not used until now

	//looping variables
	unsigned int xi, yi, oct;		//xi,yi for one octave, oct for going through all octaves

	unsigned int num = 0;			//numeber of detected keypoints
//	double curvature_threshold = (CURVATURE_THRESHOLD+1)*(CURVATURE_THRESHOLD+1)/CURVATURE_THRESHOLD;

//	cv::Mat *middle, *up, *down;



	for(oct=1;oct<m_octaves+1;oct++)
	{
		//if right then NICE :)
		const cv::Mat middle = dogPyr[oct];		
		const cv::Mat up = dogPyr[oct-1];
		const cv::Mat down = dogPyr[oct+1];

		// Allocate memory and set all points to zero ("not key point")
		//m_extrema[i][j-1] = cvCreateImage(cvGetSize(m_dogList[i][0]), 8, 1);
		//cvCreateImage
		cv::Mat* tmp;
		tmp = new cv::Mat(dogPyr.size(), 8, CV_8UC3);	
		m_extrema[oct] = tmp;		//really not sure about that stuff
	
		//...void Mat::create(...)?

		//for one image of an octave
		for(xi=1;xi<middle.cols-1;xi++)		//check if .rows/.cols really give a (int)value back
			{
				for(yi=1;yi<middle.rows-1;yi++)
				{
					// true if a keypoint is a maxima/minima
					// but needs to be tested for contrast/edge thingy
					bool justSet = false;
					double currentPixel = middle.at<double>(yi, xi);
					//double currentPixel = cvGetReal2D(middle, yi, xi);		//to be changed and used below

					// Check for a maximum
					
					if (currentPixel > middle.at<double>( yi-1, xi  )		&&
						currentPixel > middle.at<double>( yi+1, xi  )		&&
						currentPixel > middle.at<double>( yi  , xi-1)		&&
						currentPixel > middle.at<double>( yi  , xi+1)		&&
						currentPixel > middle.at<double>( yi-1, xi-1)		&&
						currentPixel > middle.at<double>( yi-1, xi+1)		&&
						currentPixel > middle.at<double>( yi+1, xi+1)		&&
						currentPixel > middle.at<double>( yi+1, xi-1)		&&
						currentPixel > up.at<double>( yi	, xi  )		&&
						currentPixel > up.at<double>( yi-1	, xi  )		&&
						currentPixel > up.at<double>( yi+1	, xi  )		&&
						currentPixel > up.at<double>( yi	, xi-1)		&&
						currentPixel > up.at<double>( yi	, xi+1)		&&
						currentPixel > up.at<double>( yi-1	, xi-1)		&&
						currentPixel > up.at<double>( yi-1	, xi+1)		&&
						currentPixel > up.at<double>( yi+1	, xi-1)		&&
						currentPixel > up.at<double>( yi+1	, xi+1)		&&
						currentPixel > down.at<double>( yi   , xi   )		&&
						currentPixel > down.at<double>( yi-1 , xi   )		&&
						currentPixel > down.at<double>( yi+1 , xi   )		&&
						currentPixel > down.at<double>( yi   , xi-1 )		&&
						currentPixel > down.at<double>( yi   , xi+1 )		&&
						currentPixel > down.at<double>( yi-1 , xi-1 )		&&
						currentPixel > down.at<double>( yi-1 , xi+1 )		&&
						currentPixel > down.at<double>( yi+1 , xi-1 )		&&
						currentPixel > down.at<double>( yi+1 , xi+1 )		)
					{
						m_extrema[oct]->at<double>(yi, xi) = 255;
						//cvSetReal2D(m_extrema[i][j-1], yi, xi, 255);
						num++;
						justSet = true;
					}

					else if (currentPixel < middle.at<double>( yi-1, xi  )		&&
						currentPixel < middle.at<double>( yi+1, xi  )		&&
						currentPixel < middle.at<double>( yi  , xi-1)		&&
						currentPixel < middle.at<double>( yi  , xi+1)		&&
						currentPixel < middle.at<double>( yi-1, xi-1)		&&
						currentPixel < middle.at<double>( yi-1, xi+1)		&&
						currentPixel < middle.at<double>( yi+1, xi+1)		&&
						currentPixel < middle.at<double>( yi+1, xi-1)		&&
						currentPixel < up.at<double>( yi	, xi  )		&&
						currentPixel < up.at<double>( yi-1	, xi  )		&&
						currentPixel < up.at<double>( yi+1	, xi  )		&&
						currentPixel < up.at<double>( yi	, xi-1)		&&
						currentPixel < up.at<double>( yi	, xi+1)		&&
						currentPixel < up.at<double>( yi-1	, xi-1)		&&
						currentPixel < up.at<double>( yi-1	, xi+1)		&&
						currentPixel < up.at<double>( yi+1	, xi-1)		&&
						currentPixel < up.at<double>( yi+1	, xi+1)		&&
						currentPixel < down.at<double>( yi   , xi   )		&&
						currentPixel < down.at<double>( yi-1 , xi   )		&&
						currentPixel < down.at<double>( yi+1 , xi   )		&&
						currentPixel < down.at<double>( yi   , xi-1 )		&&
						currentPixel < down.at<double>( yi   , xi+1 )		&&
						currentPixel < down.at<double>( yi-1 , xi-1 )		&&
						currentPixel < down.at<double>( yi-1 , xi+1 )		&&
						currentPixel < down.at<double>( yi+1 , xi-1 )		&&
						currentPixel < down.at<double>( yi+1 , xi+1 )		)
					{
						m_extrema[oct]->at<double>(yi, xi) = 255;
						//cvSetReal2D(m_extrema[i][j-1], yi, xi, 255);
						num++;
						justSet = true;
					}
			}
		}
	}
}

// not needed
void HandcraftedSIFT::allocateMemory()
{
	//nicht fragmentiert
	//m_gaussians = new cv::Mat*[m_octaveLayers*(m_octaveLayers + 3)];

	m_pyr       = new cv::vector<cv::Mat>(m_octaveLayers);
		//cv::vector<cv::Mat>[m_octaveLayers];
	dogPyr      = new cv::vector<cv::Mat*>[m_octaveLayers];
	m_extrema   = new cv::Mat*[m_octaveLayers];
	m_sigmas    = new double*  [m_octaveLayers];

	for( unsigned int i = 0; i < m_octaveLayers; i++ )
	{
		/// why +3?
		m_pyr[i]	   = new cv::Mat[m_octaveLayers + 3];
		/// why +2?
		dogPyr[i]      = new cv::Mat[m_octaveLayers + 2];
		/// why +0?
		m_extrema[i]   = new cv::Mat[m_octaveLayers];
		/// why +3?
		m_sigmas[i]    = new double  [m_octaveLayers + 3];
	}
}
