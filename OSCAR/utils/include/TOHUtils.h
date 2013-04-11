// SVN Info: $Id: TOHUtils.h 1627 2013-01-10 12:19:52Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2007-2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOHUtils                                                             //
//                                                                      //
// This namespace contains some often used functions and operations     //
// for histogram processing.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOHUtils
#define OSCAR_TOHUtils

#include "TError.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TRandom.h"

#include "TOUtils.h"


namespace TOHUtils
{
    TH1F* SubtractHistogram(TH1F* inH1, TH1F* inH2, Double_t inScale, Int_t inRb, const char* inName);
    TH2F* SubtractHistogram2D(TH2F* inH1, TH2F* inH2, Double_t inScale, Int_t inRb, const char* inName);
    TH1F* ExtractHistogram(TH1F* inH, Double_t inXmin, Double_t inXmax, const Char_t* inName = "");
    TH2F* ExtractHistogram(TH2F* inH, Double_t inXmin, Double_t inXmax, 
                                      Double_t inYmin, Double_t inYmax, const Char_t* inName = "");
    void InvertHistogram(TH1* inH);
    void InvertHistogram(THnSparse* inH);
    TH1* DeriveHistogram(TH1* inH);
    void ZeroBins(TH1* inH, Double_t th = 0);
    void ZeroBins(THnSparse* inH, Double_t th = 0);
    void ZeroBinErrors(TH1* inH);
    void ZeroBinErrors(THnSparse* inH);
    void CleanBins(TH1* inH);
    void Smooth2D(TH2* inH);

    Int_t GetFilledBins(TH1* h);
    void GetErrorDistribution(TH1* inH, TH1* outDistr);
    void GetErrorDistribution(THnSparse* inH, TH1* outDistr);
    Double_t GetRandomFast(TH1* h, Double_t* integral, TRandom* rand);

    Double_t GetBinContent(THnSparse* h, Double_t* x);
    Double_t FindContent(TH1* inH, Double_t inC, Double_t inXmin, Double_t inXmax,
                         Bool_t reverse = kFALSE);
    Double_t GetBinContentClosest2(TH1* inH, Double_t inX, Double_t inY);
    Double_t ExactIntegral(TH1* inH, Double_t inX1, Double_t inX2);
    Double_t ExactIntegralUnderFunction(TH1* inH, Double_t inX1, Double_t inX2, TF1* inF);

    TFitResultPtr FitHistogram(TH1* h, TF1* f, const Char_t* opt, Double_t fitMin = 0, Double_t fitMax = 0,
                               const Int_t maxIter = 20);
    Int_t FindOptimalBinning(TH1* h);

    void FillBinOverlap(TH1* h, Double_t x, Double_t x_width, 
                        Double_t y, Double_t z, Double_t weight = 1.);
    
    void SmoothGraph(TGraph* g, Int_t ntimes = 1);
    void RebinGraph(TGraph* g, Int_t rebin);
    void RebinGraph(TGraphErrors* g, Int_t rebin, Bool_t noXErrors = kFALSE);
    void ScaleGraph(TGraph* g, Double_t c);
    TGraph* MultiplyGraphs(TGraph* g1, TGraph* g2, Int_t np = 0);
    TGraph* DivideGraphs(TGraph* g1, TGraph* g2, Int_t np = 0);
    TGraph* AddGraphs(TGraph* g1, TGraph* g2, Double_t c = 1, Int_t np = 0);
    TGraph* MergeGraphs(TGraph* g1, TGraph* g2);
    Double_t GetGraphMaximum(TGraph* g, Double_t min = -1e30, Double_t max = 1e30);
    Double_t GetGraphMaximumX(TGraph* g, Double_t min = -1e30, Double_t max = 1e30);
    void AddFunctionToGraph(TGraph* g, TF1* f, Double_t c = 1.);
    void ConvertAxisWToEgamma(TGraph* g, Double_t targetMass);
    void ConvertAxisEgammaToW(TGraph* g, Double_t targetMass);
    void RemoveGraphErrorsX(TGraphErrors* g);
    void RemoveGraphErrorsY(TGraphErrors* g);
    TH1* ConvertGraphToHistogram(TGraphErrors* g);

    void PrintHistogram(TH1* h, const Char_t* format = "%e", const Char_t* filename = 0);
    void PrintGraph(TGraph* g, const Char_t* format = "%e", const Char_t* filename = 0);
    void PrintTCSToLaTeX(TGraphErrors* g, const Char_t* filename = 0, const Double_t* sysErr = 0,
                         Int_t nPrec = 4, const Char_t* energyLabel = "$E_{\\gamma}$");

    void FormatHistogram(TH1* h, const Char_t* xTitle = 0, const Char_t* yTitle = 0,
                         const Char_t* hTitle = 0);
}

#endif

