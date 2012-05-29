/// \file smainwindow.h
///
///
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtDeclarative/QtDeclarative>
#include "sglopencvwidget.h"

class SMainWindow : public QMainWindow
{
	Q_OBJECT;

public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

public slots:

private:
	void initialize();
	void createActions();
	void createMenus();
	void createToolBars();
	void createWidgets();
	void createStatusBar();

	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;
	/// view (windows)
	QAction *tabAct;
	QAction *tileAct;

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

	QMdiArea mMdiArea;
	QGLWidget mSharedGlWidget;

	//////////////////////////////////////////////////////////////////////////

	QStringList mAllImagePaths;
	QStringList mNewImagePaths;
	QStringList mFormatFilters;

private slots:
	void open();
	void about();

	void newThread();
	void killThread();

	void tabSubWindows();
	void tileSubWindows();

signals:
};

#endif // SMAINWINDOW_H