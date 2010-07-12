/*
 * The information in this file is
 * Copyright(c) 2010 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "ConfigurationSettings.h"
#include "DesktopServices.h"
#include "Ace.h"
#include "AceDlg.h"

using namespace std;

AceDlg::AceDlg(RasterElement* pCube, AlgorithmRunner* pRunner, Progress* pProgress,
       const string& resultsName, bool pseudocolor, bool addApply, bool contextHelp, QWidget* pParent) :
   SpectralSignatureSelector(pCube, pRunner, pProgress, resultsName, pseudocolor,
      addApply, pParent, (contextHelp ? "Help" : string()) )
{
}

AceDlg::~AceDlg()
{
}

void AceDlg::customButtonClicked()
{
   Service<DesktopServices> pDesktop;
   Service<ConfigurationSettings> pSettings;

//   string helpFile = pSettings->getHome() + Sam::getSettingSamHelp();
//   pDesktop->displayHelp(helpFile);
}
