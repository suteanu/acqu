// SVN Info: $Id: TOA2BaseDetParticle.h 1261 2012-07-28 20:48:41Z werthm $

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


#ifndef OSCAR_TOA2BaseDetParticle
#define OSCAR_TOA2BaseDetParticle

#include "TObject.h"
#include "TClass.h"
#include "TVector3.h"
#include "TPDGCode.h"
#include "TLorentzVector.h"

#include "TOA2Detector.h"
#include "TOPhysics.h"


class TOA2BaseDetParticle : public TObject
{

protected:
    PDG_t fPDG_ID;                              // assumed PDG ID
    A2Detector_t fDetector;                     // Detector in which the particle was found

    Double_t fX;                                // X-coordinate of reconstructed hit position [cm]
    Double_t fY;                                // Y-coordinate of reconstructed hit position [cm]
    Double_t fZ;                                // Z-coordinate of reconstructed hit position [cm]
    
    Double_t fEnergy;                           // detected energy [MeV]
    Double_t fTime;                             // detected time [ns]
    
    Int_t fClusterSize;                         // Cluster size

    Int_t fCentralElement;                      // ID of central element
    Double_t fCentralEnergy;                    // energy deposited in the central element of the cluster

    Int_t fPIDIndex;                            // index of the PID element (CB only)
    Double_t fPIDEnergy;                        // energy deposited in the PID (CB only)
    Double_t fPIDTime;                          // time of PID hit (CB only)
    
    Double_t fCentralSGEnergy;                  // Short gate energy of central element (TAPS only)
    
    Int_t fVetoIndex;                           // index of the Veto element (TAPS only)
    Double_t fVetoEnergy;                       // energy deposited in the Veto (TAPS only)
    Double_t fVetoTime;                         // time of veto hit (TAPS only)

public:
    TOA2BaseDetParticle();
    TOA2BaseDetParticle(const TOA2BaseDetParticle& orig);
    virtual ~TOA2BaseDetParticle() { }
 
    void Calculate4Vector(TLorentzVector* p4, Double_t mass);
    void Calculate4Vector(TLorentzVector* p4, Double_t mass, Double_t energy);
    Bool_t Calculate4VectorTOFTagger(TLorentzVector* p4, Double_t mass, Double_t taggerTime, Bool_t isMC = kFALSE);
    Double_t CalculateEkinTOFTagger(Double_t mass, Double_t taggerTime, Bool_t isMC = kFALSE);
    Double_t CalculateEkinTOFTAPS(Double_t mass, Double_t tapsTime, Bool_t isMC = kFALSE);
    Double_t CalculateTOFTagger(Double_t taggerTime, Bool_t isMC = kFALSE);
    Double_t CalculateTOFCB(Double_t cbTime, Bool_t isMC = kFALSE);
    Double_t CalculateTOFTAPS(Double_t tapsTime, Bool_t isMC = kFALSE);

    void SetPDG_ID(PDG_t id) { fPDG_ID = id; }
    void SetDetector(A2Detector_t d) { fDetector = d; }

    void SetX(Double_t x) { fX = x; }
    void SetY(Double_t y) { fY = y; }
    void SetZ(Double_t z) { fZ = z; }
    void SetPosition(TVector3* pos) { fX = pos->X(); fY = pos->Y(); fZ = pos->Z(); }
    void SetTheta(Double_t theta);

    void SetEnergy(Double_t energy) { fEnergy = energy; }
    void SetTime(Double_t time) { fTime = time; }

    void SetClusterSize(Int_t size) { fClusterSize = size; }

    void SetCentralElement(Int_t e) { fCentralElement = e; }
    void SetCentralEnergy(Double_t energy) { fCentralEnergy = energy; }

    void SetPIDIndex(Int_t index) { fPIDIndex = index; }
    void SetPIDEnergy(Double_t energy) { fPIDEnergy = energy; }
    void SetPIDTime(Double_t time) { fPIDTime = time; }
    
    void SetCentralSGEnergy(Double_t sgE) { fCentralSGEnergy = sgE; }
    
    void SetVetoIndex(Int_t index) { fVetoIndex = index; }
    void SetVetoEnergy(Double_t energy) { fVetoEnergy = energy; }
    void SetVetoTime(Double_t time) { fVetoTime = time; }

    PDG_t GetPDG_ID() const { return fPDG_ID; }
    A2Detector_t GetDetector() const { return fDetector; }

    Double_t GetX() const { return fX; }
    Double_t GetY() const { return fY; }
    Double_t GetZ() const { return fZ; }
    Double_t GetFlightPath() const { return TMath::Sqrt(fX*fX + fY*fY + fZ*fZ); }
    Double_t GetTheta() const;
    Double_t GetPhi() const;

    Double_t GetEnergy() const { return fEnergy; }
    Double_t GetTime() const { return fTime; }

    Int_t GetClusterSize() const { return fClusterSize; }
    
    Int_t GetCentralElement() const { return fCentralElement; }
    Double_t GetCentralEnergy() const { return fCentralEnergy; }
    
    Int_t GetPIDIndex() const { return fPIDIndex; }
    Double_t GetPIDEnergy() const { return fPIDEnergy; }
    Double_t GetPIDTime() const { return fPIDTime; }
   
    Double_t GetCentralSGEnergy() const { return fCentralSGEnergy; }
    
    Int_t GetVetoIndex() const { return fVetoIndex; }
    Double_t GetVetoEnergy() const { return fVetoEnergy; }
    Double_t GetVetoTime() const { return fVetoTime; }
    
    virtual void Print(Option_t* option = "") const;

    TOA2BaseDetParticle& operator=(TOA2BaseDetParticle& p);

    ClassDef(TOA2BaseDetParticle, 1)  // Base class for detected particles in the A2 setup
};

#endif

