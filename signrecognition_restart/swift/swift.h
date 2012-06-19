#ifndef SWIFT_H
#define SWIFT_H

#include <QtGui/QMainWindow>
#include "ui_swift.h"

class swift : public QMainWindow
{
	Q_OBJECT

public:
	swift(QWidget *parent = 0, Qt::WFlags flags = 0);
	~swift();

private:
	Ui::swiftClass ui;
};

#endif // SWIFT_H
