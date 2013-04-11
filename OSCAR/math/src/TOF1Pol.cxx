// SVN Info: $Id: TOF1Pol.cxx 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1Pol                                                              //
//                                                                      //
// TOF1 based function of polynomials.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1Pol.h"

ClassImp(TOF1Pol)


//______________________________________________________________________________
TOF1Pol::TOF1Pol(const Char_t* name, Double_t xmin, Double_t xmax, Int_t order)
    : TOF1(name, xmin, xmax, order+1)
{
    // Constructor.
    
    Char_t tmp[256];

    // init members
    fNOrder = order;

    // set parameter names
    for (Int_t i = 0; i < fNOrder+1; i++)
    {
        sprintf(tmp, "PolCoeff_%d", i);
        SetParName(i, tmp);
    }
}

//______________________________________________________________________________
Double_t TOF1Pol::Evaluate(Double_t* x, Double_t* par)
{
    // Polynomial function implementation.

    // select order
    switch (fNOrder)
    {
        case 0:
            return par[0];
        case 1:
            return par[0] +
                   par[1]*x[0];
        case 2:
            return par[0] +
                   par[1]*x[0] +
                   par[2]*x[0]*x[0];
        case 3:
            return par[0] +
                   par[1]*x[0] +
                   par[2]*x[0]*x[0] +
                   par[3]*x[0]*x[0]*x[0];
        case 4:
            return par[0] +
                   par[1]*x[0] +
                   par[2]*x[0]*x[0] +
                   par[3]*x[0]*x[0]*x[0] +
                   par[4]*x[0]*x[0]*x[0]*x[0];
        case 5:
            return par[0] +
                   par[1]*x[0] +
                   par[2]*x[0]*x[0] +
                   par[3]*x[0]*x[0]*x[0] +
                   par[4]*x[0]*x[0]*x[0]*x[0] +
                   par[5]*x[0]*x[0]*x[0]*x[0]*x[0];
        default:
        {
            Error("Evaluate", "Polynomial of order %d is not yet implemented!", fNOrder);
            return 0;
        }
    }
}

//______________________________________________________________________________
void TOF1Pol::Express(Char_t* outString)
{
    // Write the function expression to 'outString'.
 
    sprintf(outString, "pol%d", fNOrder);
}

