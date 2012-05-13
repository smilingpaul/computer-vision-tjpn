#ifndef SIGNRECOGNITION_H
#define SIGNRECOGNITION_H

#include <QtGui/QMainWindow>

class QAction;
class QMenu;
class QPlainTextEdit;

class signrecognition : public QMainWindow
{
	Q_OBJECT;

public:
	signrecognition(QWidget *parent = 0, Qt::WFlags flags = 0);
	~signrecognition();

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
	QAction *aboutQtAct;
};

#endif // SIGNRECOGNITION_H
