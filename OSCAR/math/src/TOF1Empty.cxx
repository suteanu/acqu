// SVN Info: $Id: TOF1Empty.cxx 1202 2012-06-06 11:58:07Z werthm $

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


#include "TOF1Empty.h"

ClassImp(TOF1Empty)


//______________________________________________________________________________
TOF1Empty::TOF1Empty(const Char_t* name, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, 0)
{
    // Constructor.
    
}

//______________________________________________________________________________
Double_t TOF1Empty::Evaluate(Double_t* x, Double_t* par)
{
    // Function implementation.

    return 0;
}

