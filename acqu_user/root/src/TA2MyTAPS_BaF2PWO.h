// SVN Info: $Id: TA2MyTAPS_BaF2PWO.h 689 2010-11-18 16:21:43Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyTAPS_BaF2PWO                                                    //
//                                                                      //
// My customized TAPS combined BaF2/PWO detector class.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef __TA2MyTAPS_BaF2PWO_h__
#define __TA2MyTAPS_BaF2PWO_h__

#include "TRandom3.h"
#include "MCBranchID.h"
#include "TA2ClusterDetector.h"
#include "MyHitClusterTAPS_t.h"


enum { 
    EMyTAPS_BaF2PWO_SGMaxDet = 999,
    EMyTAPS_BaF2PWO_SG,
    EMyTAPS_BaF2PWO_ClustDetMax,
    EMyTAPS_BaF2PWO_ClustDetNeighbour,
    EMyTAPS_BaF2PWO_RandomHitPosition
};


static const Map_t kMyTAPS_BaF2PWOKeys[] = {
  {"TAPSSGMaxDet:"        ,  EMyTAPS_BaF2PWO_SGMaxDet          },
  {"TAPSSG:"              ,  EMyTAPS_BaF2PWO_SG                },
  {"Next-TAPS:"           ,  EMyTAPS_BaF2PWO_ClustDetNeighbour },
  {"RandomHitPosition:"   ,  EMyTAPS_BaF2PWO_RandomHitPosition },
  {NULL, -1 }
};


enum ETAPSType {
    ENoType,
    EBaF2,
    EBaF2_PWO_08,
    EBaF2_PWO_09,
};
typedef ETAPSType TAPSType_t;


class TA2MyTAPS_BaF2PWO : public TA2ClusterDetector 
{

protected:
    TAPSType_t fType;                                   // type of TAPS configuration
    HitD2A_t** fSGElement;                              // short-gate detector elements
    HitD2A_t** fLGElement;                              // long-gate detector elements
    UInt_t fMaxSGElements;                              // size of short-gate arrays
    UInt_t fNSG;                                        // actual # short-gate elements entered
    TRandom3* fRandGen;                                 // random number generator for hit position smearing
    
public:
    TA2MyTAPS_BaF2PWO(const char*, TA2System*);     
    virtual ~TA2MyTAPS_BaF2PWO();
    void PostInit();
    void SaveDecoded();
    void ReadDecoded();
    void SetConfig(Char_t*, Int_t);

    TAPSType_t GetType() const { return fType; }

    HitD2A_t* GetLGElement(Int_t i)
    {
        // get LG element i
        if (fLGElement) 
        {
            if (fLGElement[i]) return fLGElement[i];
            else return 0;
        }
        else return 0;
    }

    HitD2A_t* GetSGElement(Int_t i)
    {
        // get SG element i
        if (fSGElement) 
        {
            if (fSGElement[i]) return fSGElement[i];
            else return 0;
        }
        else return 0;
    }

    Double_t GetSGEnergy(Int_t i)
    {
        // get short-gate energy of crystal i
        if (fSGElement)
        {
            if (fSGElement[i])
            {
                fSGElement[i]->Calibrate();
                return fSGElement[i]->GetEnergy();
            }
            else return 0;
        } 
        else return 0;
    }

    Double_t GetLGEnergy(Int_t i)
    {
        // get long-gate energy of crystal i
        if (fLGElement) 
        {
            if (fLGElement[i]) return (fLGElement[i]->GetEnergy());
            else return 0;
        }
        else return 0;
    }

    ClassDef(TA2MyTAPS_BaF2PWO, 1)
};

//______________________________________________________________________________
inline void TA2MyTAPS_BaF2PWO::ReadDecoded( )
{
    // Read back "decoded" data for the BaF2 array
    // In this case produced by the GEANT3 CB simulation
    // Remove j-- in indexing 2/11/05
    // Add energy thresholds 2/11/05
    // Note in Geant4 A2 simulation the numbering runs from 1-384...include j-- line

    UInt_t i,j,k;
    Double_t total = 0.0;                               // total energy counter
    fNhits = *(Int_t*)(fEvent[EI_ntaps]);               // detector hits
    Float_t* energy = (Float_t*)(fEvent[EI_ectapsl]);   // energy array
    Float_t* time = (Float_t*)(fEvent[EI_tctaps]);      // time array
    Int_t* index = (Int_t*)(fEvent[EI_ictaps]);         // element indices
    Double_t E,T;                                       // energy, time
    Double_t EthrLow,EthrHigh;                          // energy thresholds

    for( i=0,k=0; i<fNhits; i++ ){                      // loop over hits
        j = (*index++);
        j--;                                              // Need this line for A2 G4 simulation
        if (fElement[j]->IsIgnored()) continue;           // check ignored elements
        EthrLow = fElement[j]->GetEnergyLowThr();         // get low thresh
        EthrHigh = fElement[j]->GetEnergyHighThr();       // and high thresh
        //E = 1000 * (*energy++) * fEnergyScale;            // G3 output in GeV
        
        // smear energy
        E = (*energy++) * fEnergyScale * fElement[j]->GetA1();
        E+=fRandom->Gaus(0.0, 0.0059 * TMath::Sqrt(E) + 0.0191 * E);
        E*=1000.;
        // end smear energy
        
        if( (E < EthrLow) || (E > EthrHigh) ) {           // energy inside thresh?
            if(fIsTime) time++;
            continue;
        }
        fEnergy[j] = E;                                   // store energy
        fEnergyOR[k] = E;                                 
        if (fIsTime){
          T=*time++;
          T+=fElement[j]->GetT0();                      // element time offset
          T+=fRandom->Gaus(0.0, 0.5);                   // time smearing
          fTime[j] = T;
          fTimeOR[k] = T;                               // store time
        }

        fHits[k] = j;                                     // store index
        total += E;                                       // energy total counter
        k++;                                              // hits inside thresh
    }
    fHits[k] = EBufferEnd;                              // end markers
    fTimeOR[k] = EBufferEnd;
    fEnergyOR[k] = EBufferEnd;
    fNhits = k;                                         // # hits inside thresh
    fTotalEnergy = total;                               // total energy
}

#endif

