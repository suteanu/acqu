// SVN Info: $Id: TOF1LegPol.h 1202 2012-06-06 11:58:07Z werthm $

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


#ifndef OSCAR_TOF1LegPol
#define OSCAR_TOF1LegPol

#include "TOF1.h"


class TOF1LegPol : public TOF1
{

private:
    Int_t fNOrder;                          // order of the polynomial
    Double_t fScaling;                      // scaling factor

public:
    TOF1LegPol() : TOF1(), fNOrder(0), fScaling(1.) { }
    TOF1LegPol(const Char_t* name, Double_t xmin, Double_t xmax, Int_t order);
    virtual ~TOF1LegPol() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString);
    
    Double_t GetScaling() const { return fScaling; }

    void SetScaling(Double_t s) { fScaling = s; }

    ClassDef(TOF1LegPol, 1)  // Legendre polynomial function
};

#endif

