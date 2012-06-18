#include "StdAfx.h"
#include "sglpainter.h"

SGLPainter::SGLPainter(QGLWidget *widget)
	: mPainter(widget)
	, mActive(true)
{
}

SGLPainter::~SGLPainter()
{
}

void SGLPainter::start()
{
	mPainter->makeCurrent();
	startTimer(20);
}

void SGLPainter::stop()
{
	QMutexLocker locker(&mMutex);
	mActive = false;
}

void SGLPainter::resizeViewport(const QSize &size)
{
	QMutexLocker locker(&mMutex);
	mViewportWidth = size.width();
	mViewportHeight = size.height();

	//initialize();
}

void SGLPainter::timerEvent(QTimerEvent *event)
{
	QMutexLocker locker(&mMutex);
	if (!mActive) {
		killTimer(event->timerId());
		QThread::currentThread()->quit();
		return;
	}

	//update();

	paint();
}

void SGLPainter::paint()
{
	QPainter p(mPainter); // p.begin() is called in the constructor
	p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	//draw the painter stuff
	p.translate(mViewportWidth/2 - (mViewportWidth/3)/2, mViewportHeight/2-(mViewportHeight/3)/2);
	p.fillRect(QRect(0, 0, mViewportWidth/3, mViewportHeight/3), Qt::darkBlue);
	//end drawing of painter stuff

	p.resetTransform();
	p.end();
}