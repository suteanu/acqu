// SVN Info: $Id: TOF1BW_S11_1535.cxx 1394 2012-10-18 09:06:59Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Bernd Krusche, 2012
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1BW_S11_1535                                                      //
//                                                                      //
// TOF1 based S11(1535)-Breit-Wigner function with energy dependent     //
// width.                                                               //
// Adapted from PAW implementation by B. Krusche.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1BW_S11_1535.h"

ClassImp(TOF1BW_S11_1535)


//______________________________________________________________________________
TOF1BW_S11_1535::TOF1BW_S11_1535(const Char_t* name, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, 6)
{
    // Constructor.
    
    // init members
    fNucleonMass = TOGlobals::kProtonMass;
    fIsReduced = kFALSE;

    // set parameter names
    SetParNames("W_R", "Gamma_R", "A_1/2",
                "b_eta", "b_pi", "b_2pi");
}
 
//______________________________________________________________________________
Double_t TOF1BW_S11_1535::Evaluate(Double_t* x, Double_t* par)
{
    // S11(1535)-Breit-Wigner function with energy dependent width implementation.
    //
    // Should not be evaluated below approx. 1485 MeV
    //
    // sigma(W) = [q*_eta(W)/q*_eta(W_R)]x[k*(W_R)/k(W)] x
    //
    //            (A_1/2)**2 x 0.3892729 x W_R x b_eta x Gamma_R x 2 x mN /
    //
    //            [(W_R**2-W**2)**2 + W_R**2 x Gamma(W)**2]
    //
    // Gamma(W) = Gamma_R[b_eta x (q*_eta(W)/q*_eta(W_R)) +
    //
    //            b_pi0 x (q*_pi0(W)/q*i0(W_R)) + b_pipi]
    //
    //
    // W:       invariant mass (sqrt(s))
    // W_R:     resonance position
    // q*_eta:  eta cm momentum
    // q*_pi0:  pion cm momentum
    // K*:      photon cm momentum
    //
    //
    // Parameters:
    // par[0] : Resonance position W_R [MeV]
    // par[1] : Resonance width [MeV]
    // par[2] : Helicity amplitude [10^-3 GeV^-1/2]
    // par[3] : b_eta
    // par[4] : b_pi (non-reduced version)
    // par[5] : b_2pi (non-reduced version)
    //
    // standard values (B. Krusche) : 1550, 250, 120, 0.5, 0.4, 0.1
    // standard values (PDG 2012)   : 1535, 150, 90, 0.42, 0.45, 0.05
    //
    // In the reduced version the width is kept fixed.
    //

    Double_t mN = fNucleonMass;
    Double_t me = TOGlobals::kEtaMass;
    Double_t mpi = TOGlobals::kPi0Mass;

    // Mandelstam s=W^2 and s_r=W_R^2
    Double_t s = x[0]*x[0];
    Double_t sr = par[0]*par[0];

    // cm momentum of gamma at W and W_R
    Double_t kgam = (s - mN*mN) / (2.*x[0]);
    Double_t kgamr = (sr - mN*mN) / (2.*par[0]);

    // cm momentum of eta at W and W_R
    Double_t keta = (s - mN*mN + me*me) / (2.*x[0]);
    keta = TMath::Sqrt(keta*keta - me*me);
    Double_t ketar = (sr - mN*mN + me*me) / (2.*par[0]);
    ketar = TMath::Sqrt(ketar*ketar - me*me);
    
    // cm momentum of pion at W and W_R
    Double_t kpio = (s - mN*mN + mpi*mpi) / (2.*x[0]);
    kpio = TMath::Sqrt(kpio*kpio - mpi*mpi);
    Double_t kpior = (sr - mN*mN + mpi*mpi) / (2.*par[0]);
    kpior = TMath::Sqrt(kpior*kpior - mpi*mpi);
    
    // energy dependent Breit-Wigner width
    Double_t gamsp;
    if (fIsReduced) gamsp = par[1];
    else gamsp = par[1] * (par[3]*keta/ketar + par[4]*kpio/kpior + par[5]);

    // kinematical factor
    Double_t kfac = (keta * kgamr) / (kgam * ketar);

    // nominator
    Double_t nom = par[2]*par[2]*0.3892729*par[0]*par[1]*par[3]*2.*mN;

    // denominator
    Double_t den = gamsp*gamsp*par[0]*par[0] + (par[0]*par[0] - s)*(par[0]*par[0] - s);

    // final result
    return kfac * (nom / den);
}

