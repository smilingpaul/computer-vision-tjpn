/// \file main.cpp
///
/// Entry Point of Swift - Traffic Sign Recognition Software based on a Scale-Invariant-Feature-Transform Algorithm.

#include "stdafx.h"
#include "smainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication swift(argc, argv);
	SMainWindow window;

	swift.setApplicationVersion("0.0.1");

#ifdef _DEBUG
	swift.setApplicationName("Swift - DEBUG");
	window.setWindowTitle("Swift - DEBUG");
	window.show();
#else // _RELEASE
	swift.setApplicationName("Swift");
	window.setWindowTitle("Swift");
	window.showMaximized();
#endif

	return swift.exec();
}
