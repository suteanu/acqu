// SVN Info: $Id: TOF1GaussTail.h 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1GaussTail                                                        //
//                                                                      //
// TOF1 based Gaussian function with low value tail.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1GaussTail
#define OSCAR_TOF1GaussTail

#include "TMath.h"

#include "TOF1.h"


class TOF1GaussTail : public TOF1
{

public:
    TOF1GaussTail() : TOF1() { }
    TOF1GaussTail(const Char_t* name, Double_t xmin, Double_t xmax);
    virtual ~TOF1GaussTail() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString) { strcpy(outString, "gaus_tail"); }

    ClassDef(TOF1GaussTail, 1)  // Gaussian function with low value tail
};

#endif

