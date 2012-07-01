#include "stdafx.h"
#include "smainwindow.h"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

/// \class SMainWindow
/// \brief
///
///
SMainWindow::SMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mProvider(new FeatureProvider())
{
	initialize();

	/// Creates QActions which represent specific user commands
	createActions();

	/// Creates the Menu Bar (File, Edit, Help, ...)
	createMenus();

	/// Outer Interface
	createToolBars();

	/// Inner Interface
	createWidgets();

	/// Creates the Status Bar at the bottom of the Window
	createStatusBar();

	//open the image(s) we want to explore and find our templates in
	loadExploreItems();

	//open the images we want to train <-- Sign Templates
	loadTrainItems();

	cv::SIFT sift = cv::SIFT();
	//our Matcher
	mMatcher = new cv::FlannBasedMatcher(new cv::flann::CompositeIndexParams(), new cv::flann::SearchParams());
	//our Detector
	mDetector = new cv::SiftFeatureDetector(sift);
	//our Extractor
	mExtractor = new cv::SiftDescriptorExtractor(sift);

	for (int k = 0; k < mExploreItems.size(); k++)
	{
		mExploreItems[k].precalculation(*mDetector, *mExtractor);
	}

	for (int i = 0; i < mTrainItems.size(); i++)
	{
		mTrainItems[i].precalculation(*mDetector, *mExtractor);
	}

	trainItems();

	//Matching descriptor vectors using FLANN matcher
	std::vector<cv::DMatch> matches;
	//mMatcher->match(mDescriptors(mExploreItems), mDescriptors(mTrainItems), matches);
	mMatcher->match(mExploreItems[0].getDescriptors(), matches);

	double max_dist = 0; double min_dist = 100;

	// Quick calculation of max and min distances between keypoints
	for( int i = 0; i < mExploreItems[0].getDescriptors().rows; i++ )
	 { double dist = matches[i].distance;
		 if( dist < min_dist ) min_dist = dist;
		 if( dist > max_dist ) max_dist = dist;
	 }

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  std::vector<cv::DMatch > good_matches;

  for( int i = 0; i < mExploreItems[0].getDescriptors().rows; i++ )
  { if( matches[i].distance < 2*min_dist )
    { good_matches.push_back( matches[i]); }
  }

/*	cv::Mat img_matches;
	cv::drawMatches(
		 mExploreItems[0].getImage()
		,mExploreItems[0].getKeyPoints()
		,mTrainItems[0].getImage()
		,mTrainItems[0].getKeyPoints()
		,good_matches
		,img_matches
		,cv::Scalar::all(-1)
		,cv::Scalar::all(-1)
		,std::vector<char>()
		, cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Show detected matches
    cv::imshow( "Good Matches", img_matches );
	*/
}

void SMainWindow::loadExploreItems()
{
	mExploreItems.append(ImageItem(QString("..\\swift-build\\TestData\\special_1.jpg")));
}

void SMainWindow::loadTrainItems()
{
	mTrainItems.append(ImageItem(QString("..\\swift-build\\TestData\\signs\\black_30.png")));
	mTrainItems.append(ImageItem(QString("..\\swift-build\\TestData\\signs\\black_stop.png")));
	mTrainItems.append(ImageItem(QString("..\\swift-build\\TestData\\signs\\black_yield.png")));
}

void SMainWindow::trainItems()
{
	for (unsigned int i = 0; i < mTrainItems.size(); i++)
	{
		mTrainItems[i].train(*mMatcher);
	}
}

void SMainWindow::initialize()
{
	/// QStringList of file format filters.
	/// Ordered by relevance. Only file formats supported by the OpenCV library.
	mFormatFilters << tr("All Files (*.*)")
		<< tr("JPEG Files (*.jpeg *.jpg *.jpe)")
		<< tr("Portable Network Graphics (*.png)")
		<< tr("TIFF Files (*.tiff *.tif)")
		<< tr("Windows Bitmaps (*.bmp *.dib)")
		<< tr("Portable Image Format (*.pbm *.pgm *.ppm)")
		<< tr("Sun Rasters (*.sr *.ras)");
}

/// Destructor of the main window
SMainWindow::~SMainWindow()
{
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::createActions()
{
	/// "Open" Action
	openAct = new QAction(tr("&Open\tCTRL+O"), this);
	openAct->setShortcut(QKeySequence::Open);
	openAct->setStatusTip(tr("Open Files"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	/// "Exit" Action
	exitAct = new QAction(tr("E&xit\tCTRL+Q"), this);
	exitAct->setShortcut(QKeySequence(tr("CTRL+Q")));
	exitAct->setStatusTip(tr("Exit Swift"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	/// "About" Action
	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("About Swift"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::createMenus()
{
	/// "File" Menu
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(exitAct);

	/// "Edit" Menu
	editMenu = menuBar()->addMenu(tr("&Edit"));

	/// "View" Menu
	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addSeparator();

	/// "Help" Menu
	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::createToolBars()
{
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::createWidgets()
{
	//////////////////////////////////////////////////////////////////////////
	/// Dock Widget Alpha
	dockAlpha = new QDockWidget(tr("Dock Alpha"),this);
	dockAlpha->setObjectName("dockAlpha");
	dockAlpha->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//dockAlpha->setFixedWidth(262); ///< 128px + 6px + 128px

	dockContentsAlpha = new QWidget();
	dockContentsAlpha->setObjectName("dockContentsAlpha");
	dockAlpha->setWidget(dockContentsAlpha);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockAlpha);
	viewMenu->addAction(dockAlpha->toggleViewAction());

	//////////////////////////////////////////////////////////////////////////
	/// Dock Widget Beta
	dockBeta = new QDockWidget(tr("Dock Beta"),this);
	dockBeta->setObjectName("dockBeta");
	dockBeta->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	dockContentsBeta = new QWidget();
	dockContentsBeta->setObjectName("dockContentsBeta");
	dockBeta->setWidget(dockContentsBeta);
	this->addDockWidget(Qt::RightDockWidgetArea, dockBeta);
	viewMenu->addAction(dockBeta->toggleViewAction());

	//////////////////////////////////////////////////////////////////////////
	/// Dock Beta

	/// Grid Layout
	betaFormLayout = new QFormLayout(dockContentsBeta);
	betaFormLayout->setObjectName("betaFormLayout");
	betaFormLayout->setSpacing(6);
	betaFormLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

	/// Label "Detector"
	betaLabelDetector = new QLabel(dockContentsBeta);
	betaLabelDetector->setObjectName("betaLabelDetector");
	betaLabelDetector->setText(tr("Detector"));

	/// ComboBox "Detector"
	betaComboBoxDetector = new QComboBox(dockContentsBeta);
	betaComboBoxDetector->setObjectName("betaComboBoxDetector");
	betaComboBoxDetector->addItems(mProvider->getDetectorList());
	connect(
		betaComboBoxDetector
		, SIGNAL(currentIndexChanged(betaComboBoxDetector->currentIndex()))
		, mProvider
		, SLOT(setCurrent(betaComboBoxDetector->currentIndex(),-1))
	);

	/// Label "Extractor"
	betaLabelExtractor = new QLabel(dockContentsBeta);
	betaLabelExtractor->setObjectName("betaLabelExtractor");
	betaLabelExtractor->setText(tr("Extractor"));

	/// ComboBox "Extractor"
	betaComboBoxExtractor = new QComboBox(dockContentsBeta);
	betaComboBoxExtractor->setObjectName("betaComboBoxExtractor");
	betaComboBoxExtractor->addItems(mProvider->getExtractorList());
	connect(
		betaComboBoxExtractor
		, SIGNAL(currentIndexChanged(betaComboBoxExtractor->currentIndex()))
		, mProvider
		, SLOT(setCurrent(-1,betaComboBoxExtractor->currentIndex()))
	);

	/// Form Layout Layout =)
	betaFormLayout->setWidget(0,QFormLayout::LabelRole,betaLabelDetector); ///< 0 Left
	betaFormLayout->setWidget(0,QFormLayout::FieldRole,betaComboBoxDetector); ///< 0 Right
	betaFormLayout->setWidget(1,QFormLayout::LabelRole,betaLabelExtractor); ///< 1 Left
	betaFormLayout->setWidget(1,QFormLayout::FieldRole,betaComboBoxExtractor); ///< 1 Right

	betaDynamicToolbox = new SDynamicToolbox(mProvider,dockContentsBeta);
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::open()
{
	 /// file open dialog to import images into the sign recognition software.
	 /// it supports a multitude of file formats (see "filters").
	mNewImagePaths = QFileDialog::getOpenFileNames(
		this,
		tr("Open File(s)"),
		QDir::currentPath().append("/TestData"),
		mFormatFilters.join(";;"),
		&mFormatFilters[0],
		nullptr
		);

	/// wrong user input might cause duplicate file paths in the "input"
	/// string list. not exactly sure but we better be safe. we got the
	/// processing power :-D
	mNewImagePaths.removeDuplicates();

	/// remove redundant paths from the new "input" string list.
	/// redundant are paths which are already elements of the "paths"
	/// string list.
	for ( QStringList::Iterator it = mNewImagePaths.begin(); it != mNewImagePaths.end(); ++it )
	{
		if (mAllImagePaths.contains(*it)) ///< pointer-style iterator dereference
		{
			it = mNewImagePaths.erase(it);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// LOADING OF THE FILES

	mAllImagePaths += mNewImagePaths; ///< add the new input to the "paths" member
}

/// \brief
///
///
///
/// \return Nothing
/// \sa
void SMainWindow::about()
{
	QMessageBox::about(this, tr("About Swift"),
		tr("This is <b>Swift</b> by Patrick Nierath and Tim B. Jagla."));
}