// SVN Info: $Id: TOF1Gauss.cxx 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1Gauss                                                            //
//                                                                      //
// TOF1 based Gaussian function.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1Gauss.h"

ClassImp(TOF1Gauss)


//______________________________________________________________________________
TOF1Gauss::TOF1Gauss(const Char_t* name, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, 3)
{
    // Constructor.
    
    // set parameter names
    SetParNames("Constant", "Mean", "Sigma");
}
 
//______________________________________________________________________________
Double_t TOF1Gauss::Evaluate(Double_t* x, Double_t* par)
{
    // Gaussian function implementation.
    //
    // Parameters:
    // par[0] : Constant
    // par[1] : Mean
    // par[2] : Sigma

    return par[0] * TMath::Gaus(x[0], par[1], par[2]);
}

