#ifndef SIGNRECOGNITION_H
#define SIGNRECOGNITION_H

#include <QtGui/QMainWindow>
#include "ui_signrecognition.h"

class signrecognition : public QMainWindow
{
	Q_OBJECT

public:
	signrecognition(QWidget *parent = 0, Qt::WFlags flags = 0);
	~signrecognition();

private:
	Ui::signrecognitionClass ui;
};

#endif // SIGNRECOGNITION_H
