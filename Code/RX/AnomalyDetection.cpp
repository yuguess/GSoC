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
#include "AnomalyDetection.h"
#include <limits>
#include <iostream>

#include "tnt_cmat.h"
#include "tnt_array1d.h"
#include "MatrixFunctions.h"

REGISTER_PLUGIN_BASIC(AnomalyDetectionModule, AnomalyDetection);

namespace
{
	int meanmat(TNT::Matrix<double> subdata, int col, int row,int bands, TNT::Matrix<double> &u)
	{		
		if (u.num_rows()<bands)
			return -1;

		int count = col*row;
		for (int i = 0; i < bands; i++)
		{
			for (int j = 0; j < count; j++)
				u[i][0] += subdata[i][j];
			u[i][0] = (u)[i][0] / count;
		}
		return 1;
	}

	int covmat(TNT::Matrix<double> subData, int col, int row, int bands, TNT::Matrix<double> me, TNT::Matrix<double> &cov)
	{

		TNT::Matrix<double> temp(bands, 1, 0.0); 

		int count = col * row;
		for (int i = 0; i < count; i++)
		{
			for (int j = 0; j < bands; j++)
				temp[j][0] = subData[j][i] - me[j][0];

			for (int m = 0; m < bands; m++)
				for (int n = m; n < bands; n++)
				{
					cov[n][m] = temp[m][0] * temp[n][0];
				}
		}

		int count2 = count-1;
		for (int i = 0; i < bands; i++)
		{
			for (int j = i + 1; j < bands; j++)
			{
				cov[j][i] /= count2;
				cov[i][j] = cov[j][i];
			}
			cov[i][i] /= count2;
		}
		return 1;
	}

	int adddiag(int bands, double weight,TNT::Matrix<double> &cov)
	{
		for (int i = 0; i < bands; i++)
			cov[i][i] += weight;

		return 1;
	}

	int rxl(TNT::Matrix<double> &rxd, TNT::Matrix<double> realdata,int samples, int lines, int bands, Progress *pProgress)
	{
		TNT::Matrix<double> u(bands, 1, 0.0);  
		TNT::Matrix<double> temp(bands , 1, 0.0);  
		TNT::Matrix<double> cov(bands,bands, 0.0);
		TNT::Matrix<double> invcov(bands,bands, 0.0);

		int col=0;
		int row = 0;
		int ret=0;
		double weight = 0,tmp=0;

		if (rxd.num_rows()<lines||rxd.num_cols()<samples)
		{
			return -1;
		}

		ret = meanmat(realdata,samples,lines,bands,u);
		if (pProgress != NULL)
		{
			pProgress->updateProgress("Calculating", 20 * 100 / 100, NORMAL);
		}
		ret = covmat(realdata,samples ,lines,bands,u,cov);
		pProgress->updateProgress("Calculating", 30 * 100 / 100, NORMAL);
		for (int i = 0; i < bands; i++)
		{
			weight += cov[i][i];
		}

		ret = adddiag(bands,weight,cov);

	    double **pSrc = cov;
	    double **pDesc = invcov;
	    int rows = cov.num_rows();

	    if (!MatrixFunctions::invertSquareMatrix2D(pDesc, const_cast<const double**>(pSrc), rows))
	    {
		    return false;
	    }
		pProgress->updateProgress("Calculating", 70 * 100 / 100, NORMAL);
		for(int x = 0; x < lines; x++)
		{
			for (int y = 0; y < samples; y++)
			{
				for (int i=0; i<bands; i++)
				{
					temp[i][0] = realdata[i][x*samples+y]-u[i][0];
				}

				for (int i = 0 ; i < bands; i++)
				{
					tmp = 0;					
					for (int j = 0; j < bands; j++)
					{
						tmp += temp[j][0] * invcov[j][i];
					}
					rxd[x][y] += tmp * temp[i][0];
				}
			}			
		}

		pProgress->updateProgress("Calculating", 90 * 100 / 100, NORMAL);
		return ret;
	}

	template <class T>
	void RxAnomalyDetection(T *pData, DataAccessor pSrcAcc, DataAccessor desAcc, 
							int rowCount, int colCount, int bandCount, Progress *pProgress) 
	{
	   int ret=0;

	   TNT::Matrix<double> background(bandCount, rowCount * colCount, 0.0);
	   TNT::Matrix<double> rxdResult(rowCount, colCount, 0.0);
		
	   for (int band = 0; band < bandCount; band++)
		   for (int row = 0; row < rowCount; row++) 	
		    {
			for (int col = 0; col < colCount; col++)   	  
			   {
				   pSrcAcc->toPixel(row, col);
				   T *mpData = reinterpret_cast<T*>(pSrcAcc->getColumn());
				   background[band][row * colCount + col] = mpData[band];				  
			   }
		    }
	   if (pProgress != NULL)
		{
			pProgress->updateProgress("Calculating", 10 * 100 / 100, NORMAL);
		}

	   ret = rxl(rxdResult, background, colCount, rowCount, bandCount, pProgress);

	   for (int row = 0; row < rowCount; row++)
	   {
		   for (int col = 0; col < colCount; col++)
		   {
			   desAcc->toPixel(row, col);
			   T *mpData = reinterpret_cast<T*>(desAcc->getColumn());
			   (*mpData) = rxdResult[row][col]; 
		   }

	   }
	}
};


AnomalyDetection::AnomalyDetection()
{
   setDescriptorId("{BE00BBC3-A1E3-4b0d-8780-1B5D9A862111}");
   setName("AnomalyDetection");
   setVersion("Sample");
   setDescription("Apply RX anomaly detection algorithm for the provided raster element");
   setCreator("Opticks Community");
   setCopyright("Copyright (C) 2010, Ball Aerospace & Technologies Corp.");
   setProductionStatus(false);
   setType("Sample");
   setSubtype("Edge Detection");
   setMenuLocation("[Spectral]\\Anomaly Detection\\RX");
   setAbortSupported(true);
}

AnomalyDetection::~AnomalyDetection()
{
}

bool AnomalyDetection::getInputSpecification(PlugInArgList*& pInArgList)
{
   VERIFY(pInArgList = Service<PlugInManagerServices>()->getPlugInArgList());
   pInArgList->addArg<Progress>(Executable::ProgressArg(), NULL, "Progress reporter");
   pInArgList->addArg<RasterElement>(Executable::DataElementArg(), "Perform edge detection on this data element");
   return true;
}

bool AnomalyDetection::getOutputSpecification(PlugInArgList*& pOutArgList)
{
   VERIFY(pOutArgList = Service<PlugInManagerServices>()->getPlugInArgList());
   pOutArgList->addArg<RasterElement>("Result", NULL);
   return true;
}

bool AnomalyDetection::execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList)
{
   StepResource pStep("CannyEdgeDetection", "app", "5EA0CC75-9E0B-4c3d-BA23-6DB7157BBD54");
   if (pInArgList == NULL || pOutArgList == NULL)
   {
      return false;
   }

   Progress* pProgress = pInArgList->getPlugInArgValue<Progress>(Executable::ProgressArg());
   RasterElement* pCube = pInArgList->getPlugInArgValue<RasterElement>(Executable::DataElementArg());

   if (pCube == NULL)
   {
      std::string msg = "A raster cube must be specified.";
      pStep->finalize(Message::Failure, msg);
      if (pProgress != NULL) 
      {
         pProgress->updateProgress(msg, 0, ERRORS);
      }
      return false;
   }
   RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pCube->getDataDescriptor());
   
   int rowCount = pDesc->getRowCount();
   int colCount = pDesc->getColumnCount();
   int bandCount = pDesc->getBandCount();

   VERIFY(pDesc != NULL);
   if (pDesc->getDataType() == INT4SCOMPLEX || pDesc->getDataType() == FLT8COMPLEX)
   {
      std::string msg = "Canny Edge detection cannot be performed on complex types.";
      pStep->finalize(Message::Failure, msg);
      if (pProgress != NULL) 
      {
         pProgress->updateProgress(msg, 0, ERRORS);
      }
      return false;
   }

   FactoryResource<DataRequest> pRequest;
   pRequest->setInterleaveFormat(BIP);
   DataAccessor pSrcAcc = pCube->getDataAccessor(pRequest.release());

   ModelResource<RasterElement> pResultCube(RasterUtilities::createRasterElement(pCube->getName() +
      "_Anomaly_Detector_Result", pDesc->getRowCount(), pDesc->getColumnCount(), pDesc->getDataType()));

   if (pResultCube.get() == NULL)
   {
      std::string msg = "A raster cube could not be created.";
      pStep->finalize(Message::Failure, msg);
      if (pProgress != NULL) 
      {
         pProgress->updateProgress(msg, 0, ERRORS);
      }
      return false;
   }
   
   FactoryResource<DataRequest> pResultRequest;
   pResultRequest->setWritable(true);
   DataAccessor pDestAcc = pResultCube->getDataAccessor(pResultRequest.release());
   switchOnEncoding(pDesc->getDataType(), RxAnomalyDetection, pDestAcc->getColumn(), pSrcAcc, pDestAcc, 
	                pDesc->getRowCount(), pDesc->getColumnCount(), pDesc->getBandCount(), pProgress);	
//   switchOnEncoding(pDesc->getDataType(), cannyEdgeDetection, pDestAcc->getColumn(), pSrcAcc, pDestAcc, pDesc->getRowCount(), pDesc->getColumnCount());

   if (!isBatch())
   {
      Service<DesktopServices> pDesktop;

      SpatialDataWindow* pWindow = static_cast<SpatialDataWindow*>(pDesktop->createWindow(pResultCube->getName(),
         SPATIAL_DATA_WINDOW));

      SpatialDataView* pView = (pWindow == NULL) ? NULL : pWindow->getSpatialDataView();
      if (pView == NULL)
      {
         std::string msg = "Unable to create view.";
         pStep->finalize(Message::Failure, msg);
         if (pProgress != NULL) 
         {
            pProgress->updateProgress(msg, 0, ERRORS);
         }
         return false;
      }
      pView->setPrimaryRasterElement(pResultCube.get());
      pView->createLayer(RASTER, pResultCube.get());
   }

   if (pProgress != NULL)
   {
      pProgress->updateProgress("RX anomaly detection is compete.", 100, NORMAL);
   }
	
   pOutArgList->setPlugInArgValue("RX_Anomaly_Detection_Result", pResultCube.release());
   pStep->finalize();

   return true;
}
