#ifndef SIGNRECOGNITION_H
#define SIGNRECOGNITION_H

#include <QtGui/QMainWindow>

class QAction;
class QMenu;

class SMainWindow : public QMainWindow
{
	Q_OBJECT;

public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

private slots:
    void about();

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();

	QMenu *fileMenu;
	QAction *exitAct;

	QMenu *helpMenu;
	QAction *aboutAct;
};

#endif // SIGNRECOGNITION_H
