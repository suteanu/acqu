// SVN Info: $Id: TOF1H.h 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1H                                                                //
//                                                                      //
// TOF1 based function evaluating a sum of histograms.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1H
#define OSCAR_TOF1H

#include "TH1.h"

#include "TOF1.h"


class TOF1H : public TOF1
{

protected:
    Int_t fNHist;                               // number of histograms
    TH1** fHist;                                //[fNHist] array of histograms

public:
    TOF1H() : TOF1(), fNHist(0), fHist(0) { }
    TOF1H(const Char_t* name, Int_t nhist, TH1** hists, Double_t xmin, Double_t xmax);
    virtual ~TOF1H();
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString);
    
    TH1* GetHistogram(Int_t h) const; 

    ClassDef(TOF1H, 1)  // Function using a sum of histograms
};

#endif

