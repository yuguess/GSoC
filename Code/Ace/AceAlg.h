/*
 * The information in this file is
 * Copyright(c) 2010 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */
#include "MultiThreadedAlgorithm.h"
#include "AlgorithmPattern.h"
#include "AlgorithmShell.h"

#include "ProgressTracker.h"
#include "AceInputs.h"

#include <iostream>
#include <math.h>
#include <string>
#include <vector>


class Signature;
class Wavelengths;

struct AceAlgInput
{
   AceAlgInput(const RasterElement* pCube,
      RasterElement* pResultsMatrix,
      const std::vector<double>& spectrum,
      const bool* pAbortFlag, 
      const BitMaskIterator& iterCheck,
      const std::vector<int>& resampledBands) : 
	  mpCube(pCube),
      mpResultsMatrix(pResultsMatrix),
      mSpectrum(spectrum),
      mpAbortFlag(pAbortFlag),
      mIterCheck(iterCheck),
      mResampledBands(resampledBands)
   {
   }

   ~AceAlgInput()
   {
   }

   const RasterElement* mpCube;
   RasterElement* mpResultsMatrix;
   const std::vector<double>& mSpectrum;
   const bool* mpAbortFlag;
   const BitMaskIterator& mIterCheck;
   const std::vector<int>& mResampledBands;
};

class AceAlgorithm : public AlgorithmPattern
{
   bool preprocess();
   bool processAll();
   bool postprocess();
   bool initialize(void* pAlgorithmData);
   RasterElement* createResults(int numRows, int numColumns, const std::string& sigName);
   bool resampleSpectrum(Signature* pSignature, std::vector<double>& resampledAmplitude, 
      const Wavelengths& wavelengths, std::vector<int>& resampledBands);
   bool canAbort() const;
   bool doAbort();

   RasterElement* mpResults;
   AceInputs mInputs;
   bool mAbortFlag;

public:
   AceAlgorithm(RasterElement* pElement, Progress* pProgress, bool interactive, const BitMask* pAoi);
   RasterElement* getResults() const;
};