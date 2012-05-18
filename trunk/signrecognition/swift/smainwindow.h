/// \file smainwindow.h
///
/// 
#ifndef SMAINWINDOW_H
#define SMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtDeclarative/QtDeclarative>
#include "sopencvwidget.h"

class SMainWindow : public QMainWindow
{
	Q_OBJECT;

signals:


public:
	SMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SMainWindow();

public slots:


private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createWidgets();
	void createStatusBar();

	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	QStringList paths;
	QStringList filters;

	SOpenCVWidget *opencvWidget;

	QDockWidget *dockWidget_beta_right;
	QWidget *dockWidgetContents_beta_right;
	QDockWidget *dockWidget_alpha_left;
	QWidget *dockWidgetContents_alpha_left;
	QWidget *gridLayoutWidget;
	QGridLayout *gridLayout;
	QDeclarativeView *declarativeView_left;
	QLabel *label_left;
	QDeclarativeView *declarativeView_right;
	QLabel *label_right;

private slots:
	void open();
	void about();
};

#endif // SMAINWINDOW_H
