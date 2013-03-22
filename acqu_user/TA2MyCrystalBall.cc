// SVN Info: $Id: TA2MyCrystalBall.cc 495 2009-08-26 12:47:01Z werthm $

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


#include "TA2MyCrystalBall.h"
#include "TA2PlasticPID.h"
#include "TA2CalArray.h"

// Default list of detector classes that the TA2MyCrystalBall
// apparatus may contain
enum { ECB_PlasticPID, ECB_CalArray };
static Map_t kValidDetectors[] =
{
    {"TA2PlasticPID",     ECB_PlasticPID},
    {"TA2CalArray",       ECB_CalArray},
    {NULL, 		-1}
};

enum {ECB_Photon, ECB_Pi0, ECB_PiPlus, ECB_PiMinus, ECB_Proton, ECB_Neutron};

// Valid Keywords for command-line setup of CB apparatus
enum { ECBAngleLimits = 1000, ECBParticleCuts };
static const Map_t kCBKeys[] =
{
    {"AngleLimits:",    ECBAngleLimits},
    {"ParticleID-Cut:", ECBParticleCuts},
    {NULL,            -1}
};

ClassImp(TA2MyCrystalBall)

//-----------------------------------------------------------------------------
TA2MyCrystalBall::TA2MyCrystalBall( const char* name, TA2System* analysis  )
        :TA2Apparatus( name, analysis, kValidDetectors )
{
    // Zero pointers and counters, add local SetConfig command list
    // to list of other apparatus commands
    fNaI = NULL;
    fPID = NULL;
    fPIDdPhi = NULL;
    fMaxPIDdPhi = 0.0;
    fNCharged = 0;
    fAngDiffI = NULL;
    fPIDij = NULL;
    fPIDindex = fPIDdouble = NULL;
    fDeltaE = fDeltaEdouble = fEch = fECentral = NULL;
    fDeltaTheta = fDeltaPhi = NULL;
    fDeltaEOR = fEchOR = 0.0;
    fPIDEnergyDepo = NULL;
    fPIDElementIndex = NULL;
    fPIDNelem = 0;
    AddCmdList( kCBKeys );                  // for SetConfig()
}


//-----------------------------------------------------------------------------
TA2MyCrystalBall::~TA2MyCrystalBall()
{
    // Free up allocated memory

    // Needs upgrade

    if ( fDeltaE ) delete fDeltaE;
    if ( fPIDdPhi ) delete fPIDdPhi;
    if ( fPIDEnergyDepo ) delete fPIDEnergyDepo;
    if ( fPIDElementIndex ) delete fPIDElementIndex;
}

//-----------------------------------------------------------------------------
TA2DataManager* TA2MyCrystalBall::CreateChild(const char* name, Int_t dclass)
{
    // Create a TA2Detector class for use with the TA2MyCrystalBall
    // Valid detector types are...
    // 1. TA2PlasticPID
    // 2. TA2CalArray

    if ( !name ) name = Map2String( dclass );
    switch (dclass)
    {
    default:
        return NULL;
    case ECB_PlasticPID:
        fPID = new TA2PlasticPID( name, this );
        return fPID;
    case ECB_CalArray:
        fNaI = new TA2CalArray( name, this );
        return fNaI;
    }
}

//-----------------------------------------------------------------------------
void TA2MyCrystalBall::LoadVariable( )
{
    // Input name - variable pointer associations for any subsequent
    // cut or histogram setup.
    // LoadVariable( "name", pointer-to-variable, type-spec );
    // NB scaler variable pointers need the preceeding &
    //    array variable pointers do not.
    // type-spec ED prefix for a Double_t variable
    //           EI prefix for an Int_t variable
    // type-spec SingleX for a single-valued variable
    //           MultiX  for a multi-valued variable

    //                            name        pointer          type-spec
    TA2DataManager::LoadVariable("NCharged",  &fNCharged,      EISingleX);
    TA2DataManager::LoadVariable("DeltaE",    fDeltaE,         EDSingleX);
    TA2DataManager::LoadVariable("DeltaEdouble",fDeltaEdouble, EDSingleX);
    TA2DataManager::LoadVariable("Echarged",  fEch,            EDSingleX);
    TA2DataManager::LoadVariable("ECentral",  fECentral,       EDSingleX);
    TA2DataManager::LoadVariable("DeltaPhi",  fDeltaPhi,       EDSingleX);
    //TA2DataManager::LoadVariable("DeltaTheta",fDeltaTheta,     EDMultiX);
    TA2DataManager::LoadVariable("DeltaEOR",  &fDeltaEOR,      EDSingleX);
    TA2DataManager::LoadVariable("EchargedOR",&fEchOR,         EDSingleX);
    //
    TA2Apparatus::LoadVariable();
}

//-----------------------------------------------------------------------------
void TA2MyCrystalBall::SetConfig(Char_t* line, Int_t key)
{
    // Any special command-line input for Crystal Ball apparatus

    switch (key)
    {
    case ECBAngleLimits:
        // Angle difference limits
        Double_t tmp;
        if ( sscanf(line,"%lf%lf",&fMaxPIDdPhi,&tmp) != 2 )
        {
            PrintError(line,"<CB angle difference limits>");
            return;
        }
        break;
    default:
        // default main apparatus SetConfig()
        TA2Apparatus::SetConfig( line, key );
        break;
    }
}

//-----------------------------------------------------------------------------
void TA2MyCrystalBall::PostInit( )
{
    // Initialise arrays used to correlate hits in NaI and PiD.
    // Load 2D cuts file and get the contained cuts classes
    // Demand particle ID class...if not there self destruct
    // Does not come back if called with EErrFatal

    if ( !fParticleID )
        PrintError("",
                   "<Configuration aborted: ParticleID class MUST be specified>",
                   EErrFatal);
    // CB-PID Phi diff array
    fMaxParticle = fNaI->GetMaxCluster();
    fPIDEnergyDepo = new Double_t[fMaxParticle];
    fPIDElementIndex = new Int_t[fMaxParticle];
    if ( fPID )
    {
        Int_t nmax = fMaxParticle * fPID->GetNelem();
        fPIDdPhi = new Double_t[nmax];
        fPIDindex = new Int_t[fMaxParticle];
        fPIDdouble = new Int_t[fMaxParticle];
        fPIDij = new Int_t[nmax];
        fDeltaE = new Double_t[fPID->GetNelem()];
        fDeltaEdouble = new Double_t[fPID->GetNelem()];
        fEch = new Double_t[fPID->GetNelem()];
        fECentral = new Double_t[fPID->GetNelem()];
        fDeltaPhi = new Double_t[fPID->GetNelem()];
        fPIDNelem = fPID->GetNelem();
        fAngDiffI = new Int_t[nmax];
    }

    // Finally call the default apparatus post initialise
    TA2Apparatus::PostInit();
}

//-----------------------------------------------------------------------------
void TA2MyCrystalBall::Reconstruct( )
{
    // 1st levl particle reconstruction from Crystal Ball
    // Hit in NaI array required, Check if any PID

    /*
    fNparticle = 0;
    fNCharged = 0;

    Int_t nNaI = fNaI->GetNCluster();
    if ( nNaI < 1 ) return;                      // no hit in NaI..no particles
    HitCluster_t** clNaI = fNaI->GetCluster();   // NaI cluster struct pointers
    UInt_t* idNaI = fNaI->GetClustHit();         // NaI indices active clusts
    Int_t nPID;                                  // # PID hits
    if ( !fPID ) nPID = 0;                       // no PID
    else nPID = fPID->GetNhits();                // PID available, get # hits

    // No PID...assume all particles are photons as 1st guess and store
    // photon 4-vectors. Save PDG particle-type indices (kGamma) for future
    // convenience and make a fast exit
    if ( !nPID )
    {
        for ( Int_t i=0; i<nNaI; i++ )
        {
            fParticleID->SetP4(fP4+i, kGamma,
                               (clNaI[idNaI[i]])->GetEnergy(),
                               (clNaI[idNaI[i]])->GetMeanPosition());
            fPDG_ID[i] = kGamma;
            fPIDEnergyDepo[i] = 0.;
            fPIDElementIndex[i] = -1;
        }
        fNparticle = nNaI;
        fNCharged = 0;
        return;
    }

    // PID is there...automatic variables for particle ID analysis
    HitCluster_t* cl;                // cluster from NaI (or other EM cal)
    Int_t i,j,k,l,m;                 // loop indicies etc.
    Int_t* ijp = fPIDij;
    Int_t pidElem;
    Double_t diffPIDValue;
    Double_t* diffPID = fPIDdPhi;    // CB-PID phi difference array ptr.

    // Ensure energy/angle record arrays initialised to "zero"
    for ( j=0; j<fPIDNelem; j++ )     // Do all PID elements
    {
        fDeltaE[j] = (Double_t)ENullHit;
        fDeltaEdouble[j] = (Double_t)ENullHit;;
        fEch[j] = (Double_t)ENullHit;
        fECentral[j] = (Double_t)ENullHit;
        fDeltaPhi[j] = (Double_t)ENullHit;
    }

    // Sort CB-PID angle differences for all NaI clusters
    // all PID elements
    for ( i=0; i<nNaI; i++ )                       // loop cluster hits
    {
        cl = clNaI[idNaI[i]];                        // -> cluster
        fPIDdouble[i] = EMyCBNullPartner;              // init no PID double hit
        fPIDindex[i] = EMyCBNullPartner;               // init no PID partner
        // Loop over PID hits... store PID-CB phi differences
        for ( j=0; j<nPID; j++ )
        {
            pidElem = fPID->GetHits(j);
            diffPIDValue = TMath::Abs( fPID->GetPosition( pidElem )->Z()
                                     - cl->GetPhi() );
            
            // special correction for unluckily phi-mapped element 6
            if (gAR->GetProcessType() != EMCProcess && 
                pidElem == 6 && diffPIDValue > 345 && diffPIDValue <= 360) diffPIDValue = TMath::Abs(diffPIDValue - 360);

            *diffPID++ = diffPIDValue;
            *ijp++ = i | (j<<16);                     // store i,j indices
        }
    }
    // Sort PID-CB phi difference arrays in ascending order
    // If phi difference below acceptable limit store PID - CB combination
    // Also look for doubles and store 2nd-PID - CB combination
    TMath::Sort( nNaI*nPID, fPIDdPhi, fAngDiffI, EFalse );
    for ( i=0; i<nPID; i++ )
    {
        if ( fPIDdPhi[ fAngDiffI[i] ] > fMaxPIDdPhi ) break;// ang diff within lim?
        m = fPIDij[fAngDiffI[i]];                          // get indices
        j = m & 0xffff;                                    // CB index
        k = (m>>16) & 0xffff;                              // PID index
        if ( fPIDindex[j] == EMyCBNullPartner ) fPIDindex[j] = k;
        else if ( fPIDdouble[j] == EMyCBNullPartner )
        {
            if ( TMath::Abs( fPID->GetHits( fPIDindex[j] ) - fPID->GetHits( k ) )
                    == 1 ) fPIDdouble[j] = k;
        }
    }

    // Loop over NaI clusters, check for correlated PID
    // Store diagnostic variables and do particle ID by dE-E
    TVector3* trackDir;                            // track direction
    for ( i=0,l=0; i<nNaI; i++ )
    {
        cl = clNaI[idNaI[i]];                        // CB cluster
        trackDir = cl->GetMeanPosition();            // direction from CB
        if ( (j = fPIDindex[i]) == EMyCBNullPartner ) 
        {
            fPDG_ID[i] = kGamma;
            fPIDEnergyDepo[i] = 0.;
            fPIDElementIndex[i] = -1;
        }
        else
        {
            k = fPID->GetHits(j);                      // PID element index
            // PID - CB energies for correlated hit
            fDeltaE[k] = fPID->GetEnergy(k)
                         * TMath::Abs( TMath::Sin( cl->GetTheta() * TMath::DegToRad() ) );
            fEch[k] = cl->GetEnergy();                 // cluster energy
            fECentral[k] = cl->GetCentralFrac();       // cluster central fraction
            fDeltaPhi[k] = *(fPIDdPhi + i*nPID + j);   // phi difference
            // Check PID double hits...add double to fDeltaE
            if ( fPIDdouble[i] != EMyCBNullPartner )
            {
                fDeltaEdouble[k] = fPID->GetEnergy( fPID->GetHits(fPIDdouble[i]) )
                                   * TMath::Abs( TMath::Sin( cl->GetTheta() * TMath::DegToRad() ) );
                fDeltaE[k] += fDeltaEdouble[k];
            }
            
            // Save summed up PID energy in cluster array
            fPIDEnergyDepo[i] = fDeltaE[k];
            fPIDElementIndex[i] = k;
            
            // Now check particle ID by dE-E (if its initialised)
            fPDG_ID[i] = kRootino;
            fNCharged++;  // start unknown, charged
            if ( fPCut )                          // If cuts sorted
            {
                m = fPCutStart[k];                  // Start cut index for pid[k]
                fDeltaEOR = fDeltaE[k];             // OR for common dE-E cut
                fEchOR = fEch[k];                   // OR for common dE-E cut
                for ( Int_t n=0; n<fNSectorCut[k]; n++ ) // Loop over specified cuts
                {
                    if ( fPCut[m]->Test() )           // Condition met?
                    {
                        fPDG_ID[i] = GetCutPDGIndex(m); // OK get associated PDG index
                        break;                          // cut OK so exit loop
                    }
                    m++;                              // for next try
                }
            }
        }
        // Set 4 vector on the basis of ID above
        //fParticleID->SetP4( fP4+i,fPDG_ID[i],cl->GetEnergy(),trackDir );
        //fP4tot += fP4[i];                    // accumulate total 4 mom.
    }
    fNparticle = nNaI;
    if ( !fNparticle ) return;
    fMtot = fP4tot.M();
    */
}

