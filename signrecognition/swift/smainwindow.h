/// \file smainwindow.h
///
/// 
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtDeclarative/QtDeclarative>
#include "sopencvwidget.h"

class SMainWindow : public QMainWindow
{
	Q_OBJECT;

public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

public slots:


private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createWidgets();
	void createStatusBar();

	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	QDockWidget *dockAlpha;
	QWidget *dockContentsAlpha;

	QDockWidget *dockBeta;
	QWidget *dockContentsBeta;
	
	QGridLayout *gridLayout;
	QLabel *labelExplore;
	QDeclarativeView *declarativeViewExplore;
	QLabel *labelTrain;
	QDeclarativeView *declarativeViewTrain;

	SOpenCVWidget *opencvWidget;

	//////////////////////////////////////////////////////////////////////////

	QStringList m_allPaths;
	QStringList m_newPaths;
	QStringList m_filters;

private slots:
	void open();
	void about();

signals:

};

#endif // SMAINWINDOW_H
