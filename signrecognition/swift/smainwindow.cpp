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
	/// Ordered by relevance.
	filters << tr("All Files (*.*)")
		<< tr("JPEG Files (*.jpeg *.jpg *.jpe)")
		<< tr("Portable Network Graphics (*.png)")
		<< tr("TIFF Files (*.tiff *.tif)")
		<< tr("Windows Bitmaps (*.bmp *.dib)")
		<< tr("Portable Image Format (*.pbm *.pgm *.ppm)")
		<< tr("Sun Rasters (*.sr *.ras)");

	/// Creates QActions which represent specific user commands
	createActions();

	/// Creates the Menu Bar (File, Edit, Help, ...
	createMenus();
	
	/// Outer Interface
	createToolBars();

	/// Inner Interface
	createDockWidgets();

	/// Central Widget - OpenCV Viewer
	opencvWidget = new SOpenCVWidget(this);
	setCentralWidget(opencvWidget);

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
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	/// "Edit" Menu
	editMenu = menuBar()->addMenu(tr("&Edit"));

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
void SMainWindow::createDockWidgets()
{

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



















void SMainWindow::open()
{
	QStringList pathsTestImages = QFileDialog::getOpenFileNames(
		this,
		tr("Open File(s)"),
		QDir::currentPath().append("/TestData"),
		filters.join(";;"),
		&filters[1],
		nullptr
	);
}


void SMainWindow::about()
{
	QMessageBox::about(this, tr("About Swift"),
		tr("This is <b>Swift</b> by Patrick Nierath and Tim B. Jagla."));
}
