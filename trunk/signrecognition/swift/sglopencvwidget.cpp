#include "StdAfx.h"
#include "sglopencvwidget.h"

/// \class SOpenCVWidget
/// \brief
///
///
SGLOpenCVWidget::SGLOpenCVWidget(QWidget *parent, QGLWidget *shared)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent, shared)
	, mGlPainter(this)
	, mGlThread(this)
{
	setAttribute(Qt::WA_PaintOutsidePaintEvent);
	setAttribute(Qt::WA_DeleteOnClose);
	//mSceneChanged = false;
	//mBgColor = QColor::fromRgb(150, 150, 150);

	//mOutH = 0;
	//mOutW = 0;
	//mImgratio = 4.0f/3.0f; // Default image ratio

	//mPosX = 0;
	//mPosY = 0;
}

SGLOpenCVWidget::~SGLOpenCVWidget()
{
	//makeCurrent();
	stopRendering();
}

void SGLOpenCVWidget::startRendering()
{
	mGlPainter.moveToThread(&mGlThread);
	connect(&mGlThread, SIGNAL(started()), &mGlPainter, SLOT(start()));
	mGlThread.start();
}

void SGLOpenCVWidget::stopRendering()
{
	mGlPainter.stop();
	mGlThread.wait();
}

void SGLOpenCVWidget::resizeEvent(QResizeEvent *event)
{
	mGlPainter.resizeViewport(event->size());
}

void SGLOpenCVWidget::paintEvent(QPaintEvent *)
{
	// Handled by GLPainter.
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void SGLOpenCVWidget::renderImage(const QImage& frame)
{
	mGlFrame = QGLWidget::convertToGLFormat(frame);
	this->updateGL();
}

void SGLOpenCVWidget::resizeGL(int width, int height)
{
	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, width, height);

	// Change to the projection matrix and set orthogonal projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SGLOpenCVWidget::paintGL() {
	glClear (GL_COLOR_BUFFER_BIT);
	glClearColor (0.0, 0.0, 0.0, 1.0);
	if (!mGlFrame.isNull()) {
		mGlFrame = mGlFrame.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mGlFrame.width(), mGlFrame.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mGlFrame.bits() );
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, mGlFrame.height());
		glTexCoord2f(0, 1); glVertex2f(0, 0);
		glTexCoord2f(1, 1); glVertex2f(mGlFrame.width(), 0);
		glTexCoord2f(1, 0); glVertex2f(mGlFrame.width(), mGlFrame.height());
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glFlush();
	}
}

void SGLOpenCVWidget::initializeGL()
{
	makeCurrent();
	qglClearColor(Qt::black);
	glShadeModel(GL_FLAT);
	updateGL();
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

//void SGLOpenCVWidget::resizeGL(int width, int height)
//{
	//makeCurrent();
	//glViewport(0, 0, (GLint)width, (GLint)height);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//glOrtho(0, width, 0, height, 0, 1); // To Draw image in the center of the area

	//glMatrixMode(GL_MODELVIEW);

	//// ---> Scaled Image Sizes
	//mOutH = width/mImgratio;
	//mOutW = width;

	//if(mOutH>height)
	//{
	//	mOutW = height*mImgratio;
	//	mOutH = height;
	//}

	//emit imageSizeChanged( mOutW, mOutH );
	//// < --- Scaled Image Sizes

	//mPosX = (width-mOutW)/2;
	//mPosY = (height-mOutH)/2;

	//mSceneChanged = true;

	//updateScene();
//}

//void SGLOpenCVWidget::updateScene()
//{
	//if( mSceneChanged && this->isVisible() )
	//	updateGL();
//}
//
//void SGLOpenCVWidget::paintGL()
//{
	//makeCurrent();

	//if( !mSceneChanged )
	//	return;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renderImage();

	//mSceneChanged = false;
//}
//
//void SGLOpenCVWidget::renderImage()
//{
	//makeCurrent();

	////glClearColor(1.0, 0.5, 0.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT);

	//if (!mRenderQtImg.isNull())
	//{
	//	glLoadIdentity();

	//	QImage image; // the image rendered

	//	glPushMatrix();
	//	{
	//		int imW = mRenderQtImg.width();
	//		int imH = mRenderQtImg.height();

	//		// The image is to be resized to fit the widget?
	//		if( imW != this->size().width() &&
	//			imH != this->size().height() )
	//		{
	//			image = mRenderQtImg.scaled( //this->size(),
	//				QSize(mOutW,mOutH),
	//				Qt::IgnoreAspectRatio,
	//				Qt::SmoothTransformation
	//				);

	//			//qDebug( QString( "Image size: (%1x%2)").arg(imW).arg(imH).toAscii() );
	//		}
	//		else
	//			image = mRenderQtImg;

	//		// --->Centering image in draw area

	//		glRasterPos2i( mPosX, mPosY );
	//		// < --- Centering image in draw area

	//		imW = image.width();
	//		imH = image.height();

	//		glDrawPixels( imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	//	}
	//	glPopMatrix();

	//	// end
	//	glFlush();
	//}
//}
//
//bool SGLOpenCVWidget::showImage( cv::Mat image )
//{
	//image.copyTo(mOrigImage);

	//mImgratio = (float)image.cols/(float)image.rows;

	//if( mOrigImage.channels() == 3)
	//	mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
	//	mOrigImage.cols, mOrigImage.rows,
	//	mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
	//else if( mOrigImage.channels() == 1)
	//	mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
	//	mOrigImage.cols, mOrigImage.rows,
	//	mOrigImage.step, QImage::Format_Indexed8);
	//else
	//	return false;

	//mRenderQtImg = QGLWidget::convertToGLFormat(mRenderQtImg);

	//mSceneChanged = true;

	//updateScene();

	//return true;
//}