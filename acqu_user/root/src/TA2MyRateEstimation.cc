// SVN Info: $Id: TA2MyRateEstimation.cc 558 2010-03-31 09:00:52Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyRateEstimation                                                  //
//                                                                      //
// Class used for the meson production rate estimation.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyRateEstimation.h"

ClassImp(TA2MyRateEstimation)


//______________________________________________________________________________
TA2MyRateEstimation::TA2MyRateEstimation(const char* name, TA2Analysis* analysis) 
    : TA2MyPhysics(name, analysis)
{
    // Constructor.
    
}

//______________________________________________________________________________
TA2MyRateEstimation::~TA2MyRateEstimation()
{
    // Destructor.

}

//______________________________________________________________________________
void TA2MyRateEstimation::SetConfig(Char_t* line, Int_t key)
{
    // Read in analysis configuration parameters and configure the class. 
    
    // default parent class SetConfig()
    TA2MyPhysics::SetConfig(line, key);
}

//______________________________________________________________________________ 
void TA2MyRateEstimation::PostInit()
{
    // Do some further initialisation after all setup parameters have been read in.
    

    ////////////////////////////////////////////////////////////////////////////
    // Create histograms                                                      //
    ////////////////////////////////////////////////////////////////////////////
 
    fH_eta2g_IM = new TH1F("eta2g_IM",  "eta2g_IM", 1000, 0, 1000);
    fH_eta6g_raw_IM = new TH1F("eta6g_raw_IM",  "eta6g_raw_IM", 1000, 0, 1000);
    fH_eta6g_IM = new TH1F("eta6g_IM",  "eta6g_IM", 1000, 0, 1000);
    fH_eta6g_pi0_IM = new TH1F("eta6g_pi0_IM",  "eta6g_pi0_IM", 1000, 0, 1000);

    // call PostInit of parent class
    TA2MyPhysics::PostInit();
}

//______________________________________________________________________________
void TA2MyRateEstimation::ReconstructPhysics()
{
    // Main analysis method that is called for every event.
    
    
    ////////////////////////////////////////////////////////////////////////////
    // eta -> 2g                                                              //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fNNeutral == 2 || fNNeutral == 3)
    {
        Bool_t allCB = kTRUE;
        for (Int_t i = 0; i < fNNeutral; i++)
        {
            if (fPartNeutral[i]->GetDetector() == kTAPSDetector) 
            {
                allCB = kFALSE;
                break;
            }
        }

        allCB = kTRUE;
        if (allCB)
        {
            // reconstruct eta
            TOA2RecEta2g eta(fNNeutral);
            eta.Reconstruct(fNNeutral, fPartNeutral);
            
            // get invariant mass, kinetic energy and theta
            TLorentzVector* p4Eta = eta.Get4Vector();
            Double_t im = p4Eta->M();
            fH_eta2g_IM->Fill(im);
        }
    }

    
    return;
}

