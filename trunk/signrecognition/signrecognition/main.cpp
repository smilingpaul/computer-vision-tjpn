#include "stdafx.h"
#include "signrecognition.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#ifdef _DEBUG
	a.setApplicationName("SIFT Sign Recognition - DEBUG");
#else // _RELEASE
	a.setApplicationName("SIFT Sign Recognition");
#endif

	a.setApplicationVersion("0.0.1");

	signrecognition w;

#ifdef _DEBUG
	w.show();
#else // _RELEASE
	w.showMaximized();
#endif

	return a.exec();
}
