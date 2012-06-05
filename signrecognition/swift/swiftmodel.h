/// \file swiftmodel.h
///
///
///
#ifndef SWIFTMODEL_H
#define SWIFTMODEL_H

#include <QtCore/QtCore>
#include <QtGui/QMainWindow>
#include <QAbstractListModel>
#include <opencv2/features2d/features2d.hpp>
#include "swiftitem.h"

class SwiftModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum SwiftItemRoles {
		PathRole = Qt::UserRole + 1,
		ThumbnailRole,
		TrainRole
	};

	SwiftModel(QObject *parent = 0);
	~SwiftModel();

	// needs to be implemented
	// return row count for given parent
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	// return data, based on current index and requested role
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	//////////////////////////////////////////////////////////////////////////

	void loadFiles(QStringList newImagePaths);
	void trainFiles();
	void queryFiles();
private:
	QList<SwiftItem> mList;

	cv::SIFT mSift;
	cv::SiftFeatureDetector mDetector;
	cv::SiftDescriptorExtractor mExtractor;

	cv::FlannBasedMatcher mMatcher;

	//QModelIndex parent(const QModelIndex &child) const;
	//int columnCount(const QModelIndex &parent) const;
	//bool hasChildren(const QModelIndex &parent) const;

public slots:
};

#endif // SWIFTMODEL_H