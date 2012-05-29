/// \file swiftitem.h
///
///
///
#ifndef SWIFTITEM_H
#define SWIFTITEM_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

class SwiftItem
{
public:
	SwiftItem(const QString &path);
	~SwiftItem();

	QString path() const;
	QImage thumbnail() const;

private:
	QString mPath;
	QImage mThumbnail;

	cv::Mat mImage;

	void generateThumbnail();
};

#endif // MODEL_H