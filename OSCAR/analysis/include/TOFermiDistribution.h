// SVN Info: $Id: TOFermiDistribution.h 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Lilian Witthauer, Manuel Dieterle, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOFermiDistribution                                                  //
//                                                                      //
// This namespace contains parametrizations of Fermi momentum distribu- //
// tions for various nuclei.                                            //
//                                                                      //
// References:                                                          //
// - Deuteron                                                           //
//   M. Lacombe et al., Phys. Lett. B 101, 3, 139-140                   //
//                                                                      //
// - He-3, He-4                                                         //
//   McCarthy et al., Phys. Rev. C 15, 4, 1396-1414                     //
//                                                                      //
// - Li-7                                                               //
//   K. Nakamura et al., Nucl. Phys. A 296, 3, 431-443                  //
//                                                                      //
// - C-12                                                               //
//   K. Nakamura et al., Nucl. Phys. A 268, 3, 381-407                  //
//                                                                      //
// - Ca-40                                                              //
//   Taken from 'Photoproduktion von eta-Mesonen an komplexen Kernen',  //
//   M. Roebig-Landau, Dissertation                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOFermiDistribution
#define OSCAR_TOFermiDistribution

#include "TMath.h"

#include "TOGlobals.h"
#include "TOLoader.h"


enum EONucleus {
    kNoNucleus,
    kDeuteron,
    kHe3,
    kHe4,
    kLi7,
    kC12,
    kCa40,
};
typedef EONucleus ONucleus_t;


namespace TOFermiDistribution
{
    // parameter arrays
    extern Bool_t fIsDeuteronInit;
    extern Double_t fDeutC[13];
    extern Double_t fDeutD[13];
    extern Double_t fDeutM2[13];
    
    // distribution histograms
    extern TH1* fDeuteronDistr;
    extern TH1* fHe3Distr;
    extern TH1* fHe4Distr;
    extern TH1* fLi7Distr;
    extern TH1* fC12Distr;
    extern TH1* fCa40Distr;
    
    // methods
    void InitDeuteron();
    Double_t EvalDeuteron(Double_t mom);
    Double_t EvalHe3(Double_t mom);
    Double_t EvalHe4(Double_t mom);
    Double_t EvalLi7(Double_t mom);
    Double_t EvalC12(Double_t mom);
    Double_t EvalCa40(Double_t mom);
    
    TH1* GetDistribution(ONucleus_t nucleus);
};

#endif

