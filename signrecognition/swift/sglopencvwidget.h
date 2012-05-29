/// \file sopencvwidget.h
///
///
#ifndef SGLOPENCVWIDGET_H
#define SGLOPENCVWIDGET_H

#include <QGLWidget>
#include "sglpainter.h"

class SGLOpenCVWidget : public QGLWidget
{
	Q_OBJECT

//public slots:
	//bool showImage( cv::Mat image ); /// Used to set the image to be viewed

public:
	SGLOpenCVWidget(QWidget *parent, QGLWidget *shared = 0);
	~SGLOpenCVWidget();
	void startRendering();
	void stopRendering();

protected:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);
	QSize sizeHint() const { return QSize(256, 256); }

	void initializeGL(); /// OpenGL initialization
	void paintGL(); /// OpenGL Rendering
	void resizeGL(int width, int height); /// Widget Resize Event

	//void updateScene(); /// Forces a scene update
	//void renderImage(); /// Render image on openGL frame
	void renderImage(const QImage& frame);

private:
	SGLPainter mGlPainter;
	QThread mGlThread;
	//bool mSceneChanged; /// Indicates when OpenGL view is to be redrawn

	QImage mGlFrame; /// Qt image to be rendered
	//cv::Mat mOrigImage; /// original OpenCV image to be shown

	//QColor mBgColor; /// Background color

	//int mOutH; /// Resized Image height
	//int mOutW; /// Resized Image width
	//float mImgratio; /// height/width ratio

	//int mPosX; /// Top left X position to render image in the center of widget
	//int mPosY; /// Top left Y position to render image in the center of widget

//signals:
//	void imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget
};

#endif // SGLOPENCVWIDGET_H