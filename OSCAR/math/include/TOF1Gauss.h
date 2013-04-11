// SVN Info: $Id: TOF1Gauss.h 1202 2012-06-06 11:58:07Z werthm $

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


#ifndef OSCAR_TOF1Gauss
#define OSCAR_TOF1Gauss

#include "TMath.h"

#include "TOF1.h"


class TOF1Gauss : public TOF1
{

public:
    TOF1Gauss() : TOF1() { }
    TOF1Gauss(const Char_t* name, Double_t xmin, Double_t xmax);
    virtual ~TOF1Gauss() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString) { strcpy(outString, "gaus"); }

    ClassDef(TOF1Gauss, 1)  // Gaussian function
};

#endif

