#include "stdafx.h"
#include "smainwindow.h"
//#include "cqglwidgetopencv.h"


SMainWindow::SMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//opencvWidget = new CQGLWidgetOpenCV;
	//setCentralWidget(opencvWidget);
	// 

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
}

/// Destructor of the main window
SMainWindow::~SMainWindow()
{

}

void SMainWindow::createActions()
{
	/// "File" Menu
	exitAct = new QAction(tr("E&xit\tCTRL+Q"), this);
	exitAct->setShortcut(QKeySequence(tr("CTRL+Q")));
	exitAct->setStatusTip(tr("Exit Swift"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	/// "Help" Menu
	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("About Swift"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void SMainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
}

void SMainWindow::createToolBars()
{

}

void SMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}








void SMainWindow::about()
{
	QMessageBox::about(this, tr("About Swift"),
		tr("This is <b>Swift</b> by Patrick Nierath and Tim B. Jagla."));
}
