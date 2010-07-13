/*
 * The information in this file is
 * Copyright(c) 2010 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef ACE_H
#define ACE_H

#include "AlgorithmPattern.h"
#include "AlgorithmShell.h"

#include "ProgressTracker.h"

#include "AceInputs.h"

#include <math.h>
#include <string>
#include <vector>


class BitMaskIterator;
class Progress;
class Signature;
class AceDlg;
class Wavelengths;
class AceAlgorithm;

class Ace : public AlgorithmPlugIn
{
public:
   Ace();
   ~Ace();

private:
   bool canRunBatch() const { return true; }
   bool canRunInteractive() const { return true; }
   bool populateBatchInputArgList(PlugInArgList* pInArgList);
   bool populateInteractiveInputArgList(PlugInArgList* pInArgList);
   bool populateDefaultOutputArgList(PlugInArgList* pOutArgList);
   bool setActualValuesInOutputArgList(PlugInArgList* pOutArgList);
   bool parseInputArgList(PlugInArgList* pInArgList);
   QDialog* getGui(void* pAlgData);
   void propagateAbort();
   bool extractFromGui();

   ProgressTracker progress;
   Progress *pProgress;
   AceInputs inputs;
   AceDlg *pAceGui;
   AceAlgorithm *pAceAlg;
};

#endif