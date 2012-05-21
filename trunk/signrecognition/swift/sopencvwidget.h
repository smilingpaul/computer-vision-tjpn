/// \file sopencvwidget.h
///
/// 
#ifndef SOPENCVWIDGET_H
#define SOPENCVWIDGET_H

#include <QGLWidget>
#include <OpenCV\cv.h>

class SOpenCVWidget : public QGLWidget
{
	Q_OBJECT

public:
	SOpenCVWidget(QWidget *parent);
	~SOpenCVWidget();

private:
	
};

#endif // SOPENCVWIDGET_H
