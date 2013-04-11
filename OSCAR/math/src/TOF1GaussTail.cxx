// SVN Info: $Id: TOF1GaussTail.cxx 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1GaussTail                                                        //
//                                                                      //
// TOF1 based Gaussian function with low value tail.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1GaussTail.h"

ClassImp(TOF1GaussTail)


//______________________________________________________________________________
TOF1GaussTail::TOF1GaussTail(const Char_t* name, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, 5)
{
    // Constructor.
    
    // set parameter names
    SetParNames("Norm", "Mean", "FWHM", "Lambda", "Log-Factor");
}
 
//______________________________________________________________________________
Double_t TOF1GaussTail::Evaluate(Double_t* x, Double_t* par)
{
    // Gaussian with tail function implementation.
    //
    // Parameters:
    // par[0] : Norm
    // par[1] : E_peak
    // par[2] : Gamma
    // par[3] : lambda
    // par[4] : log factor
    
    Double_t eDiff = x[0] - par[1];
    Double_t G = TMath::Exp(-4. * TMath::Log(par[4]) * eDiff * eDiff / par[2] / par[2]);

    if (x[0] >= par[1]) return par[0] * G;                                  // above peak position 
    else return par[0] * (G + TMath::Exp(eDiff / par[3]) * (1. - G));       // below peak position -> tail
}

