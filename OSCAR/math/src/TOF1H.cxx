// SVN Info: $Id: TOF1H.cxx 1202 2012-06-06 11:58:07Z werthm $

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


#include "TOF1H.h"

ClassImp(TOF1H)


//______________________________________________________________________________
TOF1H::TOF1H(const Char_t* name, Int_t nhist, TH1** hists, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, nhist)
{
    // Constructor.
    
    Char_t tmp[256];

    // check number of histograms
    if (nhist < 1)
    {
        Error("TOF1H", "Number of histograms has to be greater than 0! (was %d)", nhist);
        return;
    }
    
    // init members
    fNHist = nhist;
    fHist = new TH1*[fNHist];
    
    // set parameter names
    for (Int_t i = 0; i < fNHist; i++)
    {
        sprintf(tmp, "Constant_%d", i);
        SetParName(i, tmp);
    }

    // copy the histograms to keep them in this class
    // (used for serialization)
    for (Int_t i = 0; i < fNHist; i++)
    {
        // check if histogram exists
        if (hists[i])
        {
            // clone the histogram
            fHist[i] = (TH1*) hists[i]->Clone();
            
            // prevent ROOT from deleting the histogram after closing the ROOT file
            fHist[i]->SetDirectory(0);
        }
        else
        {
            Error("TOF1H", "Could not load all %d histograms!", fNHist);
            
            // cleanup and leave
            delete [] fHist;
            fNHist = 0;
            fHist = 0;
            return;
        }
    }
}

//______________________________________________________________________________
TOF1H::~TOF1H()
{
    // Destructor.
    
    if (fHist)
    {
        for (Int_t i = 0; i < fNHist; i++) delete fHist[i];
        delete [] fHist;
    }
}

//______________________________________________________________________________
Double_t TOF1H::Evaluate(Double_t* x, Double_t* par)
{
    // Return the parameter-weighted sum of the histograms.

    Double_t out = 0.;
 
    // add histogram contributions
    for (Int_t i = 0; i < fNHist; i++) 
        out += par[i] * fHist[i]->Interpolate(x[0]);
    
    return out;
}

//______________________________________________________________________________
void TOF1H::Express(Char_t* outString)
{
    // Write the function expression to 'outString'.
    
    Char_t tmp[256];

    // clear output string
    outString[0] = '\0';

    // loop over all histos
    for (Int_t i = 0; i < fNHist; i++)
    {
        if (i > 0) strcat(outString, "+");
        sprintf(tmp, "TH1(%s)", fHist[i]->GetName());
        strcat(outString, tmp);
    }
}

//______________________________________________________________________________
TH1* TOF1H::GetHistogram(Int_t h) const
{
    // Return the 'h'-th histogram.

    // check histogram index
    if (h >= fNHist || h < 0)
    {
        Error("GetHistogram", "Cannot find histogram with index '%d'! (number of histograms: %d)", h, fNHist);
        return 0;
    }
    else
    {
        return fHist[h];
    }
}

