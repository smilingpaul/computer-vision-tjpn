#include "stdafx.h"
#include "smainwindow.h"

/// \class SMainWindow
/// \brief 
///
/// 
SMainWindow::SMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	/// Call for UI Designer generated content.
	/// At the moment this Project doesn't use the UI Designer because it is easier to code without.
	//Ui.setupUi(this);

	createActions();


	/// Header
	createMenus();
	
	/// Outer Interface
	createToolBars();

	/// Inner Interface
	createDockWidgets();


	/// \todo implementing the opencv data viewer for qt
	/// Main Window - Central Widget
	//opencvWidget = new SOpenCVWidget();
	//setCentralWidget(opencvWidget);

	/// Footer
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
	QMessageBox::about(this, tr("OPEN DIALOG"),
		tr("OPEN DIALOG PLACEHOLDER TEXT"));
}


void SMainWindow::about()
{
	QMessageBox::about(this, tr("About Swift"),
		tr("This is <b>Swift</b> by Patrick Nierath and Tim B. Jagla."));
}
