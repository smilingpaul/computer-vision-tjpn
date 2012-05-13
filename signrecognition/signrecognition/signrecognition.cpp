#include "stdafx.h"
#include "signrecognition.h"
//#include "cqglwidgetopencv.h"


signrecognition::signrecognition(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//opencvWidget = new CQGLWidgetOpenCV;
	//setCentralWidget(opencvWidget);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
}

/// Destructor of the signrecognition QMainWindow
signrecognition::~signrecognition()
{

}

void signrecognition::createActions()
{
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));





	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void signrecognition::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void signrecognition::createToolBars()
{

}

void signrecognition::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}








void signrecognition::about()
{
	QMessageBox::about(this, tr("About Application"),
		tr("This is <b>SIFT Traffic Sign Recognition</b> by Patrick Nierath and Tim B. Jagla."));
}
