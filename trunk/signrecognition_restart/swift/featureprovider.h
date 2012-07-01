#ifndef FEATUREPROVIDER_H
#define FEATUREPROVIDER_H

#include <QtCore/QObject>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>

class FeatureProvider : public QObject
{
	Q_OBJECT

public:
	FeatureProvider(QObject *parent = 0);
	~FeatureProvider();

	cv::FeatureDetector* provideDetector();
	cv::DescriptorExtractor* provideExtractor();

	//// BONUS
	//cv::DescriptorMatcher* provideMatcher();

	template<typename T> void insert(QString key, T input);
	template<typename T> void insert(QString prefix, QString key, T input);

	QVariant value(QString key);
	QVariant value(QString prefix, QString key);
	template<typename T> T value(QString key);
	template<typename T> T value(QString prefix, QString key);

	//////////////////////////////////////////////////////////////////////////

	QStringList getDetectorList();
	QStringList getExtractorList();
	QString getCurrentDetector();
	QString getCurrentExtractor();
	int getCurrentDetectorIndex();
	int getCurrentExtractorIndex();

signals:

public slots:

	void defaults();

	void setCurrent(int detector = -1, int extractor = -1);
	void setCurrent(QString detector, QString extractor);

private:
	void initialize();

	QStringList mDetectorList;
	QStringList mExtractorList;

	QMap<QString,QVariant> mConfiguration;
	QMap<QString,QString> mConfigurationType;

	bool mCurrentDetectorChanged;
	bool mCurrentExtractorChanged;

	QString mPrefix;
	QString mCurrentDetector;
	int mCurrentDetectorIndex;
	QString mCurrentExtractor;
	int mCurrentExtractorIndex;

	cv::FeatureDetector* mDetector;
	cv::DescriptorExtractor* mExtractor;
};

#endif // FEATUREPROVIDER_H