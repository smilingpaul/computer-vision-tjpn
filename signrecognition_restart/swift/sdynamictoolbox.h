#ifndef SDYNAMICTOOLBOX_H
#define SDYNAMICTOOLBOX_H

#include <QToolBox>
#include <featureprovider.h>

class SDynamicToolbox : public QToolBox
{
	Q_OBJECT

public:
	SDynamicToolbox(FeatureProvider *provider, QWidget *parent);
	~SDynamicToolbox();

private:
	FeatureProvider *mProvider;

	QVBoxLayout *toolboxVerticalLayout;

	void generateFormLayout(QString prefix);
};

#endif // SDYNAMICTOOLBOX_H