// SVN Info: $Id: TOF1Pol.h 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1Pol                                                              //
//                                                                      //
// TOF1 based function of polynomials.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1Pol
#define OSCAR_TOF1Pol

#include "TOF1.h"


class TOF1Pol : public TOF1
{

private:
    Int_t fNOrder;                          // order of the polynomial

public:
    TOF1Pol() : TOF1(), fNOrder(0) { }
    TOF1Pol(const Char_t* name, Double_t xmin, Double_t xmax, Int_t order);
    virtual ~TOF1Pol() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString);

    ClassDef(TOF1Pol, 1)  // Polynomial function
};

#endif

