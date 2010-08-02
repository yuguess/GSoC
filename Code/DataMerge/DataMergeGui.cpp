#include "DataMergeGui.h"
#include <Qt/QtGui>
#include <vector>

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
#include <algorithm>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include "DesktopServices.h"
#include "MessageLogResource.h"
#include "ModelServices.h"
#include "PlugInRegistration.h"
#include "Service.h"
#include "SessionItemSerializer.h"

using namespace std;
 
namespace 
{
	template <typename T>
	bool mergeElement(T *pData, int rowCount, int colCount, DataAccessor pSrcAcc, DataAccessor pDesAcc, int band)
	{
		//char fileName[100];

		//sprintf(fileName, "D:\\data%d.txt", band);
		//ofstream file(fileName);
		
		for (int row = 0; row < rowCount; row++) 	
			for (int col = 0; col < colCount; col++)   	  
			{
				pSrcAcc->toPixel(row, col);
				pDesAcc->toPixel(row, col);
				T *pSrcData = reinterpret_cast<T*>(pSrcAcc->getColumn());
				T *pDesData = reinterpret_cast<T*>(pDesAcc->getColumn());
				pDesData[band] = *pSrcData;
				//file << " " << pDesData[band];
			}
		//file.close();
		return true;
	}
};


DataMergeGui::DataMergeGui(QWidget *parent)
    : QDialog(parent)
{
	
	setModal( FALSE );
	QHBoxLayout *listLayout = new QHBoxLayout();
    QGroupBox *importGroup = new QGroupBox(tr("Imported RasterElement"));
    importList = new QListWidget;
    QVBoxLayout *importLayout = new QVBoxLayout;
    importLayout->addWidget(importList);
    importGroup->setLayout(importLayout);

    QGroupBox *mergeGroup = new QGroupBox(tr("Select Merge RasterElement"));
    mergeList = new QListWidget;
    QVBoxLayout *mergeLayout = new QVBoxLayout;
    mergeLayout->addWidget(mergeList);
    mergeGroup->setLayout(mergeLayout);

    listLayout->addWidget(importGroup);
    listLayout->addWidget(mergeGroup);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton(tr("Add"));
    removeButton = new QPushButton(tr("Remove"));
    mergeButton = new QPushButton(tr("Merge"));
	removeButton->setEnabled(false);
	mergeButton->setEnabled(false);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addMergeList()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeMergeList()));
	connect(mergeButton, SIGNAL(clicked()), this, SLOT(mergeData()));
	connect(mergeButton, SIGNAL(clicked()), this, SLOT(close()));

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(mergeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(listLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

DataMergeGui::~DataMergeGui()
{

}

void DataMergeGui::addMergeList()
{
    QListWidgetItem *crtItem = importList->currentItem();
    QString crtText = crtItem->text();
    QListWidgetItem *item = new QListWidgetItem(mergeList);
    item->setText(crtText);
	mergeElementList.push_back(filenameMap[crtText.toStdString()]);
	
	removeButton->setEnabled(true);
	mergeButton->setEnabled(true);
}

void DataMergeGui::removeMergeList()
{
	QListWidgetItem *item = mergeList->currentItem();
	if (item == NULL)
		return;
	QString itemText = item->text();
	
	RasterElement* pRasterElement = filenameMap[itemText.toStdString()];
	vector<RasterElement*>::iterator it;
	it = std::find(mergeElementList.begin(), mergeElementList.end(), pRasterElement);
	mergeElementList.erase(it);
	if (item != NULL)
        item->~QListWidgetItem();
	if (mergeList->count() == 0)
	{
        removeButton->setEnabled(false);
		mergeButton->setEnabled(false);
	}
}

void DataMergeGui::addImportList(vector<string>& vec)
{
    vector<string>::iterator iter;
    for (iter = vec.begin(); iter != vec.end(); ++iter)
    {
        QListWidgetItem *item = new QListWidgetItem(importList);
        item->setText(iter->c_str());
    }
}
void DataMergeGui::setFilenameMap(std::map<string, RasterElement*> tmp)
{
    filenameMap = tmp;
}

void DataMergeGui::setCubes(vector<DataElement*>& para)
{
	cubes = para;
}

bool DataMergeGui::mergeData() 
{
//	Service<ModelServices> pModel;
	StepResource pStep("Data Merge Begin", "app", "5E4BCD48-E662-408b-93AF-F9127CE56C66");

//	std::vector<DataElement*> cubes = pModel->getElements("RasterElement");

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

	RasterElement* pDesRaster = RasterUtilities::createRasterElement("DataMergeCube", rowCount,
      colCount, bandCount, type, BSQ, true, NULL);
	
	if (pDesRaster == NULL)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}

	FactoryResource<DataRequest> pRequest;
	pRequest->setInterleaveFormat(BSQ);
	pRequest->setWritable(true);
	DataAccessor pDesAcc = pDesRaster->getDataAccessor(pRequest.release());
	
	if (!pDesAcc.isValid())
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "pDesRaster Data Accessor Error!", "OK");
		pStep->finalize(Message::Failure, "pDesRaster Data Accessor Error!");
		return false;
	}

	int band = 0;
	for (vector<RasterElement*>::iterator element = mergeElementList.begin(); 
		  element != mergeElementList.end(); ++element)
	{
		
		RasterElement* pData = model_cast<RasterElement*>(*element);
		
		if (pData != NULL)
		{
			RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pData->getDataDescriptor());
			if (rowCount != pDesc->getRowCount())
			{
				QMessageBox::critical(NULL, "Spectral Data Merge", "Merge Data Format Error!", "OK");
				pStep->finalize(Message::Failure, "Merge Data Row Format Error!");
				return false;			
			}
			
			if (colCount != pDesc->getColumnCount())
			{
				QMessageBox::critical(NULL, "Spectral Data Merge", "Merge Data Format Error!", "OK");
				pStep->finalize(Message::Failure, "Merge Data Column Format Error!");
				return false;			
			}

			FactoryResource<DataRequest> pRequest;
			DataAccessor pSrcAcc = pData->getDataAccessor(pRequest.release());	
			switchOnEncoding(pDesc->getDataType(), mergeElement, NULL, rowCount, colCount, pSrcAcc, pDesAcc, band);
			band++;
		}
	}

	Service<DesktopServices> pDesktop;
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
	
	return true;
}