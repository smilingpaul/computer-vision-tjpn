#include "StdAfx.h"
#include "help.h"

namespace Help
{
	/// static utility methods
	/// http://permalink.gmane.org/gmane.comp.lib.opencv/37800
	/// cv::Mat bgr
	/// QImage rgb
	QImage Convert::cvmat2qimage(const cv::Mat& cvmat)
	{
		int c = cvmat.channels();
		cv::Mat qtmat;
		QImage::Format format; 

		std::vector<cv::Mat> channels; ///< bgr(a)
		std::vector<cv::Mat> neworder; ///< rgb(a)

		cv::split(cvmat,channels);

		switch (c)
		{
		case 3:
			cv::cvtColor(cvmat, qtmat, CV_BGR2RGB);
			format = QImage::Format_RGB888;
			break;
		case 4:
			neworder[0] = channels[2]; ///< insert r
			neworder[1] = channels[1]; ///< insert g
			neworder[2] = channels[0]; ///< insert b
			neworder[3] = channels[3]; ///< insert a
			cv::merge(channels,qtmat);
			format = QImage::Format_ARGB32;
			break;
		}
		
		return QImage((const unsigned char*)(qtmat.data), qtmat.cols, qtmat.rows, format);
	}

	cv::Mat Convert::qimage2cvmat(const QImage& qimage)
	{
		cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
		cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
		int from_to[] = { 0,0,  1,1,  2,2 };
		cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
		return mat2;
	}

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