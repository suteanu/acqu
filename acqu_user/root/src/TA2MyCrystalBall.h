// SVN Info: $Id: TA2MyCrystalBall.h 471 2009-07-14 15:28:28Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyCrystalBall                                                     //
//                                                                      //
// My customized CrystalBall detector class.                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef __TA2MyCrystalBall_h__
#define __TA2MyCrystalBall_h__

#include "TA2Apparatus.h"            // Acqu-Root histogrammer
#include "TFile.h"
#include "TCutG.h"

class TA2ClusterDetector;

enum { EMyCBNullPartner = -999999 };

class TA2MyCrystalBall : public TA2Apparatus
{
protected:
    TA2ClusterDetector* fNaI;    // Pointer to NaI array
    TA2Detector* fPID;           // Pointer to PID
    Double_t* fPIDdPhi;          // PID-CB Phi differences
    Double_t  fMaxPIDdPhi;       // Max PID-CB Phi difference
    Int_t*    fAngDiffI;         // for general sorting of angle differences
    Int_t*    fPIDij;            // back reference PID j, to CB i
    Int_t*    fPIDindex;         // Delta-E index associated with cluster
    Int_t*    fPIDdouble;        // Delta-E 2nd index associated with cluster
    Double_t* fDeltaE;           // Cluster correlated PID energy deposit
    Double_t* fDeltaEdouble;     // Cluster correlated PID energy deposit
    Double_t* fECentral;         // Fraction total energy in central clust elem.
    Double_t* fEch;              // PID correlated Cluster energy deposit
    Double_t* fDeltaPhi;         // Difference in PiD and cluster phi
    Double_t* fDeltaTheta;       // Difference in PiD and cluster phi
    Double_t fDeltaEOR;          // OR of Delta-E (for common dE-E cuts)
    Double_t fEchOR;             // OR correlated cluster-E (common dE-E cuts)
    Int_t     fNCharged;         // Number of CB clusters identified as charged
    Double_t* fPIDEnergyDepo;    // another array for the energy deposited in the PID (CB cluster wise)
    Int_t* fPIDElementIndex;     // array for the  PID element indices (CB cluster wise)
    Int_t fPIDNelem;             // number of PID elements

public:
    TA2MyCrystalBall( const char*, TA2System* );  // pass ptr to analyser
    virtual ~TA2MyCrystalBall();                  // destructor
    void PostInit();                    // some setup after parms read in
    TA2DataManager* CreateChild( const char*, Int_t );
    void LoadVariable(  );              // display setup
    void Reconstruct( );                // event by event analysis
    void Cleanup();                     // reset at end of event
    void SetConfig( Char_t*, Int_t );   // setup decode in implement

    TA2ClusterDetector* GetNaI() { return fNaI; }
    TA2Detector* GetPID() { return fPID; }
    Int_t GetNCharged() { return fNCharged; }
    Double_t* GetDeltaE() { return fDeltaE; }
    Double_t* GetPIDEnergyDepo() { return fPIDEnergyDepo; }
    Int_t* GetPIDElementIndex() { return fPIDElementIndex; }

    // Root needs this line for incorporation in dictionary
    ClassDef(TA2MyCrystalBall,1)
};

//-----------------------------------------------------------------------------
inline void TA2MyCrystalBall::Cleanup( )
{
    // Clear any arrays holding variables
    TA2DataManager::Cleanup();

    /*
    if ( !fNparticle ) return;
    Int_t i;
    for ( i=0; i<fNparticle; i++ )
    {
        fPDG_ID[i] = 0;
    }
    fP4tot.SetXYZT(0.,0.,0.,0.);
    fNparticle = 0;
    fNCharged = 0;
    */
}

#endif

