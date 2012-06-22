/// \file smainwindow.h
///
/// 
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "imageitem.h"

class QAction;
class QMenu;

class SMainWindow : public QMainWindow
{
	Q_OBJECT;

signals:


public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

	//QList<ImageItem> mExploreList;
	//lists for images to explore and the train images (signs)
	QList<ImageItem> mExploreItems;
	QList<ImageItem> mTrainItems;

	cv::FeatureDetector* mDetector;
	cv::DescriptorExtractor* mExtractor;
	cv::DescriptorMatcher* mMatcher;


public slots:


private:
	void initialize();
	void createActions();
	void createMenus();
	void createToolBars();
	void createWidgets();
	void createStatusBar();

	void loadExploreItems();
	void loadTrainItems();
	//void trainItems(QList<ImageItem> &list, cv::DescriptorMatcher &matcher);
	void trainItems();
	

	QStringList mFormatFilters;
	QStringList mAllImagePaths;
	QStringList mNewImagePaths;






	//////////////////////////////////////////////////////////////////////////
	// QActions

	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;

	//////////////////////////////////////////////////////////////////////////
	// QMenus

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	//////////////////////////////////////////////////////////////////////////
	// QWidgets
	
	// dock area alpha
	QDockWidget *dockAlpha;
	QWidget *dockContentsAlpha;

	// dock area beta
	QDockWidget *dockBeta;
	QWidget *dockContentsBeta;

private slots:
	void open();
	void about();
};

#endif // SMAINWINDOW_H
