// SVN Info: $Id: TOF1BW_S11_1535.h 1394 2012-10-18 09:06:59Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Bernd Krusche, 2012
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1BW_S11_1535                                                      //
//                                                                      //
// TOF1 based S11(1535)-Breit-Wigner function with energy dependent     //
// width.                                                               //
// Adapted from PAW implementation by B. Krusche.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1BW_S11_1535
#define OSCAR_TOF1BW_S11_1535

#include "TOGlobals.h"
#include "TMath.h"

#include "TOF1.h"


class TOF1BW_S11_1535 : public TOF1
{

private:
    Double_t fNucleonMass;                  // nucleon mass used in function
    Bool_t fIsReduced;                      // flag for reduced version of function

public:
    TOF1BW_S11_1535() : TOF1(),
                        fNucleonMass(0), 
                        fIsReduced(kFALSE) { }
    TOF1BW_S11_1535(const Char_t* name, Double_t xmin, Double_t xmax);
    virtual ~TOF1BW_S11_1535() { }
    
    void SetNucleonMass(Double_t m) { fNucleonMass = m; }
    void SetReduced(Bool_t r = kTRUE) { fIsReduced = r; }

    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString) { strcpy(outString, "S11(1535)-Breit-Wigner"); }

    ClassDef(TOF1BW_S11_1535, 1)  // S11(1535)-Breit-Wigner function
};

#endif

