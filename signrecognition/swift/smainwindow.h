/// \file smainwindow.h
///
///
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtDeclarative/QtDeclarative>
#include <QtDeclarative/QDeclarativeContext>
#include "sglopencvwidget.h"
#include "swiftmodel.h"

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

	//////////////////////////////////////////////////////////////////////////

	// actions
	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;
	QAction *tabAct;
	QAction *tileAct;

	// menus
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	// main window
	QMdiArea mMdiArea;
	QGLWidget mSharedGlWidget;

	// dock area right
	QDockWidget *dockAlpha;
	QWidget *dockContentsAlpha;

	// dock area left
	QDockWidget *dockBeta;
	QWidget *dockContentsBeta;

	QGridLayout *gridLayout;
	QLabel *labelExplore;
	QDeclarativeView *declarativeViewExplore;
	QLabel *labelTrain;
	QDeclarativeView *declarativeViewTrain;

	//////////////////////////////////////////////////////////////////////////

	SwiftModel mSwiftModel;

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