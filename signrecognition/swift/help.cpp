#include "StdAfx.h"
#include "help.h"

namespace Help
{
	/// from cv::Mat bgr(a)
	/// to QImage (a)rgb
	QImage Convert::cvmat2qimage(const cv::Mat& cvmat)
	{
		cv::Mat qtstylemat;
		QImage::Format format;

		std::vector<cv::Mat> channels; ///< bgr(a)
		std::vector<cv::Mat> neworder; ///< (a)rgb

		cv::split(cvmat,channels);

		switch (cvmat.channels())
		{
		case 3:
			/// source - http://permalink.gmane.org/gmane.comp.lib.opencv/37800
			cv::cvtColor(cvmat, qtstylemat, CV_BGR2RGB);
			format = QImage::Format_RGB888;
			break;
		case 4:
			neworder[0] = channels[3]; ///< insert a
			neworder[1] = channels[2]; ///< insert r
			neworder[2] = channels[1]; ///< insert g
			neworder[3] = channels[0]; ///< insert b
			cv::merge(channels,qtstylemat);
			format = QImage::Format_ARGB32;
			break;
		}

		return QImage((const unsigned char*)(qtstylemat.data), qtstylemat.cols, qtstylemat.rows, format);
	}

	/// source - http://permalink.gmane.org/gmane.comp.lib.opencv/37800
	/// from QImage (a)rgb
	/// to cv::Mat bgr(a)
	cv::Mat Convert::qimage2cvmat(const QImage& qimage)
	{
		cv::Mat qtstylemat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
		cv::Mat cvmat = cv::Mat(qtstylemat.rows, qtstylemat.cols, CV_8UC3 );
		int from_to[] = { 0,0,  1,1,  2,2 };
		cv::mixChannels( &qtstylemat, 1, &cvmat, 1, from_to, 3 );
		return cvmat;
	}

	//source - http://code.google.com/p/rtabmap/source/browse/branches/audio/ros-pkg/rtabmap/src/ImageViewQt.cpp?spec=svn514&r=514
	//QImage cvtCvMat2QImage(const cv::Mat & image)
	//{
	//	QImage qtemp;
	//	if(!image.empty() && image.depth() == CV_8U)
	//	{
	//		const unsigned char * data = image.data;
	//		qtemp = QImage(image.cols, image.rows, QImage::Format_RGB32);
	//		for(int y = 0; y < image.rows; ++y, data += image.cols*image.elemSize())
	//		{
	//			for(int x = 0; x < image.cols; ++x)
	//			{
	//				QRgb * p = ((QRgb*)qtemp.scanLine (y)) + x;
	//				*p = qRgb(data[x * image.channels()+2], data[x * image.channels()+1], data[x * image.channels()]);
	//			}
	//		}
	//	}
	//	else if(!image.empty() && image.depth() != CV_8U)
	//	{
	//		printf("Wrong image format, must be 8_bits\n");
	//	}
	//	return qtemp;
	//}

	//source - http://code.google.com/p/rtabmap/source/browse/branches/audio/ros-pkg/rtabmap/src/ImageViewQt.cpp?spec=svn514&r=514
	//cv::Mat cvtQImage2CvMat(const QImage & image)
	//{
	//	cv::Mat cvImage;
	//	if(!image.isNull() && image.depth() == 32 && image.format() == QImage::Format_RGB32)
	//	{
	//		// assume RGB (3 channels)
	//		int channels = 3;
	//		cvImage = cv::Mat(image.height(), image.width(), CV_8UC3);
	//		unsigned char * data = cvImage.data;
	//		const IplImage test = cvImage;
	//		printf("%d vs %d\n", cvImage.cols*cvImage.elemSize(), test.widthStep);
	//		for(int y = 0; y < image.height(); ++y, data+=cvImage.cols*cvImage.elemSize())
	//		{
	//			for(int x = 0; x < image.width(); ++x)
	//			{
	//				QRgb rgb = image.pixel(x, y);
	//				data[x * channels+2] = qRed(rgb); //r
	//				data[x * channels+1] = qGreen(rgb); //g
	//				data[x * channels] = qBlue(rgb); //b
	//			}
	//		}
	//	}
	//	else
	//	{
	//		printf("Failed to convert image : depth=%d(!=32) format=%d(!=%d)\n", image.depth(), image.format(), QImage::Format_RGB32);
	//	}
	//	return cvImage;
	//}

	//std::vector<KeyPoint> Convert::qvector2stdvector(const QVector<KeyPoint>& qvector)
	//{
	//	std::vector<KeyPoint> stdvector;
	//	return stdvector;
	//}

	//QVector<KeyPoint> Convert::stdvector2qvector(const std::vector<KeyPoint>& stdvector)
	//{
	//	QVector<KeyPoint> qvector;
	//	return qvector
	//}
}