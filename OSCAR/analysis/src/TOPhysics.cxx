// SVN Info: $Id: TOPhysics.cxx 1633 2013-01-24 14:00:17Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Manuel Dieterle, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOPhysics                                                            //
//                                                                      //
// This namespace contains some often used physics calculations.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOPhysics.h"


//______________________________________________________________________________
void TOPhysics::Calculate4Vector(Double_t x, Double_t y, Double_t z, Double_t energy, Double_t mass,
                                 TLorentzVector* outP4)
{
    // Calculate the 4-vector of a particle using 'mass' as the particle's 
    // mass, the detected energy 'energy' and the position 'x,y,z'. The components of
    // the 4-vector will be stored in 'p4'.
    
    Double_t tot_e = energy + mass;
    outP4->SetE(tot_e);
    TVector3 p(x, y, z);
    outP4->SetVect(p.Unit() * TMath::Sqrt(tot_e*tot_e - mass*mass));
}

//______________________________________________________________________________
Double_t TOPhysics::CalcQFThreeBodyRecoilPartT(Double_t beamE, TLorentzVector* p4Meson, TLorentzVector* p4Part, 
                                               Double_t mTarg, Double_t mPart, Double_t mSpec)
{
    // Calculate the kinetic energy of the recoil participant with mass 'mPart' 
    // in meson photoproduction off a target with mass 'mTarg' with beam energy 
    // 'beamE' in a quasi-free three-body-decay:
    //
    // gamma + Target(Participant + Spectator) -> Participant + Spectator + Meson 
    //
    // Use the reconstructed theta and phi angles in the 4-vector of the 
    // participant 'p4Part' and the fully reconstructed 4-vector 
    // of the meson 'p4Meson' to calculate the kinetic energy of the recoil 
    // participant. The spectator mass is 'mSpec'.

    // set input kinematics variables
    Double_t mesonPx = p4Meson->Px();
    Double_t mesonPy = p4Meson->Py();
    Double_t mesonPz = p4Meson->Pz();
    Double_t mesonE  = p4Meson->E();
    
    Double_t partTheta = p4Part->Theta();
    Double_t partPhi   = p4Part->Phi();

    // calculate terms
    Double_t a = mesonE - beamE - mTarg;
    Double_t b = (mesonE + mPart - beamE - mTarg) * (mesonE + mPart - beamE - mTarg) 
                 - mSpec * mSpec 
                 - mesonPx * mesonPx 
                 - mesonPy * mesonPy 
                 - mesonPz * mesonPz
                 - beamE * beamE
                 + 2 * beamE * mesonPz;
    Double_t c =   mesonPx * TMath::Sin(partTheta) * TMath::Cos(partPhi)
                 + mesonPy * TMath::Sin(partTheta) * TMath::Sin(partPhi) 
                 + (mesonPz - beamE ) * TMath::Cos(partTheta);
 
    Double_t quadEquA = a*a - c*c;
    Double_t quadEquB = a*b - 2*c*c*mPart;
    Double_t quadEquC = b*b;
  
    return (-quadEquB + TMath::Sqrt(quadEquB*quadEquB - quadEquA*quadEquC)) / (2*quadEquA);
}

//______________________________________________________________________________
Double_t TOPhysics::CalcPhotoprodPSFactorEg(Double_t eG, Double_t tMass, Double_t mMass)
{
    // Calculate the phase-space factor q_CM/k_CM at the photon beam energy 'eG' for
    // photoproduction of a meson with mass 'mMass' off a target with mass 'tMass'.
    
    /*
    //
    // brute force calculation
    //

    // calculate the meson momentum in the center-of-mass frame
    Double_t a = -8*eG*tMass-4*tMass*tMass;
    Double_t b = 4*eG*eG*tMass*tMass+mMass*mMass*mMass*mMass - 
                 4*eG*tMass*mMass*mMass - 
                 4*mMass*mMass*tMass*tMass;
    Double_t q = TMath::Sqrt(-b/a);

    // boost the photon beam momentum to the center-of-mass frame
    Double_t beta = eG / (eG + tMass);
    Double_t gamma = 1. / TMath::Sqrt(1. - beta*beta);
    Double_t k = gamma*eG*(1. - beta);
    */
    
    // 
    // elegant calculation
    //

    Double_t s = 2.*eG*tMass + tMass*tMass;
    Double_t k = eG * tMass / TMath::Sqrt(s);
    Double_t em = (s - tMass*tMass + mMass*mMass) / 2. / TMath::Sqrt(s);
    Double_t q = TMath::Sqrt(em*em - mMass*mMass);

    // check NaN
    if (TMath::IsNaN(q/k))
        Warning("TOPhysics::CalcPhotoprodPSFactorEg", "Phase-space factor is NaN! Below threshold?");

    return q / k;
}

//______________________________________________________________________________
Double_t TOPhysics::CalcPhotoprodPSFactorW(Double_t W, Double_t tMass, Double_t mMass)
{
    // Calculate the phase-space factor q_CM/k_CM at the center-of-mass energy 'W' for
    // photoproduction of a meson with mass 'mMass' off a target with mass 'tMass'.
    
    Double_t eg = (W*W - tMass*tMass) / 2. / tMass;
    return TOPhysics::CalcPhotoprodPSFactorEg(eg, tMass, mMass);
}

//______________________________________________________________________________
Double_t TOPhysics::CalcWSpecPart(Double_t eG, Double_t fermiP, Double_t fermiPz, 
                                  Double_t specMass, Double_t nuclMass,
                                  Double_t deG, Double_t* outdW)
{
    // Calculate the center-of-mass energy W for quasi-free photoproduction
    // in the participant-spectator-model.
    //
    // Arguments:
    // 'eg'       : photon beam energy
    // 'fermiP'   : Fermi momentum of spectator
    // 'fermiPz'  : z-component of Fermi momentum of spectator
    // 'specMass' : spectator mass
    // 'nuclMass: : nucleus mass
    // 'deG'      : photon beam energy error for calculation of dW
    // 'outdW'    : output pointer for error dW 

    Double_t es = TMath::Sqrt(specMass*specMass + fermiP*fermiP);
    Double_t w = TMath::Sqrt(nuclMass*nuclMass +
                             specMass*specMass +
                             2.*eG*(nuclMass - es) -
                             2.*nuclMass*es -
                             2.*eG*fermiPz );

    if (outdW) *outdW = (nuclMass - es - fermiPz) / w * deG;

    return w;
}

