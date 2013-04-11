// SVN Info: $Id: TOFermiDistribution.cxx 1202 2012-06-06 11:58:07Z werthm $

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


#include "TOFermiDistribution.h"


// Init namespace members
Bool_t TOFermiDistribution::fIsDeuteronInit = kFALSE;
Double_t TOFermiDistribution::fDeutC[13];
Double_t TOFermiDistribution::fDeutD[13];
Double_t TOFermiDistribution::fDeutM2[13];
TH1* TOFermiDistribution::fDeuteronDistr = 0;
TH1* TOFermiDistribution::fHe3Distr = 0;
TH1* TOFermiDistribution::fHe4Distr = 0;
TH1* TOFermiDistribution::fLi7Distr = 0;
TH1* TOFermiDistribution::fC12Distr = 0;
TH1* TOFermiDistribution::fCa40Distr = 0;


//______________________________________________________________________________
void TOFermiDistribution::InitDeuteron()
{
    // Initialize the parameter arrays for the deuteron Fermi distribution
    // parametrization.

    Info("TOFermiDistribution::InitDeuteron", "Initializing deuteron Fermi distribution parameters...");

    const Double_t alpha = 0.23162461;            // unit: 1/fm
    const Double_t mZero = 1.;                    // unit: 1/fm
    
    Double_t cj[13] = { 0.88688076e0, -0.34717093e0, -0.30502380e1,
                        0.56207766e2, -0.74957334e3,  0.53365279e4,
                       -0.22706863e5,  0.60434469e5, -0.10292058e6,
                        0.11223357e6, -0.75925226e5,  0.29059715e5,
                        0.0 }; // unit: 1/sqrt(fm)
    
    Double_t dj[13] = { 0.23135193e-1, -0.85604572e0,  0.56068193e1,
                       -0.69462922e2,   0.41631118e3, -0.12546621e4,
                        0.12387830e4,   0.33739172e4, -0.13041151e5,
                        0.19512524e5,   0.0,           0.0,
                        0.0}; // unit: 1/sqrt(fm)
    
    Double_t sum1 = 0.;
    Double_t sum2 = 0.;
    Double_t sum3 = 0.;
    Double_t rtmp;
    Double_t mj[13];
    Double_t mj2[13];
    Int_t n, n1, n2, temp;

    for (Int_t i = 0; i < 13; i++)
    {
        mj[i]  = alpha + i*mZero;
        mj2[i] = mj[i]*mj[i];
    } 
   
    cj[12] = 0.;
    
    for (Int_t i = 0; i < 12; i++) cj[12] = cj[12] - cj[i];
   
    for (Int_t k = 0; k < 5; k++)
    {
        rtmp = dj[k*2]/mj2[k*2] + dj[k*2+1]/mj2[k*2+1];
        sum1 = sum1 + rtmp;
        rtmp = dj[k*2] + dj[k*2+1];
        sum2 = sum2 + rtmp;
        rtmp = dj[k*2]*mj2[k*2] + dj[k*2+1]*mj2[k*2+1];
        sum3 = sum3 + rtmp;
    }

    n  = 12;
    n1 = 11;
    n2 = 10;

    for (Int_t l = 0; l < 3; l++)
    {  
        dj[n2] = -mj2[n1]*mj2[n]*sum1 + (mj2[n1]+mj2[n])*sum2 - sum3;
        dj[n2] = dj[n2] * mj2[n2]/(mj2[n]-mj2[n2])/(mj2[n1]-mj2[n2]); 

        temp = n2;
        n2 = n1;
        n1 = n;
        n = temp; 
    }

    for (Int_t i = 0; i < 13; i++)
    {
        fDeutC[i]   = cj[i];
        fDeutD[i]   = dj[i];
        fDeutM2[i]  = mj2[i];
    }

    fIsDeuteronInit = kTRUE;
}

//______________________________________________________________________________
Double_t TOFermiDistribution::EvalDeuteron(Double_t mom)
{
    // Returns the probability of the Fermi momentum 'mom' [MeV] in the
    // deuteron nucleus.

    // Init deuteron parameters if necessary
    if (!fIsDeuteronInit) InitDeuteron();

    // conversion to MeV
    mom *= 0.001;

    const Double_t pi = TMath::Pi();
    const Double_t hbar = TOGlobals::kHbarGeVfm;
    
    Double_t par0 = 0.;
    Double_t par1 = 0.;
    
    for (Int_t i = 0; i < 13; i++)
    {
        par0 += fDeutC[i] / (mom*mom / (hbar*hbar) + fDeutM2[i]);
        par1 += fDeutD[i] / (mom*mom / (hbar*hbar) + fDeutM2[i]);
    }
    
    par0 = TMath::Sqrt(2./pi) * par0;
    par1 = TMath::Sqrt(2./pi) * par1;
    
    return (par0*par0 + par1*par1) * mom*mom / (hbar*hbar);
}

//______________________________________________________________________________
Double_t TOFermiDistribution::EvalHe3(Double_t mom)
{
    // Returns the probability of the Fermi momentum 'mom' [MeV] in the
    // He-3 nucleus.

    const Double_t pi = TMath::Pi();
    const Double_t a  = 7.09078;
    const Double_t b  = 5.38753;
    const Double_t c  = 9.90202;
    const Double_t d  = 0.779408;

    Double_t par0 = (4./TMath::Sqrt(pi)) * TMath::Power(a, 3./2.);
    Double_t par1 = mom * mom * 25. / 1.e6;
    return d * par0 * par1 * (TMath::Exp(-par1*a) + c*TMath::Exp(-TMath::Sqrt(par1)*b));
}

//______________________________________________________________________________
Double_t TOFermiDistribution::EvalHe4(Double_t mom)
{
    // Returns the probability of the Fermi momentum 'mom' [MeV] in the
    // He-4 nucleus.

    // conversion to MeV
    mom *= 0.001;

    const Double_t hbar = TOGlobals::kHbarGeVfm;
    const Double_t a    = 0.7352;
    const Double_t b    = 0.05511;

    Double_t par0 = TMath::Exp(-mom*mom / (hbar*hbar*a));
    
    return mom * mom / (hbar*hbar*b) *par0;
}

//______________________________________________________________________________
Double_t TOFermiDistribution::EvalLi7(Double_t mom)
{
    // Returns the probability of the Fermi momentum 'mom' [MeV] in the
    // Li-7 nucleus.

    const Double_t a = 1.2e-4;
    const Double_t b = 6.87e-3;
    const Double_t c = 110.;
   
    Double_t par0 = mom * mom * a;
    Double_t par1 = (1. + b*mom);
    Double_t par2 = mom / c;
    return par0 * par1 * TMath::Exp(-par2*par2);
}

//______________________________________________________________________________
Double_t TOFermiDistribution::EvalC12(Double_t mom)
{
    // Returns the probability of the Fermi momentum 'mom' [MeV] in the
    // C-12 nucleus.

    const Double_t pi = TMath::Pi();
    const Double_t a  = 1. / 0.416;
    const Double_t b  = 1. / 0.23;
    const Double_t c  = 0.04;
   
    Double_t par0 = (4. / TMath::Sqrt(pi)) * TMath::Power(a, 3./2.);
    Double_t par1 = mom * mom * 25. / 1.e6;
    return par0 * par1 * (TMath::Exp(-par1*a) + c*TMath::Exp(-TMath::Sqrt(par1)*b));
}

//______________________________________________________________________________
Double_t TOFermiDistribution::EvalCa40(Double_t mom)
{
    // Returns the probability of the Fermi momentum 'mom' [MeV] in the
    // Ca-40 or heavier nuclei.
    
    const Double_t pi = TMath::Pi();
    const Double_t a  = 1 / 0.42;
    const Double_t b  = 1 / 0.23;
    const Double_t c  = 0.04;
   
    Double_t par0 = (4. / TMath::Sqrt(pi)) * TMath::Power(a, 3./2.);
    Double_t par1 = mom * mom * 25. / 1.e6;
    return par0 * par1 * (TMath::Exp(-par1*a) + c*TMath::Exp(-TMath::Sqrt(par1)*b));
}

//______________________________________________________________________________
TH1* TOFermiDistribution::GetDistribution(ONucleus_t nucleus)
{
    // Returns a histogram with the Fermi momentum distribution for the
    // nucleus 'nucleus'.
    
    // select distribution
    switch (nucleus)
    {
        case kDeuteron:
        {
            // Load histogram from file if necessary
            if (!fDeuteronDistr)
            {
                TOLoader::LoadObject("$OSCAR/data/fermi_deuteron.root",
                                     "dis", &fDeuteronDistr, "deuteron Fermi distribution");
                if (!fDeuteronDistr) 
                    Error("TOFermiDistribution::GetDistribution", "Could not open the deuteron Fermi momentum distribution ROOT file!\n"
                                                                  "Check your OSCAR installation and the $OSCAR environment variable!");
            }

            return fDeuteronDistr;
        }
        case kHe3:
        {
            // Load histogram from file if necessary
            if (!fHe3Distr)
            {
                TOLoader::LoadObject("$OSCAR/data/fermi_he3.root",
                                     "dis", &fHe3Distr, "He-3 Fermi distribution");
                if (!fHe3Distr) 
                    Error("TOFermiDistribution::GetDistribution", "Could not open the He-3 Fermi momentum distribution ROOT file!\n"
                                                                  "Check your OSCAR installation and the $OSCAR environment variable!");
            }

            return fHe3Distr;
        }
        case kHe4:
        {
            // Load histogram from file if necessary
            if (!fHe4Distr)
            {
                TOLoader::LoadObject("$OSCAR/data/fermi_he4.root",
                                     "dis", &fHe4Distr, "He-4 Fermi distribution");
                if (!fHe4Distr) 
                    Error("TOFermiDistribution::GetDistribution", "Could not open the He-4 Fermi momentum distribution ROOT file!\n"
                                                                  "Check your OSCAR installation and the $OSCAR environment variable!");
            }

            return fHe4Distr;
        }
        case kLi7:
        {
            // Load histogram from file if necessary
            if (!fLi7Distr)
            {
                TOLoader::LoadObject("$OSCAR/data/fermi_li7.root",
                                     "dis", &fLi7Distr, "Li-7 Fermi distribution");
                if (!fLi7Distr) 
                    Error("TOFermiDistribution::GetDistribution", "Could not open the Li-7 Fermi momentum distribution ROOT file!\n"
                                                                  "Check your OSCAR installation and the $OSCAR environment variable!");
            }

            return fLi7Distr;
        }
        case kC12:
        {
            // Load histogram from file if necessary
            if (!fC12Distr)
            {
                TOLoader::LoadObject("$OSCAR/data/fermi_c12.root",
                                     "dis", &fC12Distr, "C-12 Fermi distribution");
                if (!fC12Distr) 
                    Error("TOFermiDistribution::GetDistribution", "Could not open the C-12 Fermi momentum distribution ROOT file!\n"
                                                                  "Check your OSCAR installation and the $OSCAR environment variable!");
            }

            return fC12Distr;
        }
        case kCa40:
        {
            // Load histogram from file if necessary
            if (!fCa40Distr)
            {
                TOLoader::LoadObject("$OSCAR/data/fermi_ca40.root",
                                     "dis", &fCa40Distr, "Ca-40 Fermi distribution");
                if (!fCa40Distr) 
                    Error("TOFermiDistribution::GetDistribution", "Could not open the Ca-40 Fermi momentum distribution ROOT file!\n"
                                                                  "Check your OSCAR installation and the $OSCAR environment variable!");
            }

            return fCa40Distr;
        }
        case kNoNucleus:
        {
            Error("TOFermiDistribution::GetDistribution", "Please choose a nucleus supported by OSCAR!");
            return 0;
        }
        default:
        {
            Error("TOFermiDistribution::GetDistribution", "Please choose a nucleus supported by OSCAR!");
            return 0;
        }
    }
}

