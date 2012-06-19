#include "stdafx.h"
#include "smainwindow.h"
#include "swiftmodel.h"

#define	MAX_THREADS 4

/// \class SMainWindow
/// \brief
///
///
SMainWindow::SMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mMdiArea(this)
	, mSharedGlWidget(this)
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

	SwiftModel mSwiftModel = new SwiftModel(this);

	//train
	mAllImagePaths << "..\\swift-build\\TestData\\signs\\black_danger.png"
		<< "..\\swift-build\\TestData\\signs\\black_stop.png"
		<< "..\\swift-build\\TestData\\signs\\black_yield.png";
	//query
	mAllImagePaths << "..\\swift-build\\TestData\\danger_quantity_1.jpg";

	mSwiftModel.loadFiles(mAllImagePaths);

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
	/// Central Widget - Multi Document Interface Area
	this->setCentralWidget(&mMdiArea);
	this->mSharedGlWidget.resize(1, 1);
	//this->mSharedGlWidget.hide();

	for (unsigned int i = 0; i < MAX_THREADS; i++)
	{
		this->newThread();
	}

	//////////////////////////////////////////////////////////////////////////
	/// Dock Widget Alpha
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
	/// Dock Widget Beta
	dockBeta = new QDockWidget(tr("Controls"),this);
	dockBeta->setObjectName("dockBeta");
	dockBeta->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	dockContentsBeta = new QWidget();
	dockContentsBeta->setObjectName("dockContentsBeta");
	dockBeta->setWidget(dockContentsBeta);
	this->addDockWidget(Qt::RightDockWidgetArea, dockBeta);
	viewMenu->addAction(dockBeta->toggleViewAction());

	//////////////////////////////////////////////////////////////////////////
	/// Dock Alpha Grid Layout
	gridLayout = new QGridLayout(dockContentsAlpha);
	gridLayout->setObjectName("gridLayout");
	gridLayout->layout()->SetMaximumSize;
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(0,3,0,0); ///< 3px from top looks better

	/// Label "Explore"
	labelExplore = new QLabel(dockContentsAlpha);
	labelExplore->setObjectName("labelExplore");
	labelExplore->setText(tr("Explore"));
	labelExplore->setAlignment(Qt::AlignHCenter);

	/// Declarative View "Explore"
	declarativeViewExplore = new QDeclarativeView(dockContentsAlpha);
	//declarativeViewExplore->setAttribute(Qt::WA_OpaquePaintEvent);
	//declarativeViewExplore->setAttribute(Qt::WA_NoSystemBackground);
	//declarativeViewExplore->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	//declarativeViewExplore->viewport()->setAttribute(Qt::WA_NoSystemBackground);
	declarativeViewExplore->setResizeMode(QDeclarativeView::SizeRootObjectToView);
	declarativeViewExplore->rootContext()->setContextProperty("swiftModel",&mSwiftModel);
	declarativeViewExplore->setSource(QUrl::fromLocalFile("Main.qml"));
	declarativeViewExplore->setObjectName("declarativeViewExplore");
	declarativeViewExplore->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	/// Label "Train"
	labelTrain = new QLabel(dockContentsAlpha);
	labelTrain->setObjectName("labelTrain");
	labelTrain->setText(tr("Train"));
	labelTrain->setAlignment(Qt::AlignHCenter);

	/// Declarative View "Train"
	declarativeViewTrain = new QDeclarativeView(dockContentsAlpha);
	//declarativeViewTrain->setAttribute(Qt::WA_OpaquePaintEvent);
	//declarativeViewTrain->setAttribute(Qt::WA_NoSystemBackground);
	//declarativeViewTrain->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	//declarativeViewTrain->viewport()->setAttribute(Qt::WA_NoSystemBackground);
	declarativeViewTrain->setResizeMode(QDeclarativeView::SizeRootObjectToView);
	declarativeViewTrain->rootContext()->setContextProperty("swiftModel",&mSwiftModel);
	declarativeViewTrain->setSource(QUrl::fromLocalFile("Main.qml"));
	declarativeViewTrain->setObjectName("declarativeViewTrain");
	declarativeViewTrain->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	gridLayout->addWidget(labelExplore, 0, 0, 1, 1); ///< Upper Left
	gridLayout->addWidget(declarativeViewExplore, 1, 0, 1, 1); ///< Lower Left
	gridLayout->addWidget(labelTrain, 0, 1, 1, 1); ///< Upper Right
	gridLayout->addWidget(declarativeViewTrain, 1, 1, 1, 1); ///< Lower Right
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

	mSwiftModel.loadFiles(mNewImagePaths); ///< load all new and non duplicate images
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
void SMainWindow::tabSubWindows()
{
	//mMdiArea.hide();
	mMdiArea.setViewMode(QMdiArea::TabbedView);
	//mMdiArea.show();
}

void SMainWindow::tileSubWindows()
{
	//mMdiArea.hide();
	mMdiArea.setViewMode(QMdiArea::SubWindowView);
	mMdiArea.tileSubWindows();
	//mMdiArea.show();
}