// SVN Info: $Id: TOF1LegPol.cxx 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1LegPol                                                           //
//                                                                      //
// TOF1 based function of Legendre polynomials.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1LegPol.h"

ClassImp(TOF1LegPol)


//______________________________________________________________________________
TOF1LegPol::TOF1LegPol(const Char_t* name, Double_t xmin, Double_t xmax, Int_t order)
    : TOF1(name, xmin, xmax, order+1)
{
    // Constructor.
    
    Char_t tmp[256];

    // init members
    fNOrder = order;
    fScaling = 1.;

    // set parameter names
    for (Int_t i = 0; i < fNOrder+1; i++)
    {
        sprintf(tmp, "LegPolCoeff_%d", i);
        SetParName(i, tmp);
    }
}

//______________________________________________________________________________
Double_t TOF1LegPol::Evaluate(Double_t* x, Double_t* par)
{
    // Legendre polynomial function implementation.

    // select order
    switch (fNOrder)
    {
        case 0:
            return fScaling * 
                   par[0];
        case 1:
            return fScaling *
                   ( par[0] +
                     par[1]*x[0] );
        case 2:
            return fScaling *
                   ( par[0] +
                     par[1]*x[0] +
                     par[2]*0.5*(3.*x[0]*x[0]-1) );
        case 3:
            return fScaling * 
                   ( par[0] +
                     par[1]*x[0] +
                     par[2]*0.5*(3.*x[0]*x[0]-1) +
                     par[3]*0.5*(5.*x[0]*x[0]*x[0]-3.*x[0]) );
         case 4:
            return fScaling * 
                   ( par[0] +
                     par[1]*x[0] +
                     par[2]*0.5*(3.*x[0]*x[0]-1) +
                     par[3]*0.5*(5.*x[0]*x[0]*x[0]-3.*x[0]) + 
                     par[4]*0.125*(35.*x[0]*x[0]*x[0]*x[0]-30.*x[0]*x[0]+3) );
        default:
        {
            Error("Evaluate", "Legendre polynomial of order %d is not yet implemented!", fNOrder);
            return 0;
        }
    }
}

//______________________________________________________________________________
void TOF1LegPol::Express(Char_t* outString)
{
    // Write the function expression to 'outString'.
 
    sprintf(outString, "%e*legpol%d", fScaling, fNOrder);
}

