#include "stdafx.h"
#include "swift.h"
#include <QtGui/QApplication>
#include "smainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	SMainWindow mainWindow;

	app.setApplicationVersion("0.0.1");

#ifdef _DEBUG
	app.setApplicationName("Swift - DEBUG");
	mainWindow.setWindowTitle("Swift - DEBUG");
	mainWindow.setMinimumSize(800,600);
	mainWindow.show();
#else // _RELEASE
	app.setApplicationName("Swift");
	mainWindow.setWindowTitle("Swift");
	mainWindow.showMaximized();
#endif

	return app.exec();
}
