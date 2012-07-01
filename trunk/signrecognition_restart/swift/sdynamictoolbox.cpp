#include "StdAfx.h"
#include "sdynamictoolbox.h"

SDynamicToolbox::SDynamicToolbox(FeatureProvider *provider, QWidget *parent)
	: QToolBox(parent)
	, mProvider(provider)
{
}

SDynamicToolbox::~SDynamicToolbox()
{
}

void SDynamicToolbox::generateFormLayout(QString prefix)
{
}