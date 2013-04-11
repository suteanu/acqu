// SVN Info: $Id: TOA2BaseDetParticle.cxx 1261 2012-07-28 20:48:41Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2BaseDetParticle                                                  //
//                                                                      //
// Base class for reconstructed particles in the A2 setup.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOA2BaseDetParticle.h"

ClassImp(TOA2BaseDetParticle)


//______________________________________________________________________________
TOA2BaseDetParticle::TOA2BaseDetParticle()
    : TObject()
{
    // Empty constructor.
    
    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();

    fPDG_ID   = kRootino;
    fDetector = kNoDetector;

    fX = 0;
    fY = 0;
    fZ = 0;

    fEnergy = 0;
    fTime   = 0;;

    fClusterSize = 0;

    fCentralElement = 0;
    fCentralEnergy  = 0;
    
    fPIDIndex  = -1;
    fPIDEnergy =  0;
    fPIDTime   =  0;

    fCentralSGEnergy = 0;
    
    fVetoIndex       = -1;
    fVetoEnergy      =  0;
    fVetoTime        =  0;
}

//______________________________________________________________________________
TOA2BaseDetParticle::TOA2BaseDetParticle(const TOA2BaseDetParticle& orig)
    : TObject(orig)
{
    // Copy constructor.
    
    fPDG_ID   = orig.fPDG_ID;
    fDetector = orig.fDetector;

    fX = orig.fX;
    fY = orig.fY;
    fZ = orig.fZ;

    fEnergy = orig.fEnergy;
    fTime   = orig.fTime;

    fClusterSize = orig.fClusterSize;

    fCentralElement = orig.fCentralElement;
    fCentralEnergy  = orig.fCentralEnergy;
    
    fPIDIndex  = orig.fPIDIndex;
    fPIDEnergy = orig.fPIDEnergy;
    fPIDTime   = orig.fPIDTime;

    fCentralSGEnergy = orig.fCentralSGEnergy;
    
    fVetoIndex       = orig.fVetoIndex;
    fVetoEnergy      = orig.fVetoEnergy;
    fVetoTime        = orig.fVetoTime;
}

//______________________________________________________________________________
void TOA2BaseDetParticle::Calculate4Vector(TLorentzVector* p4, Double_t mass)
{
    // Calculate the 4-vector of this particle using 'mass' as the particle's 
    // mass and the detected energy and position information. The components of
    // the 4-vector will be stored in 'p4'.
 
    TOPhysics::Calculate4Vector(fX, fY, fZ, fEnergy, mass, p4);
}

//______________________________________________________________________________
void TOA2BaseDetParticle::Calculate4Vector(TLorentzVector* p4, Double_t mass, Double_t energy)
{
    // Calculate the 4-vector of this particle using 'mass' as the particle's 
    // mass, 'energy' as the kinetic energy and the detected position information. 
    // The energy 'fEnergy' saved in this class will not be used in this method.
    // The components of the 4-vector will be stored in 'p4'.
    
    Double_t tot_e = energy + mass;
    p4->SetE(tot_e);
    TVector3 p(fX, fY, fZ);
    p4->SetVect(p.Unit() * TMath::Sqrt(tot_e*tot_e - mass*mass));
}

//______________________________________________________________________________
Bool_t TOA2BaseDetParticle::Calculate4VectorTOFTagger(TLorentzVector* p4, Double_t mass, Double_t taggerTime,
                                                  Bool_t isMC)
{
    // Calculate the 4-vector of this particle using 'mass' as the particle's 
    // mass, 'taggerTime' as the tagger time and the detected position information. 
    // The components of the 4-vector will be stored in 'p4'.
    //
    // Return kFALSE if the calculated solution is unphysical, otherwise kTRUE.

    // calculate the kinetic energy
    Double_t ekin = CalculateEkinTOFTagger(mass, taggerTime, isMC);
    
    // reject unphysical solutions
    if (ekin < 0.) return kFALSE;

    // calculate the 4-vector
    Double_t tot_e = ekin + mass;
        
    // set the 4-vector
    p4->SetE(tot_e);
    TVector3 p(fX, fY, fZ);
    p4->SetVect(p.Unit() * TMath::Sqrt(tot_e*tot_e - mass*mass));
    
    return kTRUE;
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::CalculateEkinTOFTagger(Double_t mass, Double_t taggerTime, Bool_t isMC)
{
    // Calculate the kinetic energy of this particle using 'mass' as the particle's 
    // mass, 'taggerTime' as the tagger time and the detected position information. 
    //
    // Return a negative value if the calculated solution is unphysical.

    // calculate the time of flight for 1 meter [ns/1m]
    Double_t t_norm = CalculateTOFTagger(taggerTime, isMC);
    
    // calculate the beta
    Double_t beta = 1. / (t_norm * 1e-9 * TMath::C());

    // calculate the kinetic energy
    if (beta*beta < 1)
    {
        Double_t gamma = TMath::Sqrt(1. / (1. - beta*beta));
        return  mass * (gamma - 1.);
    }
    else return -1.;
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::CalculateEkinTOFTAPS(Double_t mass, Double_t tapsTime, Bool_t isMC)
{
    // Calculate the kinetic energy of this particle using 'mass' as the particle's 
    // mass, 'tapsTime' as the TAPS time and the detected position information. 
    //
    // Return a negative value if the calculated solution is unphysical.

    // calculate the time of flight for 1 meter [ns/1m]
    Double_t t_norm = CalculateTOFTAPS(tapsTime, isMC);
    
    // calculate the beta
    Double_t beta = 1. / (t_norm * 1e-9 * TMath::C());

    // calculate the kinetic energy
    if (beta*beta < 1)
    {
        Double_t gamma = TMath::Sqrt(1. / (1. - beta*beta));
        return  mass * (gamma - 1.);
    }
    else return -1.;
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::CalculateTOFTagger(Double_t taggerTime, Bool_t isMC)
{
    // Return the time-of-flight normalized to 1 meter [ns/1m] relative
    // to the tagger time 'taggerTime'.
    //
    // If 'isMC' is kTRUE the MC timing is returned (different calculation for 
    // the TAPS TOF).
    
    // calculate the particle tof
    Double_t tof = 0;

    //
    // TAPS
    // t_TAPS = t_trig - t_CFD
    // t_tagg = t_scint - t_trig
    // t = -(t_TAPS + t_tagg) = -(t_scint - t_CFD) = t_CFD - t_scint
    if (fDetector == kTAPSDetector) 
    {
        tof = - (fTime + taggerTime);
    }
    //
    // CB
    // t_CB = t_NaI - t_trig
    // t = t_CB - t_tagg = t_NaI - t_scint
    else if (fDetector == kCBDetector) 
    {
        tof = fTime - taggerTime;
    }

    // calculate the time of flight for 1 meter [ns/1m]
    if (isMC)
        return (fTime - taggerTime) / (GetFlightPath() / 100.);
    else
        return tof / (GetFlightPath() / 100.) + 1e9 / TMath::C();
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::CalculateTOFCB(Double_t cbTime, Bool_t isMC)
{
    // Return the time-of-flight normalized to 1 meter [ns/1m] relative
    // to the CB time 'cbTime'.
    //
    // If 'isMC' is kTRUE the MC timing is returned (different calculation).
    
    // calculate the particle tof
    Double_t tof = 0;

    //
    // TAPS
    // t_TAPS = t_trig - t_CFD
    // t_CB = t_NaI - t_trig
    // t = -(t_TAPS + t_CB) = -(t_NaI - t_CFD) = t_CFD - t_NaI
    if (fDetector == kTAPSDetector) 
    {
        tof = - (fTime + cbTime);
    }
    //
    // CB
    // t_CB = t_NaI - t_trig
    // t = t_CB - t_CB_ref = t_NaI - t_NaI_ref
    else if (fDetector == kCBDetector) 
    {
        tof = fTime - cbTime;
    }

    // calculate the time of flight for 1 meter [ns/1m]
    if (isMC)
        return fTime / (GetFlightPath() / 100.);
    else
        return tof / (GetFlightPath() / 100.) + 1e9 / TMath::C();
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::CalculateTOFTAPS(Double_t tapsTime, Bool_t isMC)
{
    // Return the time-of-flight normalized to 1 meter [ns/1m] relative
    // to the TAPS time 'tapsTime'.
    //
    // If 'isMC' is kTRUE the MC timing is returned (different calculation).
    
    // calculate the particle tof
    Double_t tof = 0;

    //
    // TAPS
    // t_TAPS = t_trig - t_CFD
    // t = t_TAPS_ref - t_TAPS = t_CFD - t_CFD_ref
    if (fDetector == kTAPSDetector) 
    {
        tof = tapsTime - fTime;
    }
    //
    // CB
    // t_TAPS = t_trig - t_CFD
    // t_CB = t_NaI - t_trig
    // t = t_CB + t_TAPS = t_NaI - t_CFD
    else if (fDetector == kCBDetector) 
    {
        tof = fTime + tapsTime;
    }

    // calculate the time of flight for 1 meter [ns/1m]
    if (isMC)
        return fTime / (GetFlightPath() / 100.);
    else
        return tof / (GetFlightPath() / 100.) + 1e9 / TMath::C();
}

//______________________________________________________________________________
void TOA2BaseDetParticle::SetTheta(Double_t theta)
{
    // Set the theta angle keeping the magnitude and the azimuth angle constant (ROOT).

    Double_t mag = TMath::Sqrt(fX*fX + fY*fY + fZ*fZ); 
    Double_t phi = GetPhi();
    fX = mag * TMath::Sin(theta) * TMath::Cos(phi);
    fY = mag * TMath::Sin(theta) * TMath::Sin(phi);
    fZ = mag * TMath::Cos(theta);
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::GetTheta() const 
{
    // Return the polar angle (ROOT).

    return fX == 0.0 && fY == 0.0 && fZ == 0.0 ? 0.0 : TMath::ATan2(TMath::Sqrt(fX*fX + fY*fY), fZ); 
}

//______________________________________________________________________________
Double_t TOA2BaseDetParticle::GetPhi() const 
{
    // Return the azimuth angle (from -pi to pi) (ROOT).

    return fX == 0.0 && fY == 0.0 ? 0.0 : TMath::ATan2(fY, fX);
}

//______________________________________________________________________________
void TOA2BaseDetParticle::Print(Option_t* option) const
{
    // Print out the content of this class.

    // detector string
    Char_t detector[256] = "";
    switch (fDetector)
    {
        case kNoDetector:
            strcpy(detector, "no detector");
            break;
        case kCBDetector:
            strcpy(detector, "CB");
            break;
        case kTAPSDetector:
            strcpy(detector, "TAPS");
            break;
    }

    printf("TOA2BaseDetParticle content:\n");
    printf("PDG ID                 : %d\n", fPDG_ID);
    printf("Detector               : %s\n", detector);
    printf("Direction (x, y, z)    : %f, %f, %f\n", fX, fY, fZ);
    printf("Direction (theta, phi) : %f, %f\n", GetTheta()*TMath::RadToDeg(), GetPhi()*TMath::RadToDeg());
    printf("Energy                 : %f\n", fEnergy);
    printf("Time                   : %f\n", fTime);
    printf("Cluster size           : %d\n", fClusterSize);
    printf("Central element        : %d\n", fCentralElement);
    printf("Central energy         : %f\n", fCentralEnergy);
    printf("PID index              : %d\n", fPIDIndex);
    printf("PID energy             : %f\n", fPIDEnergy);
    printf("PID time               : %f\n", fPIDTime);
    printf("Central SG energy      : %f\n", fCentralSGEnergy);
    printf("Veto index             : %d\n", fVetoIndex);
    printf("Veto energy            : %f\n", fVetoEnergy);
    printf("Veto time              : %f\n", fVetoTime);
}

//______________________________________________________________________________
TOA2BaseDetParticle& TOA2BaseDetParticle::operator=(TOA2BaseDetParticle& p)
{
    // Assignment operator.
    
    fPDG_ID   = p.fPDG_ID;
    fDetector = p.fDetector;

    fX = p.fX;
    fY = p.fY;
    fZ = p.fZ;

    fEnergy = p.fEnergy;
    fTime   = p.fTime;

    fClusterSize = p.fClusterSize;

    fCentralElement = p.fCentralElement;
    fCentralEnergy  = p.fCentralEnergy;
    
    fPIDIndex  = p.fPIDIndex;
    fPIDEnergy = p.fPIDEnergy;
    fPIDTime   = p.fPIDTime;

    fCentralSGEnergy = p.fCentralSGEnergy;
    
    fVetoIndex       = p.fVetoIndex;
    fVetoEnergy      = p.fVetoEnergy;
    fVetoTime        = p.fVetoTime;
    
    return *this;
}

