/// \file smainwindow.h
///
/// 
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>

class QAction;
class QMenu;

class SMainWindow : public QMainWindow
{
	Q_OBJECT;

signals:


public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

public slots:


private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createDockWidgets();
	void createStatusBar();

	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;

private slots:
	void open();
	void about();
};

#endif // SMAINWINDOW_H