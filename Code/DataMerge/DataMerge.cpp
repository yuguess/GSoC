#include "DataAccessor.h"
#include "DataAccessorImpl.h"
#include "DataRequest.h"
#include "DesktopServices.h"
#include "MessageLogResource.h"
#include "ObjectResource.h"
#include "PlugInArgList.h"
#include "PlugInManagerServices.h"
#include "PlugInRegistration.h"
#include "Progress.h"
#include "RasterDataDescriptor.h"
#include "RasterElement.h"
#include "RasterUtilities.h"
#include "SpatialDataView.h"
#include "SpatialDataWindow.h"
#include "switchOnEncoding.h"
#include "DataMerge.h"
#include "DataMergeGui.h"
#include "DataElement.h"
#include <limits>
#include <iostream>
#include <vector>
#include <Qt/QtGui>
#include <fstream>
#include <map>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include "DesktopServices.h"
#include "MessageLogResource.h"
#include "ModelServices.h"
#include "PlugInRegistration.h"
#include "Service.h"
#include "SessionItemSerializer.h"

using namespace std;

REGISTER_PLUGIN_BASIC(SpectralDataMerge, DataMerge);


DataMerge::DataMerge() :
   mpGui(NULL)
{
   setCreator("Opticks Community");
   setVersion("Sample");
   setCopyright("Copyright (C) 2008, Ball Aerospace & Technologies Corp.");
   setProductionStatus(false);
   ViewerShell::setName("Spectral Data Merge");
   setDescription("Merge one-band spectral data into into one multi-band RasterElement");
   setMenuLocation("[Spectral Data Merge]/SpectralDataMerge");
   setDescriptorId("{1AA900B7-09FE-4d7b-A8D4-25DEEDE39CEC}");
   destroyAfterExecute(false);
   setWizardSupported(false);
   setAbortSupported(true);
}

DataMerge::~DataMerge()
{
}

bool DataMerge::getInputSpecification(PlugInArgList*& pArgList)
{
	VERIFY(pArgList = Service<PlugInManagerServices>()->getPlugInArgList());
	VERIFY(pArgList != NULL);
    VERIFY(pArgList->addArg<Progress>(ProgressArg(), NULL));
	return true;
}
   
bool DataMerge::getOutputSpecification(PlugInArgList*& pArgList)
{
	pArgList = NULL;
	return true;
}

bool DataMerge::showGui()
{
	
	Service<ModelServices> pModel;
	StepResource pStep("Data Merge Begin", "app", "5E4BCD48-E662-408b-93AF-F9127CE56C66");

	std::vector<DataElement*> cubes = pModel->getElements("RasterElement");

	if (cubes.size() == 0)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}

	vector<DataElement*>::iterator initIter = cubes.begin();
	RasterElement* pData = model_cast<RasterElement*>(*initIter);
	
	if (pData == NULL)
	{
		pStep->finalize(Message::Failure, "Cube Data error!");
		return false;
	}

	RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
	EncodingType type = pDesc->getDataType();
	int rowCount = pDesc->getRowCount();
	int colCount = pDesc->getColumnCount();
	int bandCount = cubes.size();
	
	vector<string> filenameVec;
	map<string, RasterElement*> filenameMap;
	for (vector<DataElement*>::iterator element = cubes.begin(); element != cubes.end(); ++element)
	{
		
		RasterElement* pData = model_cast<RasterElement*>(*element);
		
		if (pData != NULL)
		{
			RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
			filenameMap[pData->getFilename()] = pData;
			filenameVec.push_back(pData->getFilename());
		}
	}

    Service<DesktopServices> pDesktop;
	mpGui = new DataMergeGui(pDesktop->getMainWidget());
    connect(mpGui, SIGNAL(finished(int)), this, SLOT(dialogClosed()));
	mpGui->addImportList(filenameVec);
	mpGui->setFilenameMap(filenameMap);
	mpGui->setCubes(cubes);
	mpGui->show();

	/*
	SpatialDataWindow* pWindow = static_cast<SpatialDataWindow*>(pDesktop->createWindow("DataMergeResult",
	   SPATIAL_DATA_WINDOW));

    SpatialDataView* pView = (pWindow == NULL) ? NULL : pWindow->getSpatialDataView();
    if (pView == NULL)
    {
		pStep->finalize(Message::Failure, "SpatialDataView error!");
	    return false;
    }

    pView->setPrimaryRasterElement(pDesRaster);
    pView->createLayer(RASTER, pDesRaster);
*/
	return true;
}

bool DataMerge::execute(PlugInArgList* inputArgList, PlugInArgList* outputArgLists)
{
	pProgress = inputArgList->getPlugInArgValue<Progress>(ProgressArg());
	Service<ModelServices> pModel;
	StepResource pStep("Data Merge Begin", "app", "5E4BCD48-E662-408b-93AF-F9127CE56C66");

	std::vector<DataElement*> cubes = pModel->getElements("RasterElement");

	if (cubes.size() == 0)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}

	vector<DataElement*>::iterator initIter = cubes.begin();
	RasterElement* pData = model_cast<RasterElement*>(*initIter);
	
	if (pData == NULL)
	{
		pStep->finalize(Message::Failure, "Cube Data error!");
		return false;
	}

	RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
	EncodingType type = pDesc->getDataType();
	int rowCount = pDesc->getRowCount();
	int colCount = pDesc->getColumnCount();
	int bandCount = cubes.size();
	
	vector<string> filenameVec;
	map<string, RasterElement*> filenameMap;
	for (vector<DataElement*>::iterator element = cubes.begin(); element != cubes.end(); ++element)
	{
		
		RasterElement* pData = model_cast<RasterElement*>(*element);
		
		if (pData != NULL)
		{
			RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
			filenameMap[pData->getFilename()] = pData;
			filenameVec.push_back(pData->getFilename());
		}
	}

    Service<DesktopServices> pDesktop;
	mpGui = new DataMergeGui(pDesktop->getMainWidget());
    connect(mpGui, SIGNAL(finished(int)), this, SLOT(dialogClosed()));
	mpGui->addImportList(filenameVec);
	mpGui->setFilenameMap(filenameMap);
	mpGui->setCubes(cubes);
	mpGui->setProgress(pProgress);
	mpGui->show();

	return true;
	//	return showGui();
}

QWidget* DataMerge::getWidget() const
{
    return mpGui;
}

void DataMerge::dialogClosed()
{
    abort();
}

bool DataMerge::serialize(SessionItemSerializer &serializer) const
{
    return serializer.serialize(NULL, 0); // force recreation on session load
}

bool DataMerge::deserialize(SessionItemDeserializer &deserializer)
{
    return showGui();
}
