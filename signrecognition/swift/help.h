#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"

/// \file help.h
///
///
#ifndef HELP_H
#define HELP_H

namespace Help
{
	class Convert
	{
	public:
		static QImage cvmat2qimage(const cv::Mat& cvmat);
		static cv::Mat qimage2cvmat(const QImage& qimage);
		//static QMatrix cvmat2qmatrix(const cv::Mat& cvmat);
		//static cv::Mat qmatrix2cvmat(const QMatrix& qmatrix);
		//static std::vector<KeyPoint> qvector2stdvector(const QVector<KeyPoint>& qvector);
		//static QVector<KeyPoint> stdvector2qvector(const std::vector<KeyPoint>& stdvector);
	};
}

#endif // HELP_H