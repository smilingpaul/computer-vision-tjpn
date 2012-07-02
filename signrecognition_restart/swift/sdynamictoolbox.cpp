#include "StdAfx.h"
#include "sdynamictoolbox.h"

SDynamicToolbox::SDynamicToolbox(FeatureProvider *provider, QWidget *parent)
	: QToolBox(parent)
	, mProvider(provider)
{
	toolboxVerticalLayout = new QVBoxLayout();
}

SDynamicToolbox::~SDynamicToolbox()
{
}

void SDynamicToolbox::generateFormLayout(QString prefix)
{
}