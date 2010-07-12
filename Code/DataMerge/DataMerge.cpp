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

namespace 
{
	template <typename T>
	void mergeElement(T *pData, int rowCount, int colCount, DataAccessor pSrcAcc, int i)
	{
		char fileName[100];
		sprintf(fileName, "D:\\data%d.txt", i);
		ofstream file(fileName);
		
		for (int row = 0; row < rowCount; row++) 	
			for (int col = 0; col < colCount; col++)   	  
			{
				pSrcAcc->toPixel(row, col);
				T *mpData = reinterpret_cast<T*>(pSrcAcc->getColumn());
				file << " " << *mpData;
			}
		file.close();
	}
};

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

bool DataMerge::showGui()
{

	Service<DesktopServices> pDesktop;
	Service<ModelServices> pModel;
	StepResource pStep("Pixel Aspect Ratio Started.", "app", "5E4BCD48-E662-408b-93AF-F9127CE56C66");

	std::vector<DataElement*> cubes = pModel->getElements("RasterElement");
	if (cubes.size() == 0)
	{
		QMessageBox::critical(NULL, "Pixel Aspect Ratio Test", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}

	int i = 0;
	for (vector<DataElement*>::iterator element = cubes.begin(); element != cubes.end(); ++element)
	{
		RasterElement* pData = model_cast<RasterElement*>(*element);
		
		if (pData != NULL)
		{
			RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
			int rowCount = pDesc->getRowCount();
			int colCount = pDesc->getColumnCount();
			FactoryResource<DataRequest> pRequest;
			DataAccessor pSrcAcc = pData->getDataAccessor(pRequest.release());
			switchOnEncoding(pDesc->getDataType(), mergeElement, NULL, rowCount, colCount, pSrcAcc, i++);
		}
	}
/*
	mpGui = new DataMergeGui(pDesktop->getMainWidget(), "test", false);
	connect(mpGui, SIGNAL(finished(int)), this, SLOT(dialogClosed()));
	mpGui->show();

	pStep->finalize(Message::Success); */
	return true;
}

bool DataMerge::execute(PlugInArgList* inputArgList, PlugInArgList* outputArgLists)
{
   	/*
	Service<ModelServices> pModel;
	std::vector<DataElement*> cubes = pModel->getElements("RasterELement");
	
	if (cubes.size() == 0)
	{
		QMessageBox::critical(NULL, "Data Merge", "No RasterElement input found!", "OK");
//		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}
	int i = 0;
	for (vector<DataElement*>::iterator element = cubes.begin(); element != cubes.end(); ++element)
	{
		RasterElement* pData = model_cast<RasterElement*>(*element);
		
		if (pData != NULL)
		{
			RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
			int rowCount = pDesc->getRowCount();
			int colCount = pDesc->getColumnCount();
			FactoryResource<DataRequest> pRequest;
			DataAccessor pSrcAcc = pData->getDataAccessor(pRequest.release());
			switchOnEncoding(pDesc->getDataType(), mergeElement, NULL, rowCount, colCount, pSrcAcc, i++);
		}
	}
*/
	return showGui();
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
