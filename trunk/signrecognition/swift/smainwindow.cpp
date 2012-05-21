#include "stdafx.h"
#include "smainwindow.h"

/// \class SMainWindow
/// \brief 
///
/// 
SMainWindow::SMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	/// QStringList of file format filters.
	/// Ordered by relevance. Only file formats supported by the OpenCV library.
	m_filters << tr("All Files (*.*)")
		<< tr("JPEG Files (*.jpeg *.jpg *.jpe)")
		<< tr("Portable Network Graphics (*.png)")
		<< tr("TIFF Files (*.tiff *.tif)")
		<< tr("Windows Bitmaps (*.bmp *.dib)")
		<< tr("Portable Image Format (*.pbm *.pgm *.ppm)")
		<< tr("Sun Rasters (*.sr *.ras)");

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
	opencvWidget = new SOpenCVWidget(this);
	opencvWidget->setObjectName("opencvWidget");
	this->setCentralWidget(opencvWidget);

	//////////////////////////////////////////////////////////////////////////
	/// Left Dock Widget
	dockAlpha = new QDockWidget(tr("Picture Manager"),this);
	dockAlpha->setObjectName("dockAlpha");
	dockAlpha->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

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
	declarativeViewExplore->setSource(QUrl::fromLocalFile("myqmlfile.qml"));
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
	declarativeViewTrain->setSource(QUrl::fromLocalFile("myqmlfile.qml"));
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
	m_newPaths = QFileDialog::getOpenFileNames(
		this,
		tr("Open File(s)"),
		QDir::currentPath().append("/TestData"),
		m_filters.join(";;"),
		&m_filters[1],
		nullptr
	);

	/// would have been enough if we didn't needed the exact new paths:
	/// \code
	///	paths += input;
	///	paths.removeDuplicates();
	/// \endcode
	/// to avoid recalculation of icons & additional stuff the following
	/// is the only way to go:

	/// wrong user input might cause duplicate file paths in the "input"
	/// string list. not exactly sure but we better be safe. we got the
	/// processing power :-D
	m_newPaths.removeDuplicates();

	/// remove redundant paths from the new "input" string list.
	/// redundant are paths which are already elements of the "paths" 
	/// string list.
	for ( QStringList::Iterator it = m_newPaths.begin(); it != m_newPaths.end(); ++it ) 
	{
		if (m_allPaths.contains(*it)) ///< pointer-style iterator dereference
		{
			it = m_newPaths.erase(it);
		}
	}

	m_allPaths += m_newPaths; ///< add the new input to the "paths" member
	

}

void SMainWindow::about()
{
	QMessageBox::about(this, tr("About Swift"),
		tr("This is <b>Swift</b> by Patrick Nierath and Tim B. Jagla."));
}
