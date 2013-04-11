// SVN Info: $Id: TOF1BW.h 1202 2012-06-06 11:58:07Z werthm $

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


#ifndef OSCAR_TOF1BW
#define OSCAR_TOF1BW

#include "TMath.h"

#include "TOF1.h"


class TOF1BW : public TOF1
{

protected:
    Bool_t fIsRel;                      // flag for relativistic version
    Bool_t fIsGConv;                    // flag for the Gaussian convolution
    Int_t fGConvNstep;                  // number of steps in the numerical convolution
    Double_t fGConvSigma;               // sigma of Gaussian convolution function

    Double_t EvalRel(Double_t* x, Double_t* par);
    Double_t EvalNonRel(Double_t* x, Double_t* par);

public:
    TOF1BW() : TOF1(), fIsRel(kTRUE), fIsGConv(kFALSE),
               fGConvNstep(1000), fGConvSigma(1) { }
    TOF1BW(const Char_t* name, Double_t xmin, Double_t xmax, 
           Bool_t isRel = kTRUE, Bool_t isGConv = kFALSE);
    virtual ~TOF1BW() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString);

    void SetGConvNstep(Int_t nstep) { fGConvNstep = nstep; }
    void SetGConvSigma(Double_t sigma) { fGConvSigma = sigma; }

    ClassDef(TOF1BW, 1)  // Breit-Wigner function
};

#endif

