// SVN Info: $Id: TA2MyTAPS_BaF2PWO.cc 689 2010-11-18 16:21:43Z werthm $

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


#include "TA2MyTAPS_BaF2PWO.h"

ClassImp(TA2MyTAPS_BaF2PWO)


//______________________________________________________________________________
TA2MyTAPS_BaF2PWO::TA2MyTAPS_BaF2PWO(const char* name, TA2System* apparatus)
    : TA2ClusterDetector(name, apparatus)
{
    // Ensure Short/Long gate array stuff is tagged as uninitialised
    fType = ENoType;
    fSGElement = NULL;
    fLGElement = NULL;
    fMaxSGElements = 0;
    fRandGen = NULL;
 
    fRandom = new TRandom3(123);
 
    // Add specialist TAPS command keys
    AddCmdList(kMyTAPS_BaF2PWOKeys);
}

//______________________________________________________________________________
TA2MyTAPS_BaF2PWO::~TA2MyTAPS_BaF2PWO()
{
    // Free up all allocated memory
    // ...arrays created at the initialisation stage
    
    for(UInt_t i = 0; i < fMaxSGElements; i++) 
    {
        if(fSGElement[i]) delete fSGElement[i];
    }
    
    delete fSGElement;
    DeleteClusterArrays();
    if (fRandGen) delete fRandGen;
}

//______________________________________________________________________________
void TA2MyTAPS_BaF2PWO::SaveDecoded( )
{
  // Save decoded info to Root Tree file
}

//______________________________________________________________________________
void TA2MyTAPS_BaF2PWO::SetConfig(Char_t* line, Int_t key)
{
    // Own special cluster setup
    // Cluster specific configuration
    
    Bool_t error = false;
    UInt_t seed;

    switch (key)
    {
        // Max number of BaF2-SG-clusters
        case EMyTAPS_BaF2PWO_SGMaxDet:
            if (sscanf(line, "%d", &fMaxSGElements) < 1) error = true;
            else
            {
                fSGElement = new HitD2A_t*[fMaxSGElements];
                fNSG = 0;
            }
            break;
        // SG-Pedestal and Gain of BaF2-Crystals
        case EMyTAPS_BaF2PWO_SG:
            // parameters for each element...need to input # of elements 1st
            if(fNSG >= fMaxSGElements)
            {
                PrintError(line,"<Too many detector elements input>");
                break;
            }   
            fSGElement[fNSG] = new HitD2A_t(line, fNSG, this);
            fNSG++;
            break;
        // Nearest neighbout input
        case EMyTAPS_BaF2PWO_ClustDetNeighbour:
            if(fNCluster < fNelement) fCluster[fNCluster] = new HitCluster_t(line, fNCluster);
            //if(fNCluster < fNelement) fCluster[fNCluster] = new MyHitClusterTAPS_t(line, fNCluster, 1, fRandGen);
            fNCluster++;
            break;
        // Random hit position
        case EMyTAPS_BaF2PWO_RandomHitPosition:
            if (sscanf(line, "%d", &seed) < 1)
            {
                fRandGen = new TRandom3();
                PrintMessage("Using random hit positioning within crystals (no seed specified)\n");
            }
            else 
            {
                fRandGen = new TRandom3(seed);
                PrintMessage("Using random hit positioning within crystals (using specified seed)\n");
            }
            break;
        // Command not found...check standard cluster detector list
        default:
            TA2ClusterDetector::SetConfig( line, key );
            break;;
    }

    if (error) PrintError(line);
    return;
}

//______________________________________________________________________________
void TA2MyTAPS_BaF2PWO::PostInit()
{
    // Some further initialisation after all setup parameters read in
    // Start with alignment offsets
    // Create space for various output arrays
    
    TA2ClusterDetector::PostInit();

    // init the type
    switch (GetNelement())
    {
        case 384:
            fType = EBaF2;
            break;
        case 402:
            fType = EBaF2_PWO_08;
            break;
        case 438:
            fType = EBaF2_PWO_09;
            break;
        default:
            fType = ENoType;
            break;
    }

    // init the long-gate pointer
    fLGElement = TA2Detector::GetElement();
}

