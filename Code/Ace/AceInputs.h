/*
 * The information in this file is
 * Copyright(c) 2010 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */
#ifndef ACEINPUTS_H
#define ACEINPUTS_H

class AoiElement;
class Signature;

struct AceInputs
{
   AceInputs() : mThreshold(5.0),
                 mbDisplayResults(false),
                 mResultsName("Ace Results"),
                 mpAoi(NULL),
                 mbCreatePseudocolor(true) {}
   std::vector<Signature*> mSignatures;
   double mThreshold;
   bool mbDisplayResults;
   std::string mResultsName;
   AoiElement* mpAoi;
   bool mbCreatePseudocolor;
};

#endif