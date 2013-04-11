// SVN Info: $Id: TOF1Empty.h 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2012
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1Empty                                                            //
//                                                                      //
// Empty pseudo-function based on TOF1.                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1Empty
#define OSCAR_TOF1Empty

#include "TOF1.h"


class TOF1Empty : public TOF1
{

public:
    TOF1Empty() : TOF1() { }
    TOF1Empty(const Char_t* name, Double_t xmin, Double_t xmax);
    virtual ~TOF1Empty() { }
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString) { strcpy(outString, "empty_func"); }
    
    ClassDef(TOF1Empty, 1)  // Empty pseudo-function
};

#endif

