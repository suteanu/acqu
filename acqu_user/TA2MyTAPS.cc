// SVN Info: $Id: TA2MyTAPS.cc 689 2010-11-18 16:21:43Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyTAPS                                                            //
//                                                                      //
// My customized TAPS detector class.                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyTAPS.h"

ClassImp(TA2MyTAPS)


//______________________________________________________________________________
TA2MyTAPS::TA2MyTAPS(const char* name, TA2System* analysis )
    : TA2Apparatus(name, analysis, kValidTAPSDetectors)
{
    // Zero pointers and counters, add local SetConfig command list
    // to list of other apparatus commands
    
    fBaF2PWO = NULL;
    fVETO = NULL;
    fTapsFudge = 1.0;
    AddCmdList(kTAPSKeys);                  // for SetConfig()
}

//______________________________________________________________________________
TA2MyTAPS::~TA2MyTAPS()
{
    // Destructor.
    // Free up allocated memory.
}

//______________________________________________________________________________
TA2DataManager* TA2MyTAPS::CreateChild(const char* name, Int_t dclass)
{
    // Create a TA2Detector class for use with the TA2MyTAPS
    // Valid detector types are...

    if(!name) name = Map2String(dclass);

    switch (dclass)
    {
        case EChild_Det_BaF2PWO:
            fBaF2PWO = new TA2MyTAPS_BaF2PWO(name, this);
            return fBaF2PWO;
        case EChild_Det_Veto:
            fVETO = new TA2PlasticVETO(name, this);
            return fVETO;
        default:
            return NULL;
    }
}

//______________________________________________________________________________
void TA2MyTAPS::SetConfig(char* line, int key)
{
    // Load TAPS parameters from file or command line
    // TAPS specific configuration.
    
    Bool_t error = false;

    switch(key)
    {
        //TAPS Factor
        case ETAPSFactor:
            if(sscanf(line, "%lf", &fTapsFudge) < 1) error = true;
            break;
        
        // Command not found...possible pass to next config
        default:
            TA2Apparatus::SetConfig(line, key);
            break;;
    }

    if (error) PrintError(line);
    return;
}

//______________________________________________________________________________
void TA2MyTAPS::LoadVariable( )
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
    //TA2DataManager::LoadVariable("NCharged",  &fNCharged,      EISingleX);
    //TA2DataManager::LoadVariable("NProtons",  &fNtapsproton,      EISingleX);
    //TA2DataManager::LoadVariable("NNeutrons",  &fNtapsneutron,      EISingleX);
    //TA2DataManager::LoadVariable("NElectrons",  &fNtapselectron,      EISingleX);
    //TA2DataManager::LoadVariable("NGammas",  &fNtapsgamma,      EISingleX);
    //TA2DataManager::LoadVariable("NRootinos",  &fNtapsrootino,      EISingleX);
    
    TA2Apparatus::LoadVariable();
}

//______________________________________________________________________________
void TA2MyTAPS::PostInit( )
{
    // Initialise arrays used to correlate hits in BaF2 and Veto detectors.
    // Load 2D cuts file and get the contained cuts classes
    // Demand particle ID class...if not there self destruct
    // Does not come back if called with EErrFatal

    if(!fParticleID) PrintError("", "<Configuration aborted: ParticleID class MUST be specified>", EErrFatal);

    if (fBaF2PWO) fMaxParticle = fBaF2PWO->GetMaxCluster();
 
    // Finally call the default apparatus post initialise
    TA2Apparatus::PostInit();
}

//______________________________________________________________________________
void TA2MyTAPS::Reconstruct()
{
    /*
    UInt_t* id_clBaF2;
    HitCluster_t** clBaF2;
    UInt_t nBaF2;
    //UInt_t fNhitsVETO     = fVETO->GetNhits();               // nr. of veto hits
    //Int_t * fHitsVETO     = fVETO->GetHits();                // veto hits
 
    if (fBaF2)
    {
        id_clBaF2  = fBaF2->GetClustHit();            // central detector array
        clBaF2     = fBaF2->GetCluster();             // cluster array
        nBaF2      = fBaF2->GetNCluster();            // nr. of clusters
    }
    
    if (fBaF2PWO)
    {
        id_clBaF2  = fBaF2PWO->GetClustHit();            // central detector array
        clBaF2     = fBaF2PWO->GetCluster();             // cluster array
        nBaF2      = fBaF2PWO->GetNCluster();            // nr. of clusters
    }

    
    // set number of particles
    fNparticle = nBaF2;      

    // iterate over all cluster hits
    for (UInt_t i = 0; i < nBaF2; i++) 
    { 
        fParticleID->SetP4(fP4+i, kGamma, (fTapsFudge*(clBaF2[id_clBaF2[i]])->GetEnergy()), 
                           (clBaF2[id_clBaF2[i]])->GetMeanPosition());    
        fPDG_ID[i] = kGamma;

        fClusterTime[i] = (clBaF2[id_clBaF2[i]])->GetTime(); 
    }
    */
}

