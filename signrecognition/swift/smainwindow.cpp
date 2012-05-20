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
	filters << tr("All Files (*.*)")
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
	dockWidget_alpha_left = new QDockWidget(tr("Picture Manager"),this);
	dockWidget_alpha_left->setObjectName("dockWidget_alpha_left");
	dockWidget_alpha_left->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	dockWidgetContents_alpha_left = new QWidget();
	dockWidgetContents_alpha_left->setObjectName("dockWidgetContents_alpha_left");
	dockWidget_alpha_left->setWidget(dockWidgetContents_alpha_left);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_alpha_left);
	viewMenu->addAction(dockWidget_alpha_left->toggleViewAction());
	
	//////////////////////////////////////////////////////////////////////////
	/// Right Dock Widget
	dockWidget_beta_right = new QDockWidget(tr("Controls"),this);
	dockWidget_beta_right->setObjectName("dockWidget_beta_right");
	dockWidget_beta_right->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	dockWidgetContents_beta_right = new QWidget();
	dockWidgetContents_beta_right->setObjectName("dockWidgetContents_beta_right");
	dockWidget_beta_right->setWidget(dockWidgetContents_beta_right);
	this->addDockWidget(Qt::RightDockWidgetArea, dockWidget_beta_right);
	viewMenu->addAction(dockWidget_beta_right->toggleViewAction());

	//////////////////////////////////////////////////////////////////////////
	/// Grid Layout
	gridLayout = new QGridLayout(dockWidgetContents_alpha_left);
	gridLayout->setObjectName("gridLayout");
	gridLayout->layout()->SetMaximumSize;
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(0,3,0,0); ///< Left = Base 11 + Padding 4 

	/// Grid Layout - Upper Left
	label_left = new QLabel(dockWidgetContents_alpha_left);
	label_left->setObjectName("label_left");
	label_left->setText(tr("Explore"));
	label_left->setAlignment(Qt::AlignHCenter);
	gridLayout->addWidget(label_left, 0, 0, 1, 1);
	
	/// Grid Layout - Lower Left
	declarativeView_left = new QDeclarativeView(dockWidgetContents_alpha_left);
	declarativeView_left->setSource(QUrl::fromLocalFile("myqmlfile.qml"));
	declarativeView_left->setObjectName("declarativeView_left");
	declarativeView_left->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	gridLayout->addWidget(declarativeView_left, 1, 0, 1, 1);

	/// Grid Layout - Upper Right
	label_right = new QLabel(dockWidgetContents_alpha_left);
	label_right->setObjectName("label_right");
	label_right->setText(tr("Train"));
	label_right->setAlignment(Qt::AlignHCenter);
	gridLayout->addWidget(label_right, 0, 1, 1, 1);

	/// Grid Layout - Lower Right
	declarativeView_right = new QDeclarativeView(dockWidgetContents_alpha_left);
	declarativeView_right->setSource(QUrl::fromLocalFile("myqmlfile.qml"));
	declarativeView_right->setObjectName("declarativeView_right");
	declarativeView_right->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	gridLayout->addWidget(declarativeView_right, 1, 1, 1, 1);
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
	QStringList input = QFileDialog::getOpenFileNames(
		this,
		tr("Open File(s)"),
		QDir::currentPath().append("/TestData"),
		filters.join(";;"),
		&filters[1],
		nullptr
	);

	/// would have been enough if we didn't need the exact new paths.
	/// \code
	///	paths += input;
	///	paths.removeDuplicates();
	/// \endcode
	/// to avoid recalculation of icons & additional stuff the following
	/// is the only way to go

	/// wrong user input might cause duplicate file paths in the "input"
	/// string list. not exactly sure but we better be safe. we got the
	/// processing power :-D
	input.removeDuplicates();

	/// remove redundant paths from the new "input" string list.
	/// redundant are paths which are already elements of the "paths" 
	/// string list.
	for ( QStringList::Iterator it = input.begin(); it != input.end(); ++it ) 
	{
		if (paths.contains(*it)) ///< pointer-style iterator dereference
		{
			it = input.erase(it);
		}
	}

	paths += input; ///< add only the new input to the "paths" member

	/// add only the new input to list model
	/// 
	

}

void SMainWindow::about()
{
	QMessageBox::about(this, tr("About Swift"),
		tr("This is <b>Swift</b> by Patrick Nierath and Tim B. Jagla."));
}
