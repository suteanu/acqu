// SVN Info: $Id: TOF1BW.cxx 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1BW                                                               //
//                                                                      //
// TOF1 based (non-)relativistic Breit-Wigner function (Perkins).       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1BW.h"

ClassImp(TOF1BW)


//______________________________________________________________________________
TOF1BW::TOF1BW(const Char_t* name, Double_t xmin, Double_t xmax, 
               Bool_t isRel, Bool_t isGConv)
    : TOF1(name, xmin, xmax, 3)
{
    // Constructor.

    // init members
    fIsRel = isRel;
    fIsGConv = isGConv;
    fGConvNstep = 1000;
    fGConvSigma = 1;

    // set parameter names
    SetParNames("Constant", "Mean", "Gamma");
}

//______________________________________________________________________________
Double_t TOF1BW::EvalRel(Double_t* x, Double_t* par)
{
    // Evaluate the relativistic Breit-Wigner function (Perkins).
    
    Double_t a = x[0]*x[0] - par[1]*par[1];
    Double_t b = par[1]*par[1]*par[2]*par[2];
    
    return par[0] * b / (a*a + b);
}

//______________________________________________________________________________
Double_t TOF1BW::EvalNonRel(Double_t* x, Double_t* par)
{
    // Evaluate the non-relativistic Breit-Wigner function (Perkins).
    // This should be the same as TMath::BreitWigner() 
    // except for the normalization.
    
    Double_t a = x[0] - par[1];
    Double_t b = par[2]*par[2] / 4.;
    
    return par[0] * b / (a*a + b);
}

//______________________________________________________________________________
Double_t TOF1BW::Evaluate(Double_t* x, Double_t* par)
{
    // Breit-Wigner function implementation.
    //
    // Parameters:
    // par[0] : Constant
    // par[1] : Mean
    // par[2] : Gamma
    
    // check if the gaussian convolution should be applied
    if (fIsGConv)
    {
        Double_t out = 0.;

        // calculate number of steps
        Double_t xMin = x[0] - 3. * fGConvSigma;
        Double_t xMax = x[0] + 3. * fGConvSigma;

        // step size
        Int_t nstep = Int_t(xMax - xMin) * fGConvNstep;
        Double_t delta = (xMax - xMin) / (Double_t)nstep;

        // numerical convolution
        for (Int_t i = 0; i < nstep; i++)
        {
            Double_t xx = xMin + i*delta;

            // Breit-Wigner value at xx
            Double_t bw;
            if (fIsRel) bw = EvalRel(&xx, par);
            else bw = EvalNonRel(&xx, par);

            // Gaussian with mean xx 
            out += bw * TMath::Gaus(x[0], xx, fGConvSigma, kTRUE);
        }

        return out / fGConvNstep;
    }
    else
    {
        // check if the relativistic version should be used
        if (fIsRel) return EvalRel(x, par);
        else return EvalNonRel(x, par);
    }
}

//______________________________________________________________________________
void TOF1BW::Express(Char_t* outString)
{
    // Write the function expression to 'outString'.
    
    // write basic expression
    sprintf(outString, "Breit-Wigner(");
    
    // rel./non-rel.
    if (fIsRel) strcat(outString, "rel");
    else strcat(outString, "non-rel");

    // with/without convolution
    if (fIsGConv) strcat(outString, ",gauss-conv");

    // finish expression
    strcat(outString, ")");
}

