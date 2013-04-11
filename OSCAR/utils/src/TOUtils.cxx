// SVN Info: $Id: TOUtils.cxx 1630 2013-01-24 13:54:58Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOUtils                                                              //
//                                                                      //
// This namespace contains some often used utility functions.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOUtils.h"


//______________________________________________________________________________
Int_t TOUtils::GetPositionInArray(Int_t e, Int_t n, Int_t* a)
{
    // Returns the position of the first occurence of the number e
    // in the array a of length n.
    // If e is not found -1 is returned.

    // loop over array
    for (Int_t i = 0; i < n; i++)
    {
        if (a[i] == e) return i;
    }
    
    return -1;
}

//______________________________________________________________________________
void TOUtils::CalculateMeanErrorWeights(Int_t n, Double_t* v, Double_t* e, 
                                        Double_t* outMean, Double_t* outMeanErr)
{
    // Calculate the error-weighted mean and the mean error for 'n' points
    // using the value and error arrays 'v' and 'e', respectively.
    // The calculate mean and error are saved to 'outMean' and 'outMeanErr'.

    Double_t m = 0;
    Double_t me = 0;

    // loop over values
    for (Int_t i = 0; i < n; i++)
    {
        m += v[i] / e[i] / e[i];
        me += 1. / e[i] / e[i];
    }
    
    // complete the calculation (order of statements is important)
    m /= me;
    me = 1. / TMath::Sqrt(me);

    // set values
    *outMean = m;
    *outMeanErr = me;
}

//______________________________________________________________________________
Int_t TOUtils::Round(Double_t x)
{
    // Correct rounding of 'x' to integer.

    return x > 0 ? TMath::Floor(x+0.5) : -TMath::Floor(-x+0.5);
}

//______________________________________________________________________________
Int_t TOUtils::GetNumberOfCPUs()
{
    // Return the number of CPU cores on a system.
    // Supported on Linux, Mac OS X and Windows.
    // Source: http://www.cprogramming.com/snippets/source-code/find-the-number-of-cpu-cores-for-windows-mac-or-linux

    #ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
    #elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
    #else
    return sysconf(_SC_NPROCESSORS_ONLN);
    #endif
}

//______________________________________________________________________________
void TOUtils::PlotPreliminary(Double_t textSize)
{
    // Plot a "preliminary" watermark on the current canvas.

    TText* t = new TText(); 
    t->SetNDC(kTRUE);
    t->SetTextColor(17); 
    t->SetTextSize(textSize); 
    t->SetTextAlign(22);
    t->SetTextAngle(35); 
    t->DrawText(0.5, 0.5, "preliminary");
}

