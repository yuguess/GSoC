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
#include <fstream>
#include <algorithm>

#include "DesktopServices.h"
#include "MessageLogResource.h"
#include "ModelServices.h"
#include "PlugInRegistration.h"
#include "Service.h"
#include "SessionItemSerializer.h"

#include "UtilityServices.h"
#include "ApplicationServices.h"
#include "Progress.h"
#include "Resource.h"
#include "DesktopServices.h"
#include "UtilityServices.h"
#include "StringUtilities.h"

#include "RasterFileDescriptor.h"
#include "GcpList.h"

using namespace std;
 
namespace 
{
	template <typename T>
	bool mergeElement(T *pData, int rowCount, int colCount, DataAccessor pSrcAcc, DataAccessor pDesAcc, int band, Progress *pProgress)
	{
		//char fileName[100];
		//sprintf(fileName, "D:\\data%d.txt", band);
		//ofstream file(fileName);

		for (int row = 0; row < rowCount; row++)
		{
			for (int col = 0; col < colCount; col++)   	  
			{
				pSrcAcc->toPixel(row, col);
				pDesAcc->toPixel(row, col);
				T *pSrcData = reinterpret_cast<T*>(pSrcAcc->getColumn());
				T *pDesData = reinterpret_cast<T*>(pDesAcc->getColumn());
				pDesData[band] = *pSrcData;
		//		file << " " << pDesData[band];
			}
			pProgress->updateProgress("Processing RasterElement" + StringUtilities::toDisplayString(band + 1), 
				(row+1) * 100 / rowCount, NORMAL);

		}
		//file.close();
		return true;
	}
};


DataMergeGui::DataMergeGui(QWidget *parent)
    : QDialog(parent)
{
	
	setModal(true);
	flag = 0;
	QHBoxLayout *listLayout = new QHBoxLayout();
    QGroupBox *importGroup = new QGroupBox(tr("Imported RasterElement"));
    importList = new QListWidget;
    QVBoxLayout *importLayout = new QVBoxLayout;
    importLayout->addWidget(importList);
    importGroup->setLayout(importLayout);

    QGroupBox *mergeGroup = new QGroupBox(tr("Select Merge RasterElement"));
    mergeList = new QListWidget;
	connect(mergeList, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChangedSlot()));
	//connect(mergeList, SIGNAL(pressed(QModelIndex)), this, SLOT(selectionChangedSlot()));
	//connect(mergeList, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(selectionChangedSlot()));	
	
    QVBoxLayout *mergeLayout = new QVBoxLayout;
    mergeLayout->addWidget(mergeList);
    mergeGroup->setLayout(mergeLayout);

    listLayout->addWidget(importGroup);
    listLayout->addWidget(mergeGroup);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton(tr("Add"));
    removeButton = new QPushButton(tr("Remove"));
    mergeButton = new QPushButton(tr("Merge"));
	upButton = new QPushButton(tr("Up"));
	downButton = new QPushButton(tr("Down"));
	removeButton->setEnabled(false);
	mergeButton->setEnabled(false);
	upButton->setEnabled(false);
	downButton->setEnabled(false);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addMergeList()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeMergeList()));
	connect(mergeButton, SIGNAL(clicked()), this, SLOT(mergeData()));
	connect(upButton, SIGNAL(clicked()), this, SLOT(upButtonSlot()));
	connect(downButton, SIGNAL(clicked()), this, SLOT(downButtonSlot()));

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(mergeButton);
	buttonLayout->addWidget(upButton);
	buttonLayout->addWidget(downButton);

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
	
	if (flag == 0)
	{
		RasterElement *pFisrtElement = filenameMap[crtText.toStdString()];
		RasterDataDescriptor* pFirstDesc = static_cast<RasterDataDescriptor*>(pFisrtElement->getDataDescriptor());
		firstType = pFirstDesc->getDataType();
		firstRowCount = pFirstDesc->getRowCount();
		firstColCount = pFirstDesc->getColumnCount();
	}
	else
	{
		RasterElement *pTempElement = filenameMap[crtText.toStdString()];
		RasterDataDescriptor* pTempDesc = static_cast<RasterDataDescriptor*>(pTempElement->getDataDescriptor());
		EncodingType tempType = pTempDesc->getDataType();
		int tempRowCount = pTempDesc->getRowCount();
		int tempColCount = pTempDesc->getColumnCount();

		if (firstType != tempType)
		{
			QMessageBox::critical(NULL, "Spectral Data Merge", "Merge RasterElement type different!", "OK");
			return;
		}

		if (firstRowCount != tempRowCount)
		{
			QMessageBox::critical(NULL, "Spectral Data Merge", "Merge RasterElement type row different!", "OK");
			return;
		}

		if (firstColCount != tempColCount)
		{
			QMessageBox::critical(NULL, "Spectral Data Merge", "Merge RasterElement type column different!", "OK");
			return;
		}
	} 

    QListWidgetItem *item = new QListWidgetItem(mergeList);
    item->setText(crtText);
//	mergeElementList.push_back(filenameMap[crtText.toStdString()]);
	removeButton->setEnabled(true);
	mergeButton->setEnabled(true);
	flag = 1;
}

void DataMergeGui::removeMergeList()
{
	QListWidgetItem *item = mergeList->currentItem();
	if (item == NULL)
		return;
	QString itemText = item->text();
/*
	RasterElement *pRasterElement = filenameMap[itemText.toStdString()];
	vector<RasterElement*>::iterator it;
	it = std::find(mergeElementList.begin(), mergeElementList.end(), pRasterElement);
	mergeElementList.erase(it);*/
	if (item != NULL)
        item->~QListWidgetItem();
	if (mergeList->count() == 0)
	{
        removeButton->setEnabled(false);
		mergeButton->setEnabled(false);
		flag = 0;
	}
	selectionChangedSlot();
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

void DataMergeGui::setProgress(Progress *paraProgress)
{
	pProgress = paraProgress;
}

bool DataMergeGui::upButtonSlot()
{
	int curRow = mergeList->currentRow();
	QListWidgetItem *curItem = mergeList->currentItem();
	QString curItemText = curItem->text();

	QListWidgetItem *preItem = mergeList->item(curRow - 1);
	QString preItemText = preItem->text();

	curItem->setText(preItemText);
	preItem->setText(curItemText);
	mergeList->setCurrentRow(curRow - 1);

	return true;
}

bool DataMergeGui::downButtonSlot()
{
	int curRow = mergeList->currentRow();
	QListWidgetItem *curItem = mergeList->currentItem();
	QString curItemText = curItem->text();

	QListWidgetItem *nextItem = mergeList->item(curRow + 1);
	QString nextItemText = nextItem->text();

	curItem->setText(nextItemText);
	nextItem->setText(curItemText);
	mergeList->setCurrentRow(curRow + 1);
	return true;
}

bool DataMergeGui::selectionChangedSlot()
{
	if (mergeList->count() == 0)
	{
		upButton->setEnabled(false);
		downButton->setEnabled(false);
		return true;
	}

	int num = mergeList->count();
	int curRow = mergeList->currentRow();
	if (curRow == 0 && curRow == (num - 1))
	{
		upButton->setEnabled(false);
		downButton->setEnabled(false);
		return true;
	}

	if (curRow == 0)
	{
		upButton->setEnabled(false);
		downButton->setEnabled(true);
		return true;
	}

	if (curRow == (num - 1))
	{
		upButton->setEnabled(true);
		downButton->setEnabled(false);
		return true;
	}
	
	upButton->setEnabled(true);
	downButton->setEnabled(true);
	return true;
}

bool DataMergeGui::mergeData() 
{
//	Service<ModelServices> pModel;
	StepResource pStep("Data Merge Begin", "app", "5E4BCD48-E662-408b-93AF-F9127CE56C66");
	
	int mergeItemNum = mergeList->count();
	
	if (mergeItemNum == 0)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement to merge", "OK");
		pStep->finalize(Message::Failure, "No RasterElement to merge");
		return false;
	}

	for (int i = 0; i < mergeItemNum; i++)
	{
		QListWidgetItem *tmpItem = mergeList->item(i);
		QString tmpItemText = tmpItem->text();
		mergeElementList.push_back(filenameMap[tmpItemText.toStdString()]);
	}
	

//	pProgress = new Progress(this, "Progress Reporter");
//	std::vector<DataElement*> cubes = pModel->getElements("RasterElement");
	if (mergeElementList.size() == 0)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}
	vector<RasterElement*>::iterator initIter = mergeElementList.begin();
	RasterElement* pInitData = model_cast<RasterElement*>(*initIter);

	if (pInitData == NULL)
	{
		pStep->finalize(Message::Failure, "Cube Data error!");
		return false;
	}

	RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pInitData->getDataDescriptor());
	EncodingType type = pDesc->getDataType();
	int rowCount = pDesc->getRowCount();
	int colCount = pDesc->getColumnCount();
	int bandCount = mergeElementList.size();

	RasterElement* pDesRaster = RasterUtilities::createRasterElement("DataMergeCube", rowCount,
      colCount, bandCount, type, BIP, true, NULL);
	
	if (pDesRaster == NULL)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}

	FactoryResource<DataRequest> pRequest;
	pRequest->setInterleaveFormat(BIP);
	pRequest->setWritable(true);
	DataAccessor pDesAcc = pDesRaster->getDataAccessor(pRequest.release());
	
	if (!pDesAcc.isValid())
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "pDesRaster Data Accessor Error!", "OK");
		pStep->finalize(Message::Failure, "pDesRaster Data Accessor Error!");
		return false;
	} 

	if (pProgress == NULL) 
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "pProgress Initialize Error!", "OK");
		pStep->finalize(Message::Failure, "pProgress Error!");
		return false;
	}
//	progressDialog = new QProgressDialog();
//	progressDialog->setRange(0, rowCount);

	int band = 0;
	for (vector<RasterElement*>::iterator element = mergeElementList.begin(); 
		  element != mergeElementList.end(); ++element)
	{
//		progressDialog->setLabelText(tr("Merge RasterElement %1").arg(band + 1));
//		progressDialog->show();
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
			pRequest->setInterleaveFormat(BIP);
			pRequest->setWritable(true);
			DataAccessor pSrcAcc = pData->getDataAccessor(pRequest.release());	
			switchOnEncoding(pDesc->getDataType(), mergeElement, NULL, rowCount, colCount, pSrcAcc, pDesAcc, band, pProgress);
			band++;
		}
//		progressDialog->hide();
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
	

	if (pDesc != NULL)
    {
		const RasterFileDescriptor* pFileDescriptor = dynamic_cast<const RasterFileDescriptor*>(pDesc->getFileDescriptor());
        if (pFileDescriptor != NULL)
        {
			Service<ModelServices> pModel;
            if (pModel.get() != NULL)
            {
				list<GcpPoint> gcps;             
                gcps = pFileDescriptor->getGcps();           
                if (gcps.empty() == true)
                {
					if (pInitData->isGeoreferenced())
                    {
						GcpPoint gcp;

                        // Lower left
                        gcp.mPixel.mX = 0.0;
                        gcp.mPixel.mY = 0.0;
                        gcp.mCoordinate = pInitData->convertPixelToGeocoord(gcp.mPixel);
                        gcps.push_back(gcp);

                        // Lower right
                        gcp.mPixel.mX = colCount - 1;
                        gcp.mPixel.mY = 0.0;
                        gcp.mCoordinate = pInitData->convertPixelToGeocoord(gcp.mPixel);
                        gcps.push_back(gcp);

                        // Upper left
                        gcp.mPixel.mX = 0.0;
                        gcp.mPixel.mY = rowCount - 1;
                        gcp.mCoordinate = pInitData->convertPixelToGeocoord(gcp.mPixel);
                        gcps.push_back(gcp);

                        // Upper right
                        gcp.mPixel.mX = colCount - 1;
                        gcp.mPixel.mY = rowCount - 1;
                        gcp.mCoordinate = pInitData->convertPixelToGeocoord(gcp.mPixel);
                        gcps.push_back(gcp);

                        // Center
                        gcp.mPixel.mX = colCount / 2.0;
                        gcp.mPixel.mY = rowCount / 2.0;
                        gcp.mCoordinate = pInitData->convertPixelToGeocoord(gcp.mPixel);
                        gcps.push_back(gcp);
                     }
                  }

                  if (gcps.empty() == false)
                  {
                     DataDescriptor* pGcpDescriptor = pModel->createDataDescriptor("Corner Coordinates",
                        "GcpList", pDesRaster);
                     if (pGcpDescriptor != NULL)
                     {
                        GcpList* pGcpList = static_cast<GcpList*>(pModel->createElement(pGcpDescriptor));
                        if (pGcpList != NULL)
                        {
                           // Add the GCPs to the GCP list
                           pGcpList->addPoints(gcps);

                           // Create the GCP list layer
                           pView->createLayer(GCP_LAYER, pGcpList);
                        }
                     }
                  }
               }
		}
	}

	return true;
}