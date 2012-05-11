#include "stdafx.h"
#include "signrecognition.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	signrecognition w;
	w.show();
	return a.exec();
}
