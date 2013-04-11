// SVN Info: $Id: TOF1Expo.cxx 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1Expo                                                             //
//                                                                      //
// TOF1 based function of an exponential.                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1Expo.h"

ClassImp(TOF1Expo)


//______________________________________________________________________________
TOF1Expo::TOF1Expo(const Char_t* name, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, 2)
{
    // Constructor.
    
    // set parameter names
    SetParNames("Constant", "Slope");
}

//______________________________________________________________________________
Double_t TOF1Expo::Evaluate(Double_t* x, Double_t* par)
{
    // Exponential function implementation.

    return TMath::Exp(par[0]+par[1]*x[0]);
}

