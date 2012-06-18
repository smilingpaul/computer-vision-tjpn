#ifndef SGLPAINTER_H
#define SGLPAINTER_H

#include <QObject>
#include <QGLWidget>

class SGLPainter : public QObject
{
	Q_OBJECT

public:
	SGLPainter(QGLWidget *widget);
	~SGLPainter();
	void stop();
	void resizeViewport(const QSize &size);

protected:
	void timerEvent(QTimerEvent *event);
	void paint();

public slots:
	void start();

private:
	QMutex mMutex;
	QGLWidget *mPainter;
	bool mActive;
	int mViewportWidth;
	int mViewportHeight;
};

#endif // SGLPAINTER_H