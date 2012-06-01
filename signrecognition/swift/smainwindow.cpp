#include "stdafx.h"
#include "smainwindow.h"
#include "swiftitem.h"
#include "swiftmodel.h"

#define	MAX_THREADS 4
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/features2d/features2d.hpp>

/// \class SMainWindow
/// \brief
///
///
SMainWindow::SMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mMdiArea(this)
	, mSharedGlWidget(this)
{
	//#warning
	SwiftItem* mSwiftItem = new SwiftItem("C:\\Users\\Patrickson\\Documents\\Studium\\SS2012\\Computer Vision\\Projekt\\SVN\\signrecognition\\swift-build\\TestData\\50_distraction_1.jpg");

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
}

/// Destructor of the main window
SMainWindow::~SMainWindow()
{
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

	SwiftModel smodel = new SwiftModel(this);

	tabSubWindows();
	mMdiArea.setTabsClosable(true);
	mMdiArea.setTabsMovable(true);
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

	/// "Tab Windows" Action
	tabAct = new QAction(tr("Tab &Windows"), this);
	tabAct->setStatusTip(tr("Tab the windows"));
	connect(tabAct, SIGNAL(triggered()), this, SLOT(tabSubWindows()));

	/// "Tile Windows" Action
	tileAct = new QAction(tr("&Tile Windows"), this);
	tileAct->setStatusTip(tr("Tile the windows"));
	connect(tileAct, SIGNAL(triggered()), this, SLOT(tileSubWindows()));
	//connect(tileAct, SIGNAL(triggered()), &mMdiArea, SLOT(tileSubWindows()));
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
	viewMenu->addAction(tabAct);
	viewMenu->addAction(tileAct);
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
	/// Central Widget - OpenCV Viewer
	//opencvWidget->setObjectName("opencvWidget");
	this->setCentralWidget(&mMdiArea);
	this->mSharedGlWidget.resize(1, 1);
	//this->mSharedGlWidget.hide();

	for (unsigned int i = 0; i < MAX_THREADS; i++)
	{
		this->newThread();
	}

	// #warning
	// opencvWidget->showImage(cv::imread("D:\\STUDIUM\\Sem6\\ComputerVision\\svn\\signrecognition\\swift-build\\TestData\\50_distraction_1.jpg"));

	//////////////////////////////////////////////////////////////////////////
	/// Left Dock Widget
	dockAlpha = new QDockWidget(tr("Picture Manager"),this);
	dockAlpha->setObjectName("dockAlpha");
	dockAlpha->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockAlpha->setFixedWidth(262); ///< 128px + 6px + 128px

	dockContentsAlpha = new QWidget();
	dockContentsAlpha->setObjectName("dockContentsAlpha");
	dockAlpha->setWidget(dockContentsAlpha);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockAlpha);
	viewMenu->addAction(dockAlpha->toggleViewAction());

	//////////////////////////////////////////////////////////////////////////
	/// Right Dock Widget
	dockBeta = new QDockWidget(tr("Controls"),this);
	dockBeta->setObjectName("dockBeta");
	dockBeta->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	dockContentsBeta = new QWidget();
	dockContentsBeta->setObjectName("dockContentsBeta");
	dockBeta->setWidget(dockContentsBeta);
	this->addDockWidget(Qt::RightDockWidgetArea, dockBeta);
	viewMenu->addAction(dockBeta->toggleViewAction());

	//////////////////////////////////////////////////////////////////////////
	/// Grid Layout
	gridLayout = new QGridLayout(dockContentsAlpha);
	gridLayout->setObjectName("gridLayout");
	gridLayout->layout()->SetMaximumSize;
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(0,3,0,0); ///< 3px from top looks better

	/// Grid Layout - Upper Left
	labelExplore = new QLabel(dockContentsAlpha);
	labelExplore->setObjectName("labelExplore");
	labelExplore->setText(tr("Explore"));
	labelExplore->setAlignment(Qt::AlignHCenter);
	gridLayout->addWidget(labelExplore, 0, 0, 1, 1);

	/// Grid Layout - Lower Left
	declarativeViewExplore = new QDeclarativeView(dockContentsAlpha);
	declarativeViewExplore->setAttribute(Qt::WA_OpaquePaintEvent);
	declarativeViewExplore->setAttribute(Qt::WA_NoSystemBackground);
	declarativeViewExplore->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	declarativeViewExplore->viewport()->setAttribute(Qt::WA_NoSystemBackground);

	declarativeViewExplore->setSource(QUrl::fromLocalFile("thumbnailview.qml"));
	declarativeViewExplore->setObjectName("declarativeViewExplore");
	declarativeViewExplore->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	gridLayout->addWidget(declarativeViewExplore, 1, 0, 1, 1);

	/// Grid Layout - Upper Right
	labelTrain = new QLabel(dockContentsAlpha);
	labelTrain->setObjectName("labelTrain");
	labelTrain->setText(tr("Train"));
	labelTrain->setAlignment(Qt::AlignHCenter);
	gridLayout->addWidget(labelTrain, 0, 1, 1, 1);

	/// Grid Layout - Lower Right
	declarativeViewTrain = new QDeclarativeView(dockContentsAlpha);
	//declarativeViewTrain->setAttribute(Qt::WA_OpaquePaintEvent);
	//declarativeViewTrain->setAttribute(Qt::WA_NoSystemBackground);
	//declarativeViewTrain->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	//declarativeViewTrain->viewport()->setAttribute(Qt::WA_NoSystemBackground);

	declarativeViewTrain->setSource(QUrl::fromLocalFile("thumbnailview.qml"));
	declarativeViewTrain->setObjectName("declarativeViewTrain");
	declarativeViewTrain->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	gridLayout->addWidget(declarativeViewTrain, 1, 1, 1, 1);
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

	mAllImagePaths += mNewImagePaths; ///< add the new input to the "paths" member
}

void SMainWindow::newThread()
{
	static int windowCount = 1;

	if (mMdiArea.subWindowList().count() == MAX_THREADS)
	{
		QMessageBox::information(0, "No, no, no...",
		"Maximum Nr. of open documents reached"
		"Dont torture your computer!");
		return;
	}

	SGLOpenCVWidget *widget = new SGLOpenCVWidget(&mMdiArea, &mSharedGlWidget);
	mMdiArea.addSubWindow(widget);
	widget->setWindowTitle("Thread #" + QString::number(windowCount++));
	widget->show();
	widget->startRendering();
}

void SMainWindow::killThread()
{
	delete mMdiArea.activeSubWindow();
}

void SMainWindow::about()
{
	QMessageBox::about(this, tr("TMP"),
		tr("TMP"));
}

/// VIEW
///
void SMainWindow::tabSubWindows()
{
	mMdiArea.setViewMode(QMdiArea::TabbedView);
}

void SMainWindow::tileSubWindows()
{
	mMdiArea.setViewMode(QMdiArea::SubWindowView);
	mMdiArea.tileSubWindows();
}