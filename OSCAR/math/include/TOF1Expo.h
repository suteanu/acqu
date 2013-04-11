// SVN Info: $Id: TOF1Expo.h 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1Expo                                                             //
//                                                                      //
// TOF1 based function of an exponential.                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1Expo
#define OSCAR_TOF1Expo

#include "TMath.h"

#include "TOF1.h"


class TOF1Expo : public TOF1
{

public:
    TOF1Expo() : TOF1() { }
    TOF1Expo(const Char_t* name, Double_t xmin, Double_t xmax);
    virtual ~TOF1Expo() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString) { strcpy(outString, "expo"); }
    
    ClassDef(TOF1Expo, 1)  // Exponential function
};

#endif

