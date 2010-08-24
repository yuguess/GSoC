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
#include "DataAccessor.h"
#include "DataAccessorImpl.h"
#include "DataRequest.h"
#include "DesktopServices.h"
#include "DynamicObject.h"
#include "ObjectResource.h"
#include "PlugInArgList.h"
#include "PlugInRegistration.h"
#include "PlugInResource.h"
#include "ProgressTracker.h"
#include "RasterDataDescriptor.h"
#include "RasterElement.h"
#include "RasterUtilities.h"
#include "Resampler.h"
#include "Ace.h"
#include "AceDlg.h"
#include "AceAlg.h"
#include "AceErr.h"
#include "Signature.h"
#include "SpectralUtilities.h"
#include "SpectralVersion.h"
#include "Statistics.h"
#include "switchOnEncoding.h"
#include "Wavelengths.h"

using namespace std;

REGISTER_PLUGIN_BASIC(SpectralAce, Ace);

Ace::Ace() : AlgorithmPlugIn(&inputs), pAceGui(NULL), pAceAlg(NULL), pProgress(NULL)
{
   setDescriptorId("{D202C405-0F25-46A9-9C1D-A436EC5D3288}");
   setName("ACE");
   setCreator("Ball Aerospace & Technologies Corp.");
   setShortDescription("Adaptice Cosine Estimator");
   setDescription("Compute cosine estimator for target detection against target spectral");
   setCopyright(SPECTRAL_COPYRIGHT);
   setVersion(SPECTRAL_VERSION_NUMBER);
   setProductionStatus(SPECTRAL_IS_PRODUCTION_RELEASE);
   setMenuLocation("[Spectral]\\Targe Detection\\ACE");
   setAbortSupported(true);
}

Ace::~Ace()
{
}

bool Ace::populateBatchInputArgList(PlugInArgList* pInArgList)
{
   VERIFY(pInArgList != NULL);
   VERIFY(pInArgList->addArg<Progress>(Executable::ProgressArg(), NULL));
   VERIFY(pInArgList->addArg<RasterElement>(Executable::DataElementArg()));
   VERIFY(pInArgList->addArg<Signature>("Target Signatures", NULL));
   VERIFY(pInArgList->addArg<double>("Threshold", inputs.mThreshold));
   VERIFY(pInArgList->addArg<AoiElement>("AOI", inputs.mpAoi));
   VERIFY(pInArgList->addArg<bool>("Display Results", inputs.mbDisplayResults));
   VERIFY(pInArgList->addArg<string>("Results Name", inputs.mResultsName));
   return true;
}
   
bool Ace::populateInteractiveInputArgList(PlugInArgList* pInArgList)
{
   VERIFY(pInArgList != NULL);
   VERIFY(pInArgList->addArg<Progress>(Executable::ProgressArg(), NULL));
   VERIFY(pInArgList->addArg<RasterElement>(Executable::DataElementArg()));
   return true;
}

bool Ace::populateDefaultOutputArgList(PlugInArgList* pOutArgList)
{
   VERIFY(pOutArgList->addArg<RasterElement>("Ace Results"));
   return true;
}

bool Ace::parseInputArgList(PlugInArgList* pInArgList)
{
   pProgress = pInArgList->getPlugInArgValue<Progress>(Executable::ProgressArg());
   progress = ProgressTracker(pProgress, "Adaptive Cosine Estimator",
                              "spectral", "82DDE066-46DA-4241-94EE-5E3D16B5358E");
   RasterElement* pElement = pInArgList->getPlugInArgValue<RasterElement>(Executable::DataElementArg());
   if (pElement == NULL)
   {
      progress.report(ACEERR001, 0, ERRORS, true);
      return false;
   }

   const RasterDataDescriptor* pElementDescriptor = static_cast<RasterDataDescriptor*>(pElement->getDataDescriptor());
   EncodingType dataType = pElementDescriptor->getDataType();
   if (dataType == INT4SCOMPLEX || dataType == FLT8COMPLEX)
   {
      progress.report(ACEERR013, 0, ERRORS, true);
      return false;
   }

   if (pElementDescriptor->getBandCount() == 1)
   {
      progress.report(ACEERR014, 0, ERRORS, true);
      return false;
   }

   Signature* pSignatures = NULL;
   if (!isInteractive())
   {
      pSignatures = pInArgList->getPlugInArgValue<Signature>("Target Signatures");
      VERIFY(pInArgList->getPlugInArgValue("Threshold", inputs.mThreshold));
      inputs.mpAoi = pInArgList->getPlugInArgValue<AoiElement>("AOI");
      VERIFY(pInArgList->getPlugInArgValue("Display Results", inputs.mbDisplayResults));
      VERIFY(pInArgList->getPlugInArgValue("Results Name", inputs.mResultsName));

      inputs.mSignatures = SpectralUtilities::extractSignatures(vector<Signature*>(1, pSignatures));
   }

   const BitMask* pBitMask = NULL;
   if (inputs.mpAoi != NULL)
   {
      pBitMask = inputs.mpAoi->getSelectedPoints();
   }

   pAceAlg = new AceAlgorithm(pElement, pProgress, isInteractive(), pBitMask);
   setAlgorithmPattern(Resource<AlgorithmPattern>(pAceAlg));
   return true;
}

bool Ace::setActualValuesInOutputArgList(PlugInArgList* pOutArgList)
{
   VERIFY(pOutArgList->setPlugInArgValue("Ace Results", pAceAlg->getResults()));
   progress.upALevel(); 
   return true;
}

QDialog* Ace::getGui(void* pAlgData)
{
    pAceGui = new AceDlg(pAceAlg->getRasterElement(), this, pProgress,
      inputs.mResultsName, inputs.mbCreatePseudocolor, false, NULL,
      Service<DesktopServices>()->getMainWidget());
	pAceGui->setWindowTitle("Adaptive Cosine Estimator");
	pAceGui->setThreshold(0.1);
   return pAceGui;
}

void Ace::propagateAbort()
{
   if (pAceGui != NULL)
   {
      pAceGui->abortSearch();
   }
}

bool Ace::extractFromGui()
{
   if (pAceGui == NULL)
   {
      return false;
   }
   inputs.mThreshold = pAceGui->getThreshold();
   inputs.mSignatures = pAceGui->getExtractedSignatures();
   inputs.mResultsName = pAceGui->getResultsName();
   inputs.mpAoi = pAceGui->getAoi();
   inputs.mbCreatePseudocolor = pAceGui->isPseudocolorLayerUsed();

   if (inputs.mResultsName.empty())
   {
      inputs.mResultsName = "Ace Results";
   }
   return true;
}