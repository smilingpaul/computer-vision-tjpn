#include "stdafx.h"
#include "swift.h"
#include <QtGui/QApplication>
#include "smainwindow.h"

/*
http://stackoverflow.com/questions/5461148/sift-implementation-with-opencv-2-2
http://www.copypastecode.com/71973/
http://stackoverflow.com/questions/5695034/how-to-use-flann-based-matcher-or-generally-flann-in-opencv
*/
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationVersion("0.0.1");
	app.setApplicationName("S.W.I.F.T");

	// Using QPainter to draw into QGLWidgets is only supported with GL 2.0
	if (!((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_0) ||
		  (QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_ES_Version_2_0)))
	{
			QMessageBox::information(0, "System Requirements",
				"OpenGL 2.0 or OpenGL ES 2.0 not available."
				"That's it, you are out!");
			return 0;
	}

	SMainWindow mainWindow;

#ifdef _DEBUG
	mainWindow.setWindowTitle("Swift - DEBUG");
	mainWindow.show();
#else // _RELEASE
	mainWindow.setWindowTitle("Swift - RELEASE");
	mainWindow.showMaximized();
#endif

	return app.exec();
}