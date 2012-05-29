#include "StdAfx.h"
#include "help.h"

namespace Help
{
	/// static utility methods
	/// http://permalink.gmane.org/gmane.comp.lib.opencv/37800
	QImage Convert::cvmat2qimage(const cv::Mat& cvmat)
	{
		cv::Mat bgr;
		cv::cvtColor(cvmat, bgr, CV_BGR2RGB);
		return QImage((const unsigned char*)(bgr.data), bgr.cols, bgr.rows, QImage::Format_RGB888);
	}

	cv::Mat Convert::qimage2cvmat(const QImage& qimage)
	{
		cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
		cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
		int from_to[] = { 0,0,  1,1,  2,2 };
		cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
		return mat2;
	}

	//// TODO
	//QMatrix Convert::mat2qmatrix(const cv::Mat& mat)
	//{
	//	QMatrix qmatrix;
	//	return qmatrix;
	//}

	//cv::Mat Convert::qmatrix2mat(const QMatrix& qmatrix)
	//{
	//	cv::Mat mat;
	//	return mat;
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