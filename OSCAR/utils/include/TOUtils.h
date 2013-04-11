// SVN Info: $Id: TOUtils.h 1630 2013-01-24 13:54:58Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOUtils                                                              //
//                                                                      //
// This namespace contains some often used utility functions.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOUtils
#define OSCAR_TOUtils

#include "TText.h"
#include "TMath.h"

#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif


namespace TOUtils
{
    Int_t GetPositionInArray(Int_t e, Int_t n, Int_t* a);
 
    void CalculateMeanErrorWeights(Int_t n, Double_t* v, Double_t* e, 
                                   Double_t* outMean, Double_t* outMeanErr);
    Int_t Round(Double_t x);

    Int_t GetNumberOfCPUs();

    void PlotPreliminary(Double_t textSize = 0.2);
}

#endif

