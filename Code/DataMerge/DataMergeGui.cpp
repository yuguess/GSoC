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

#include "ImportElement.h"

using namespace std;
 
namespace 
{
	template <typename T>
	bool mergeElement(T *pData, int rowCount, int colCount, DataAccessor pSrcAcc, DataAccessor pDesAcc, 
		int index, int band, Progress *pProgress, int size)
	{
		//char fileName[100];
		//sprintf(fileName, "D:\\data%d.txt", band);
		//ofstream file(fileName);
		//QMessageBox::about(NULL, "Test", "Here1");
		for (int row = 0; row < rowCount; row++)
		{
			for (int col = 0; col < colCount; col++)   	  
			{
		//		QMessageBox::about(NULL, "Test", "Here3.1");
				pSrcAcc->toPixel(row, col);
		//		QMessageBox::about(NULL, "Test", "Here3.3");
				pDesAcc->toPixel(row, col);
		//		QMessageBox::about(NULL, "Test", "Here3.5");
				T *pSrcData = reinterpret_cast<T*>(pSrcAcc->getColumn());
				T *pDesData = reinterpret_cast<T*>(pDesAcc->getColumn());
		//		QMessageBox::about(NULL, "Test", "Here3");
				pDesData[index] = pSrcData[band];
		//		QMessageBox::about(NULL, "Test", "Here4");
		//		file << " " << pDesData[band];
			}	
		
			pProgress->updateProgress("Processing RasterElement" + StringUtilities::toDisplayString(index + 1), 
				((row + 1) + (index + 1) * rowCount * 100) / (rowCount * size), NORMAL);

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
    importTree = new QTreeWidget();
    connect(importTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            this, SLOT(treeItemSlot(QTreeWidgetItem *, int)));
	importTree->setSelectionMode(QAbstractItemView::ContiguousSelection);
	//importList = new QListWidget;
	//importList->setSelectionMode(QAbstractItemView::ContiguousSelection);
    QVBoxLayout *importLayout = new QVBoxLayout;
    //importLayout->addWidget(importList);
	importLayout->addWidget(importTree);
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

void DataMergeGui::treeItemSlot(QTreeWidgetItem *item, int column)
{
//     QMessageBox::about(this, "Test", tr("Here"));
	if (!item->parent())
    {
        if (item->checkState(0) == Qt::Unchecked)
        {
            updateCheckState(item, Qt::Unchecked);
        } else {
            updateCheckState(item, Qt::Checked);
        }
    }
    return;
}

void DataMergeGui::updateCheckState(QTreeWidgetItem *parent, Qt::CheckState state)
{
    if (parent->childCount() == 0)
        return;    

    for (int i = 0; i < parent->childCount(); i++)
    {
        QTreeWidgetItem * item = parent->child(i);
//        QMessageBox::about(this, "Test", tr("Here%"));
        item->setCheckState(0, state);
    }
}

void DataMergeGui::addMergeList()
{

	for (int i = 0; i < importTree->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *topItem = importTree->topLevelItem(i);
		for (int i = 0; i < topItem->childCount(); i++)
		{
			QTreeWidgetItem *childItem = topItem->child(i);
			if (childItem->checkState(0) == Qt::Checked)
			{				
				QString crtText = topItem->text(0);

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
						QMessageBox::critical(NULL, "Spectral Data Merge", 
							tr("Merge RasterElement %1 type different!").arg(crtText), "OK");
						return;
					}

					if (firstRowCount != tempRowCount)
					{
						QMessageBox::critical(NULL, "Spectral Data Merge", 
							tr("Merge RasterElement %1 row different!").arg(crtText), "OK");
						return;
					}

					if (firstColCount != tempColCount)
					{
						QMessageBox::critical(NULL, "Spectral Data Merge", 
							tr("Merge RasterElement column different!").arg(crtText), "OK");
						return;
					}
				} 

				QListWidgetItem *item = new QListWidgetItem(mergeList);		
				item->setText(crtText + "--" + childItem->text(0));
				//	mergeElementList.push_back(filenameMap[crtText.toStdString()]);
				removeButton->setEnabled(true);
				mergeButton->setEnabled(true);
				flag = 1;
			}
		}
		
    //QListWidgetItem *crtItem = importList->currentItem();
/*	QList<QListWidgetItem *> selectedList = importList->selectedItems();
    
	for (int i = 0; i < selectedList.count(); i++)
	{
		QListWidgetItem *crtItem = selectedList.at(i);
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
				QMessageBox::critical(NULL, "Spectral Data Merge", "Merge RasterElement row different!", "OK");
				return;
			}

			if (firstColCount != tempColCount)
			{
				QMessageBox::critical(NULL, "Spectral Data Merge", "Merge RasterElement column different!", "OK");
				return;
			}
		} 

		QListWidgetItem *item = new QListWidgetItem(mergeList);
		item->setText(crtText);
		//	mergeElementList.push_back(filenameMap[crtText.toStdString()]);
		removeButton->setEnabled(true);
		mergeButton->setEnabled(true);
		flag = 1;
	} */
	}
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

//void DataMergeGui::addImportList(vector<string>& vec)
void DataMergeGui::addImportList(vector<ImportElement>& vec)
{
	vector<ImportElement>::iterator iter;
    for (iter = vec.begin(); iter != vec.end(); ++iter)
    {
        QTreeWidgetItem *topItem = new QTreeWidgetItem(importTree);
		string name = iter->fileName;
		
		topItem->setText(0, name.c_str());
		topItem->setCheckState(0, Qt::Unchecked);
		for (int i = 0; i < iter->bandNum; i++)
		{
			QTreeWidgetItem *childItem = new QTreeWidgetItem(topItem);
			childItem->setCheckState(0, Qt::Unchecked);
			childItem->setText(0, tr("Band%1").arg(i + 1));
		}
    }

	/* 
	vector<string>::iterator iter;
    for (iter = vec.begin(); iter != vec.end(); ++iter)
    {
        QListWidgetItem *item = new QListWidgetItem(importList);
        item->setText(iter->c_str());
    }
*/
	
   // importTree->setColumnCount(1);
   /*
    for (int i = 0; i < 4; ++i)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(importTree);
        item->setText(0, tr("item%1").arg(i));
        item->setCheckState(0, Qt::Unchecked);
        connect(importTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
                this, SLOT(treeItemSlot(QTreeWidgetItem *, int)));

        items.append(item);
    }

    for (int t = 0; t < 4; ++t) 
	{
        QTreeWidgetItem *table = new QTreeWidgetItem(items.at(0));
        table->setCheckState(0, Qt::Unchecked);
        table->setText(0, "Test");
    }

    importTree->insertTopLevelItems(0, items); */
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

RasterElement* DataMergeGui::extractRasterElement(QString str)
{
	QStringList strList = str.split("--");
	QString fileName = strList.at(0);
//	QMessageBox::about(this, "Test", tr("%1").arg(fileName));
	return filenameMap[fileName.toStdString()];
}

int DataMergeGui::extractMergeBand(QString str)
{
	QStringList strList = str.split("--");
	QString tempStr = strList.at(1);
	strList = tempStr.split("Band");	
	QString mergeBand = strList.at(1);
//	QMessageBox::about(this, "Test", tr("%1").arg(mergeBand));
	return mergeBand.toInt() - 1;
}

bool DataMergeGui::mergeData() 
{
//	Service<ModelServices> pModel;
	StepResource pStep("Data Merge Begin", "app", "5E4BCD48-E662-408b-93AF-F9127CE56C66");	
	if (mergeList->count() == 0)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement to merge", "OK");
		pStep->finalize(Message::Failure, "No RasterElement to merge");
		return false;
	}

//	pProgress = new Progress(this, "Progress Reporter");
//	std::vector<DataElement*> cubes = pModel->getElements("RasterElement");
/*	if (mergeElementList.size() == 0)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "No RasterElement input found!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	} */
	//QListWidgetItem *tmpItem = mergeList->item(i0);
	//QString tmpItemText = tmpItem->text();
	RasterElement* pInitData = extractRasterElement(mergeList->item(0)->text());

//	vector<RasterElement*>::iterator initIter = mergeElementList.begin();
//	RasterElement* pInitData = model_cast<RasterElement*>(*initIter);

	if (pInitData == NULL)
	{
		pStep->finalize(Message::Failure, "Cube Data error!");
		QMessageBox::critical(this, "Error", "pInitData Error");
		return false;
	}

	RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pInitData->getDataDescriptor());
	EncodingType type = pDesc->getDataType();
	int rowCount = pDesc->getRowCount();
	int colCount = pDesc->getColumnCount();
	int bandCount = mergeList->count();

	RasterElement* pDesRaster = RasterUtilities::createRasterElement("DataMergeCube", rowCount,
      colCount, bandCount, type, BIP, true, NULL);
	
	if (pDesRaster == NULL)
	{
		QMessageBox::critical(NULL, "Spectral Data Merge", "Create RasterElement failed, Please close the previous merge result!", "OK");
		pStep->finalize(Message::Failure, "No RasterElement input found!");
		return false;
	}

	FactoryResource<DataRequest> pRequest;
	pRequest->setInterleaveFormat(BIP);
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
	
	//int index = 0;
	for (int i = 0; i < mergeList->count(); i++)
	{
		QListWidgetItem *tmpItem = mergeList->item(i);
		QString tmpItemText = tmpItem->text();
		RasterElement* pData = extractRasterElement(tmpItemText);
		int band = extractMergeBand(tmpItemText);

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
	//		QMessageBox::about(this, "Test", "Here2");
			FactoryResource<DataRequest> pRequest;
			pRequest->setInterleaveFormat(BIP);
		//	pRequest->setWritable(true);
			DataAccessor pSrcAcc = pData->getDataAccessor(pRequest.release());	
			switchOnEncoding(pDesc->getDataType(), mergeElement, NULL, rowCount, 
				colCount, pSrcAcc, pDesAcc, i, band, pProgress, mergeList->count());
	//		QMessageBox::about(this, "Test", "Here5");
		}
		else {
			QMessageBox::critical(this, "Error", "pData is NULL");
			return false;
		}

	//	mergeElementList.push_back(filenameMap[tmpItemText.toStdString()]);
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