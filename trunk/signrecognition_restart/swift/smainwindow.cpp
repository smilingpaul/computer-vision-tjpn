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

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	loadEvaluationDatabase("..\\swift-build\\TestData\\evaluation\\eval_database.txt");
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	cv::SIFT sift = cv::SIFT();
	mMatcher = new cv::FlannBasedMatcher();
	//mMatcher = new cv::BFMatcher(cv::NORM_L2,false);
	mDetector = new cv::SiftFeatureDetector(sift);
	mExtractor = new cv::SiftDescriptorExtractor(sift);
	mBOWTrainer = new cv::BOWKMeansTrainer(10);
	mBFMatcher = new cv::BFMatcher(cv::NORM_L2);

	for (int k = 0; k < mExploreItems.size(); k++)
	{
		mExploreItems[k].precalculation(*mDetector, *mExtractor);
	}

	for (int i = 0; i < mTrainItems.size(); i++)
	{
		mTrainItems[i].precalculation(*mDetector, *mExtractor);
	}

	trainItems();

	matchItems();

	computeBOW();

	for (unsigned int j = 0; j < mExploreItems.size(); j++)
	{
		for (unsigned int i = 0; i < mTrainItems.size(); i++)
		{
			cv::Mat img_matches;

			cv::drawMatches(
				mExploreItems[j].getImage()
				,mExploreItems[j].getKeyPoints()
				,mTrainItems[i].getImage()
				,mTrainItems[i].getKeyPoints()
				,mExploreItems[j].getGoodMatchesByIndex(i)
				,img_matches
				,cv::Scalar::all(-1)
				,cv::Scalar::all(-1)
				,std::vector<char>()
				//,cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS
				);

			//-- Show detected matches
			QString window = QString("Matches from Explore[%1] with Train[%2]").arg(j,i);
			std::string w = window.toStdString();
			cv::imshow( w , img_matches );
		}
	}

	for (unsigned int i = 0; i < mExploreItems.size(); i++)
		mExploreItems[i].evaluate();

	evaluate();
}

void SMainWindow::loadExploreItems()
{
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image01.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image02.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image03.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image04.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image05.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image06.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image07.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image08.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image09.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image10.png")));

	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image11.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image12.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image13.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image14.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image15.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image16.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image17.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image18.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image19.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image20.png")));

	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image21.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image22.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image23.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image24.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image25.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image26.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image27.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image28.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image29.png")));
	mExploreItems.append(ImageItemExplore(QString("..\\swift-build\\TestData\\evaluation\\image30.png")));
}

void SMainWindow::loadTrainItems()
{
	mTrainItems.append(ImageItemTrain(QString("..\\swift-build\\TestData\\signs\\black_stop.png")));
	mTrainItems.append(ImageItemTrain(QString("..\\swift-build\\TestData\\signs\\black_yield.png")));
	mTrainItems.append(ImageItemTrain(QString("..\\swift-build\\TestData\\signs\\black_30.png")));
	mTrainItems.append(ImageItemTrain(QString("..\\swift-build\\TestData\\signs\\black_60.png")));
	mTrainItems.append(ImageItemTrain(QString("..\\swift-build\\TestData\\signs\\black_120.png")));
	mTrainItems.append(ImageItemTrain(QString("..\\swift-build\\TestData\\signs\\black_danger.png")));
}

void SMainWindow::trainItems()
{
	//cv::Mat trainDescriptors(1,mExtractor->descriptorSize(),mExtractor->descriptorType());
	for (unsigned int i = 0; i < mTrainItems.size(); i++)
	{
		mTrainItems[i].train(*mMatcher);
		//trainDescriptors.push_back(mTrainItems[i].getDescriptors());
		mBOWTrainer->add(mTrainItems[i].getDescriptors());
	}

	mVocabulary = mBOWTrainer->cluster();
}

void SMainWindow::matchItems()
{
	for (unsigned int i = 0; i < mExploreItems.size(); i++)
	{
		mExploreItems[i].match(*mMatcher);
	}

	mBFMatcher->add(mMatcher->getTrainDescriptors());
	mBFMatcher->train();

	mBOWExtractor = new cv::BOWImgDescriptorExtractor(mExtractor,mBFMatcher);
	mBOWExtractor->setVocabulary(mVocabulary);
}

void SMainWindow::computeBOW()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Evaluation Area

void SMainWindow::loadEvaluationDatabase(QString path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream textstream(&file);
	QString line = textstream.readLine();
	while (!line.isNull()) {
		mOfficialEvaluationData.append(line);
		//process_line(line);
		line = textstream.readLine();
	}
}

void SMainWindow::evaluate()
{
	mNames << "STOP-Schild"
		<< "Vorfahrt Achten"
		<< "Geschwindigkeit - 30 km/h"
		<< "Geschwindigkeit - 60 km/h"
		<< "Geschwindigkeit - 120 km/h"
		<< "Gefahrenzeichen";

	// mOfficialEvaluationData[0] ist ein String aus "1;image01.jpg;1446;560;1542;648;3"
	//                                               ID;Bildname   ;x1  ;y1 ;x2  ;y2 ;Class-ID
	// QStringList list = mOfficialEvaluationData[0].split(";")

	//std::vector<cv::Point2i> pointsToEvaluate;

	for (unsigned int k = 0; k < mExploreItems.size(); k++)
	{
		for (unsigned int j = 0; j < mTrainItems.size(); j++)
		{
			if (mExploreItems[k].getFinalPointsByIndex(j).size() > 0)
			{
				QString explorenr_string, explorename_string, x_string, y_string, trainname_string;

				// evaluation image name / number...
				int nr = k+1;
				explorenr_string = QString("%1").arg(nr,2);
				explorename_string = explorename_string.append("image").append(explorenr_string).append(".png");

				// ...position of the point...
				int x = mExploreItems[k].getFinalPointsByIndex(j)[0].x;
				x_string = x_string.setNum(x);
				int y = mExploreItems[k].getFinalPointsByIndex(j)[0].y;
				y_string = y_string.setNum(y);

				// ... with train item
				trainname_string = mNames[j];

				mOwnEvaluationData.append(QString("%1 matcht mit %2 am Punkt P(%3|%4)").arg(explorename_string,trainname_string,x_string,y_string));
			}
		}
		// my breakpoint int
		int i = 0;
	}

	QFile fOut("..\\swift-build\\log.txt");

	if (fOut.open(QFile::WriteOnly | QFile::Text))
	{
		QTextStream s(&fOut);

		for (int i = 0; i < mOwnEvaluationData.size(); i++)
		{
			s << mOwnEvaluationData[i] << '\n';
		}
	}
	fOut.close();

	//for (unsigned int i = 0; i < mOfficial; i++)
	//{
	//}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

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
		QDir::currentPath().append("\\TestData"),
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