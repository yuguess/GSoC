/*
 * The information in this file is
 * Copyright(c) 2010 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */
#include "AoiElement.h"
#include "AppConfig.h"
#include "AppAssert.h"
#include "AppVerify.h"
#include "BitMask.h"
#include "BitMaskIterator.h"
#include "DynamicObject.h"
#include "ObjectResource.h"
#include "PlugInArgList.h"
#include "PlugInRegistration.h"
#include "PlugInResource.h"
#include "ProgressTracker.h"
#include "Resampler.h"
#include "AceInputs.h"
#include "AceAlg.h"
#include "AceErr.h"
#include "Signature.h"
#include "SpectralUtilities.h"
#include "SpectralVersion.h"
#include "Statistics.h"
#include "switchOnEncoding.h"
#include "Wavelengths.h"
#include "DataAccessor.h"
#include "DataAccessorImpl.h"
#include "DataRequest.h"
#include "DesktopServices.h"
#include "RasterElement.h"
#include "RasterDataDescriptor.h"
#include "RasterUtilities.h"
#include "MatrixFunctions.h"
#include "SpatialDataWindow.h"
#include "SpatialDataView.h"
#include "tnt_cmat.h"
#include "tnt_array1d.h"
#include <Qt/QtGui>
#include <climits>

using namespace std;

namespace 
{

   template <class T>
   bool aceAlg(T* pData, RasterElement *pElement, RasterElement *pResults, vector<double> targetSpectrum, Progress* pProgress)
   {
		
	   RasterDataDescriptor* pDescriptor = static_cast<RasterDataDescriptor*>(pElement->getDataDescriptor());

	   FactoryResource<DataRequest> pRequest;
	   pRequest->setInterleaveFormat(BIP);
	   DataAccessor accessor = pElement->getDataAccessor(pRequest.release());
	   if (!accessor.isValid())
		   return false;

	   int rowCount = pDescriptor->getRowCount();
	   int colCount = pDescriptor->getColumnCount();
	   int bandCount = pDescriptor->getBandCount(); 

	   TNT::Matrix<double> background(bandCount, rowCount * colCount, 0.0);
	   TNT::Matrix<double> transBackground(rowCount * colCount, bandCount, 0.0);
	   TNT::Matrix<double> tempBackground(rowCount * colCount, bandCount, 0.0);
	   TNT::Matrix<double> covMatrix(bandCount, bandCount, 0.0);
	   TNT::Matrix<double> invertCovMatrix(bandCount, bandCount, 0.0);
	   TNT::Matrix<double> target(bandCount, 1, 0.0);
	   TNT::Matrix<double> transTarget(1, bandCount, 0.0);
	   TNT::Matrix<double> means(bandCount, 1, 0.0);

	   for (int row = 0; row < rowCount; row++)   		
	   {
		   for (int col = 0; col < colCount; col++)	
		   {		
			   for (int band = 0; band < bandCount; band++)
			   {

				   T *mpData = reinterpret_cast<T*>(accessor->getColumn());
				   background[band][colCount * row + col] = mpData[band];  
			   }
			   accessor->nextColumn();		
		   }
		   accessor->nextRow();
		   pProgress->updateProgress("Initializing Step 1 of 4", (row+1) * 100 / rowCount, NORMAL);
	   } 

	   for (int band = 0; band < bandCount; band++)
	   {
		   target[band][0] = targetSpectrum[band];
	   }	 

	   for (int row = 0; row < background.num_rows(); row++)
	   {
		   for (int col = 0; col < background.num_cols(); col++) 
		   {
			   means[row][0] += background[row][col]; 	
		   }
		   means[row][0] /= background.num_cols();
	   }



	   for (int row = 0; row < background.num_rows(); row++) 
	   {
		   for (int col = 0; col < background.num_cols(); col++)
		   {
			   background[row][col] -= means[row][0];  		 
		   } 	
	   }

	   for (int row = 0; row < target.num_rows(); row++)
	   {
		   target[row][0] -= means[row][0];  		 
	   } 	

	   transBackground = TNT::transpose(background);

	   covMatrix = matmult(background, transBackground);
	
	   for (int row = 0; row < covMatrix.num_rows(); row++) 
	   {
		   for (int col = 0; col < covMatrix.num_cols(); col++)
		   {
			   covMatrix[row][col] /= (rowCount * colCount); 
		   }
		   pProgress->updateProgress("Calculating Step 2 of 4", (row + 1) * 100 / covMatrix.num_rows(), NORMAL);
	   }

	   double **pSrc = covMatrix;
	   double **pDesc = invertCovMatrix;
	   int rows = covMatrix.num_rows();

	   if (!MatrixFunctions::invertSquareMatrix2D(pDesc, const_cast<const double**>(pSrc), rows))
	   {
		   return false;
	   }

	   transTarget = TNT::transpose(target);

	   TNT::Matrix<double> numeratorMat(1, rowCount * colCount, 0.0);

	   numeratorMat = matmult(matmult(transTarget, invertCovMatrix), background);

	   for (int col = 0; col < numeratorMat.num_cols(); col++) 
	   {
		   numeratorMat[0][col] = numeratorMat[0][col] * numeratorMat[0][col];
	   }

	   tempBackground = matmult(transBackground, invertCovMatrix);

	   for (int row = 0; row < transBackground.num_rows(); row++)
		   for (int col = 0; col < transBackground.num_cols(); col++)
		   {
			   transBackground[row][col] *= tempBackground[row][col]; 
		   }


	   TNT::Matrix<double> denominatorMat(rowCount * colCount, 1, 0.0);

	   for (int row = 0; row < transBackground.num_rows(); row++)
	   {
			for (int col = 0; col < transBackground.num_cols(); col++)
			{
				   denominatorMat[row][0] += transBackground[row][col];
			}
			pProgress->updateProgress("Calculating Step 3 of 4", (row + 1) * 100 / transBackground.num_rows(), NORMAL);
	   }

	   TNT::Matrix<double> singleMatrix(1, 1, 0.0);
	   singleMatrix = matmult(matmult(transTarget, invertCovMatrix), target);

	   for (int row = 0; row < denominatorMat.num_rows(); row++)
	   {
		   denominatorMat[row][0] *= singleMatrix[0][0];
	   }

	   for (int col = 0; col < numeratorMat.num_cols(); col++)
	   {
		   numeratorMat[0][col] /= denominatorMat[col][0];
	   }	

	   TNT::Matrix<double> result(colCount, rowCount, 0.0);
	  
	   double pixMax = INT_MIN;
	   double pixMin = INT_MAX;
	   for(int i = 0; i < rowCount; i++)
		   for(int j = 0; j < colCount; j++)
		   {
				result[j][i] = numeratorMat[0][i * colCount + j]; 
				if (result[j][i] > pixMax)
					pixMax = result[j][i];
				if (result[j][i] < pixMin)
					pixMin = result[j][i];
		   }
		   /*
	   double temp = pElement->getStatistics()->getMax();
	   FILE *fp;
	   fp = fopen("D://Max.txt", "w");
	   fprintf(fp, "%lf", temp);
	   fclose(fp);*/
	   for(int j = 0; j < colCount; j++)
		   for(int i = 0; i < rowCount; i++)
		   {
			   result[j][i] = ((result[j][i] - pixMin) / (pixMax - pixMin)) * 10;	
		   } 
		  
/*
      double threhold = 0.5;
	  for(int col = 0; col < colCount; col++)
		   for(int row = 0; row < rowCount; row++)
		   {
				if (result[row][col] > 0.2)
				    result[row][col] = 1.0;
				else
					result[row][col] = 0.0;
		   } 
*/
	   FactoryResource<DataRequest> pRequest2;
	   pRequest2->setWritable(true);
	   DataAccessor accessor2 = pResults->getDataAccessor(pRequest2.release());

	   if (!accessor2.isValid())
		   return false;
	   
	   for (int col = 0; col < colCount; col++)
	   {
			for (int row = 0; row < rowCount; row++)
			{
			   accessor2->toPixel(row, col);
			   float *mpData = reinterpret_cast<float*>(accessor2->getColumn());
			   (*mpData) = (float)result[col][row]; 
			}
			pProgress->updateProgress("Calculating Step 4 of 4", (col + 1) * 100 / colCount, NORMAL);
	   }
	   return true;
   }
}

AceAlgorithm::AceAlgorithm(RasterElement* pElement, Progress* pProgress, bool interactive, const BitMask* pAoi) :
               AlgorithmPattern(pElement, pProgress, interactive, pAoi),
               mpResults(NULL),
               mAbortFlag(false)
{
	mpProgress = pProgress;
}

bool AceAlgorithm::preprocess()
{
   return true;
}

bool AceAlgorithm::processAll()
{
   auto_ptr<Wavelengths> pWavelengths;

   ProgressTracker progress(getProgress(), "Starting Ace", "spectral", "C4320027-6359-4F5B-8820-8BC72BF1B8F0");
   RasterElement* pElement = getRasterElement();
   if (pElement == NULL)
   {
      progress.report(ACEERR012, 0, ERRORS, true);
      return false;
   }

   const RasterDataDescriptor* pDescriptor = static_cast<RasterDataDescriptor*>(pElement->getDataDescriptor());
   VERIFY(pDescriptor != NULL);

   BitMaskIterator iter(getPixelsToProcess(), pElement);
   unsigned int numRows = iter.getNumSelectedRows();
   unsigned int numColumns = iter.getNumSelectedColumns();
   unsigned int numBands = pDescriptor->getBandCount();

   Opticks::PixelOffset layerOffset(iter.getColumnOffset(), iter.getRowOffset());

   // get cube wavelengths
   DynamicObject* pMetadata = pElement->getMetadata();
   if (pMetadata != NULL)
   {
      pWavelengths.reset(new Wavelengths(pMetadata));
      if (!pWavelengths->isEmpty() && (!pWavelengths->hasEndValues() || !pWavelengths->hasStartValues()))
      {
         pWavelengths->calculateFwhm();
      }
   }
   VERIFY(pWavelengths.get() != NULL);

   int sig_index = 0;
   bool bSuccess = true;

   if (mInputs.mSignatures.empty())
   {
      progress.report(ACEERR005, 0, ERRORS, true);
      return false;
   }

   int iSignatureCount = mInputs.mSignatures.size();

   // Create a vector for the signature names
   vector<string> sigNames;

   RasterElement* pResults = NULL;
   bool resultsIsTemp = false;

   Signature* pSignature = mInputs.mSignatures[sig_index];

   sigNames.push_back(pSignature->getName());
   std::string rname = mInputs.mResultsName;
   if (iSignatureCount > 1 && !mInputs.mbCreatePseudocolor)
   {
	   rname += " " + sigNames.back();
   }
   else if (iSignatureCount > 1)
   {
	   rname += "AceTemp";
	   resultsIsTemp = true;
   }

   pResults = createResults(numRows, numColumns, rname);
   if (pResults == NULL)
   {
	   return false;
   }

   vector<double> spectrumValues;
   vector<int> resampledBands;
   bSuccess = resampleSpectrum(pSignature, spectrumValues, *pWavelengths.get(), resampledBands);

   // Check for limited spectral coverage and warning log 
   if (bSuccess && pWavelengths->hasCenterValues() &&
	   resampledBands.size() != pWavelengths->getCenterValues().size())
   {
	   QString buf = QString("Warning AceAlg014: The spectrum only provides spectral coverage for %1 of %2 bands.")
		   .arg(resampledBands.size()).arg(pWavelengths->getCenterValues().size());
	   progress.report(buf.toStdString(), 0, WARNING, true);
   }

   BitMaskIterator iterChecker(getPixelsToProcess(), pElement);

   EncodingType type = pDescriptor->getDataType();		 
   switchOnEncoding(type, aceAlg, NULL, pElement, pResults, spectrumValues, mpProgress); 

/////////////////////////////////////////////////////
   vector<ColorType> layerColors, excludeColors;
   excludeColors.push_back(ColorType(0, 0, 0));
   excludeColors.push_back(ColorType(255, 255, 255));
   ColorType::getUniqueColors(iSignatureCount, layerColors, excludeColors);

   ColorType color;
   if (0 <= static_cast<int>(layerColors.size()))
   {
	   color = layerColors[0];
   }

   double dMaxValue = pResults->getStatistics()->getMax();

   // Displays results for current signature
   displayThresholdResults(pResults, color, UPPER, mInputs.mThreshold, dMaxValue, layerOffset);
 //  displayThresholdResults(pResults, color, UPPER, 0.6, dMaxValue, layerOffset);
//////////////////////////////////////////////////////
/*
   Service<DesktopServices> pDesktop;

   SpatialDataWindow* pWindow = static_cast<SpatialDataWindow*>(pDesktop->createWindow(rname,
	   SPATIAL_DATA_WINDOW));

   SpatialDataView* pView = (pWindow == NULL) ? NULL : pWindow->getSpatialDataView();
   if (pView == NULL)
   {
	   std::string msg = "Unable to create view.";
	   progress.report(msg, 0, ERRORS, true);
	   return false;
   }

   pView->setPrimaryRasterElement(pResults);
   pView->createLayer(RASTER, pResults);
*/
	return true;
}

bool AceAlgorithm::resampleSpectrum(Signature* pSignature, 
                                    vector<double>& resampledAmplitude,
                                    const Wavelengths& wavelengths, 
                                    vector<int>& resampledBands)
{
   StepResource pStep("Resample Signature", "spectral", "E1C6F0EA-4D00-4B0E-851F-F677A479169D");

   Progress* pProgress = getProgress();
   if (wavelengths.isEmpty())
   {
      // Check for an in-scene signature
      RasterElement* pElement = getRasterElement();
      VERIFY(pElement != NULL);

      if (pSignature->getParent() == pElement)
      {
         vector<double> sigReflectances =
            dv_cast<vector<double> >(pSignature->getData("Reflectance"), vector<double>());
         resampledAmplitude = sigReflectances;

         resampledBands.clear();
         for (vector<double>::size_type i = 0; i < sigReflectances.size(); ++i)
         {
            resampledBands.push_back(static_cast<int>(i));
         }

         pStep->finalize(Message::Success);
         return true;
      }

      string messageText = "The data set wavelengths are invalid.";
      if (pProgress != NULL) pProgress->updateProgress(messageText, 0, ERRORS);
      pStep->finalize(Message::Failure, messageText);
      return false;
   }

   vector<double> fwhm = const_cast<Wavelengths&>(wavelengths).getFwhm();
   PlugInResource resampler("Resampler");
   Resampler* pResampler = dynamic_cast<Resampler*>(resampler.get());
   if (pResampler == NULL)
   {
      string messageText = "The resampler plug-in could not be created.";
      if (pProgress != NULL) pProgress->updateProgress(messageText, 0, ERRORS);
      pStep->finalize(Message::Failure, messageText);
      return false;
   }
   string err;
   try
   {
      vector<double> sigReflectance = dv_cast<vector<double> >(pSignature->getData("Reflectance"));
      resampledAmplitude.reserve(sigReflectance.size());
      resampledBands.reserve(sigReflectance.size());
      if (!pResampler->execute(sigReflectance,
                              resampledAmplitude,
                              dv_cast<vector<double> >(pSignature->getData("Wavelength")),
                              wavelengths.getCenterValues(),
                              fwhm,
                              resampledBands,
                              err))
      {
         string messageText = "Resampling failed: " + err;
         if (pProgress != NULL) pProgress->updateProgress(messageText, 0, ERRORS);
         pStep->finalize(Message::Failure, messageText);
         return false;
      }
   }
   catch(std::bad_cast&)
   {
      string messageText = "Resampling failed: " + err;
      if (pProgress != NULL) pProgress->updateProgress(messageText, 0, ERRORS);
      pStep->finalize(Message::Failure, messageText);
      return false;
   }

   pStep->finalize(Message::Success);
   return true;
}

RasterElement* AceAlgorithm::createResults(int numRows, int numColumns, const string& sigName)
{
   RasterElement* pElement = getRasterElement();
   if (pElement == NULL)
   {
      return NULL;
   }

   // Delete an existing element to ensure that the new results element is the correct size
   Service<ModelServices> pModel;

   RasterElement* pExistingResults = static_cast<RasterElement*>(pModel->getElement(sigName,
      TypeConverter::toString<RasterElement>(), pElement));
   if (pExistingResults != NULL)
   {
      pModel->destroyElement(pExistingResults);
   }

   // Create the new results element
   ModelResource<RasterElement> pResults(RasterUtilities::createRasterElement(sigName, numRows, numColumns,
      FLT4BYTES, true, pElement));
   if (pResults.get() == NULL)
   {
      pResults = ModelResource<RasterElement>(RasterUtilities::createRasterElement(sigName, numRows, numColumns,
         FLT4BYTES, false, pElement));
      if (pResults.get() == NULL)
      {
         reportProgress(ERRORS, 0, ACEERR009);
         MessageResource(ACEERR009, "spectral", "C89D361B-DB12-43ED-B276-6D98CA3539EE");
         return NULL;
      }
   }

   FactoryResource<Units> pUnits;
   pUnits->setUnitName("degrees");

   vector<int> badValues(1, 181);

   RasterDataDescriptor* pResultsDescriptor = static_cast<RasterDataDescriptor*>(pResults->getDataDescriptor());
   VERIFYRV(pResultsDescriptor != NULL, NULL);
   pResultsDescriptor->setUnits(pUnits.get());
   pResultsDescriptor->setBadValues(badValues);

   Statistics* pStatistics = pResults->getStatistics();
   VERIFYRV(pStatistics != NULL, NULL);
   pStatistics->setBadValues(badValues);

   return pResults.release();
}

bool AceAlgorithm::postprocess()
{
   return true;
}

bool AceAlgorithm::initialize(void* pAlgorithmData)
{
   bool bSuccess = true;
   if (pAlgorithmData != NULL)
   {
      mInputs = *reinterpret_cast<AceInputs*>(pAlgorithmData);
   }

   if (mInputs.mSignatures.empty())
   {
      reportProgress(ERRORS, 0, ACEERR011);
      MessageResource(ACEERR011, "spectral", "07592D6A-50B9-48D3-86FD-329608F6537B");
      bSuccess = false;
   }

   const BitMask* pAoi = NULL;
   if (mInputs.mpAoi != NULL)
   {
      pAoi = mInputs.mpAoi->getSelectedPoints();
   }
   setRoi(pAoi);

   return bSuccess;
}

RasterElement* AceAlgorithm::getResults() const
{
   return mpResults;
}

bool AceAlgorithm::canAbort() const
{
   return true;
}

bool AceAlgorithm::doAbort()
{
   mAbortFlag = true;
   return true;
}