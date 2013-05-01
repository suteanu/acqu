//*************************************************************************
//* Author: Patrik Ott
//*************************************************************************

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TCFitTools                                                           //
//                                                                      //
// Some Fit Tools for the calibration.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TCFITTOOLS_H
#define TCFITTOOLS_H

#include "TSystem.h"
#include "TH1D.h"
#include "TSpectrum.h"



namespace TCUtils
{
	Double_t		PedFinder(const TH1D* hist);
}

#endif
