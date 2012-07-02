/// \file smainwindow.h
///
///
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>

#include "imageitemtrain.h"
#include "imageitemexplore.h"

#include "featureprovider.h"
#include "sdynamictoolbox.h"

class QAction;
class QMenu;

class SMainWindow : public QMainWindow
{
	Q_OBJECT

signals:

public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

	QList<ImageItemExplore> mExploreItems;
	QList<ImageItemTrain> mTrainItems;

	cv::FeatureDetector* mDetector;
	cv::DescriptorExtractor* mExtractor;
	cv::DescriptorMatcher* mMatcher;
	cv::BOWKMeansTrainer *mBOWTrainer;
	cv::BOWImgDescriptorExtractor *mBOWExtractor;
	cv::BFMatcher *mBFMatcher;
	cv::Mat mVocabulary;

public slots:

private:
	// <NOT THE MOST BEAUTIFUL KIND OF CODE>
	//////////////////////////////////////////////////////////////////////////
	void loadExploreItems();
	void loadTrainItems();
	void trainItems();
	void matchItems();
	//////////////////////////////////////////////////////////////////////////
	void loadEvaluationDatabase(QString path);
	void evaluate();
	void computeBOW();
	//////////////////////////////////////////////////////////////////////////
	// </NOT THE MOST BEAUTIFUL KIND OF CODE>

	void initialize();
	void createActions();
	void createMenus();
	void createToolBars();
	void createWidgets();
	void createStatusBar();

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

	// beta widgets
	QFormLayout *betaFormLayout;

	QLabel *betaLabelDetector;
		QComboBox *betaComboBoxDetector;
	QLabel *betaLabelExtractor;
		QComboBox *betaComboBoxExtractor;

	SDynamicToolbox *betaDynamicToolbox;

	//////////////////////////////////////////////////////////////////////////

	FeatureProvider *mProvider;

	QStringList mNames;
	QStringList mOwnEvaluationData;
	QStringList mOfficialEvaluationData;

private slots:
	void open();
	void about();
};

#endif // SMAINWINDOW_H