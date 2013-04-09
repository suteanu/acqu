// SVN Info: $Id: TA2MyClusterCalib.cc 744 2011-01-13 18:29:10Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyClusterCalib                                                    //
//                                                                      //
// Class used for cluster calibration using MC data.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyClusterCalib.h"

ClassImp(TA2MyClusterCalib)


//______________________________________________________________________________
TA2MyClusterCalib::TA2MyClusterCalib(const char* name, TA2Analysis* analysis) 
    : TA2MyPhysics(name, analysis)
{
    // Constructor.
    
    // command-line recognition for SetConfig()
    AddCmdList(myClusterCalibConfigKeys);

    // init members
    fIsPhotonCalib = kFALSE;
}

//______________________________________________________________________________
TA2MyClusterCalib::~TA2MyClusterCalib()
{
    // Destructor.

}

//______________________________________________________________________________
void TA2MyClusterCalib::SetConfig(Char_t* line, Int_t key)
{
    // Read in analysis configuration parameters and configure the class. 
    
    switch (key)
    {
        case EMCC_PHOTON_CALIB:
        {
            // activate photon cluster calibration
            fIsPhotonCalib = kTRUE;

            // connect branches
            //fInputChain->SetBranchAddress("Px_l0101", &fPhotonMC_Px);
            //fInputChain->SetBranchAddress("Py_l0101", &fPhotonMC_Py);
            //fInputChain->SetBranchAddress("Pz_l0101", &fPhotonMC_Pz);
            //fInputChain->SetBranchAddress("Pt_l0101", &fPhotonMC_Pt);
            //fInputChain->SetBranchAddress("En_l0101", &fPhotonMC_En);
            
            Info("SetConfig", "Switching on the photon cluster calibration");
            break;
        }
        default:
        {
            // default parent class SetConfig()
            TA2MyPhysics::SetConfig(line, key);
            break;
        }
    }
}

//______________________________________________________________________________ 
void TA2MyClusterCalib::PostInit()
{
    // Do some further initialisation after all setup parameters have been read in.
    

    ////////////////////////////////////////////////////////////////////////////
    // Create histograms                                                      //
    ////////////////////////////////////////////////////////////////////////////
 
    Char_t name[256];
    
    //
    // photon histograms
    //
    
    if (fIsPhotonCalib)
    {
        fH_Photon_Energy_MC    = new TH1F("Photon_Energy_MC",    "Photon_Energy_MC",    2000, 0, 2000);
        
        fH_Photon_CB_Diff_Px      = new TH1F("Photon_CB_Diff_Px",      "Photon_CB_Diff_Px",      200, -100, 100);
        fH_Photon_CB_Diff_Py      = new TH1F("Photon_CB_Diff_Py",      "Photon_CB_Diff_Py",      200, -100, 100);
        fH_Photon_CB_Diff_Pz      = new TH1F("Photon_CB_Diff_Pz",      "Photon_CB_Diff_Pz",      200, -100, 100);
        fH_Photon_CB_Diff_E       = new TH1F("Photon_CB_Diff_E",       "Photon_CB_Diff_E",       200, -100, 100);
        fH_Photon_CB_Diff_Theta   = new TH1F("Photon_CB_Diff_Theta",   "Photon_CB_Diff_Theta",   200, -100, 100);
        fH_Photon_CB_Diff_Phi     = new TH1F("Photon_CB_Diff_Phi",     "Photon_CB_Diff_Phi",     200, -100, 100);
        fH_Photon_CB_CorrFac_Tot  = new TH1F("Photon_CB_CorrFac_Tot",  "Photon_CB_CorrFac_Tot",  500, 0, 5);
        
        fH_Photon_TAPS_Diff_Px      = new TH1F("Photon_TAPS_Diff_Px",      "Photon_TAPS_Diff_Px",      200, -100, 100);
        fH_Photon_TAPS_Diff_Py      = new TH1F("Photon_TAPS_Diff_Py",      "Photon_TAPS_Diff_Py",      200, -100, 100);
        fH_Photon_TAPS_Diff_Pz      = new TH1F("Photon_TAPS_Diff_Pz",      "Photon_TAPS_Diff_Pz",      200, -100, 100);
        fH_Photon_TAPS_Diff_E       = new TH1F("Photon_TAPS_Diff_E",       "Photon_TAPS_Diff_E",       200, -100, 100);
        fH_Photon_TAPS_Diff_Theta   = new TH1F("Photon_TAPS_Diff_Theta",   "Photon_TAPS_Diff_Theta",   200, -100, 100);
        fH_Photon_TAPS_Diff_Phi     = new TH1F("Photon_TAPS_Diff_Phi",     "Photon_TAPS_Diff_Phi",     200, -100, 100);
        fH_Photon_TAPS_CorrFac_Tot  = new TH1F("Photon_TAPS_CorrFac_Tot",  "Photon_TAPS_CorrFac_Tot",  500, 0, 5);
    
        for (Int_t i = 0; i < 180; i++)
        {
            sprintf(name, "Photon_CB_CorrFac_%d", i);
            fH_Photon_CB_CorrFac[i] = new TH1F(name, name, 500, 0, 5);
            sprintf(name, "Photon_CB_CorrTheta_%d", i);
            fH_Photon_CB_CorrTheta[i] = new TH1F(name, name, 200, -10, 10);
            
            sprintf(name, "Photon_TAPS_CorrFac_%d", i);
            fH_Photon_TAPS_CorrFac[i] = new TH1F(name, name, 500, 0, 5);
            sprintf(name, "Photon_TAPS_CorrTheta_%d", i);
            fH_Photon_TAPS_CorrTheta[i] = new TH1F(name, name, 200, -10, 10);
        }
    }

    fH_Calib_CB_2g_IM  = new TH2F("Calib.CB.2g.IM",  "Calib.CB.2g.IM",  1000, 0, 1000, 720, 0, 720);
    fH_Calib_CB_2g_IM2 = new TH2F("Calib.CB.2g.IM2", "Calib.CB.2g.IM2", 1250, 0, 500000, 720, 0, 720);
    fH_Calib_TAPS_1g_IM  = new TH2F("Calib.TAPS.1g.IM",  "Calib.TAPS.1g.IM",  1000, 0, 1000, 384, 0, 384);
    fH_Calib_TAPS_1g_IM2 = new TH2F("Calib.TAPS.1g.IM2", "Calib.TAPS.1g.IM2", 1250, 0, 500000, 384, 0, 384);
    
    fH_Calib_TAPS_2g_Time = new TH2F("Calib.TAPS.2g.Time", "Calib.TAPS.2g.Time", 1600, -20, 20, 402, 0, 402);
     
    fH_CB_2g_IM = new TH1F("CB_2g_IM",  "CB_2g_IM", 1000, 0, 1000);
    fH_CB_Pion_Avr_E = new TH1F("CB_Pion_Avr_E",  "CB_Pion_Avr_E", 1000, 0, 1000);
    fH_CB_Eta_Avr_E = new TH1F("CB_Eta_Avr_E",  "CB_Eta_Avr_E", 1000, 0, 1000);
    
    fH_TAPS_1g_IM = new TH1F("TAPS_1g_IM",  "TAPS_1g_IM", 1000, 0, 1000);
    fH_TAPS_2g_IM = new TH1F("TAPS_2g_IM",  "TAPS_2g_IM", 1000, 0, 1000);
    fH_TAPS_Pion_Avr_E = new TH1F("TAPS_Pion_Avr_E",  "TAPS_Pion_Avr_E", 2000, 0, 2000);
    fH_TAPS_Eta_Avr_E = new TH1F("TAPS_Eta_Avr_E",  "TAPS_Eta_Avr_E", 2000, 0, 2000);

    // call PostInit of parent class
    TA2MyPhysics::PostInit();
}

//______________________________________________________________________________
void TA2MyClusterCalib::ReconstructPhysics()
{
    // Main analysis method that is called for every event.
    

    ////////////////////////////////////////////////////////////////////////////
    // Collect MC generated particles                                         //
    ////////////////////////////////////////////////////////////////////////////
    
    TLorentzVector p4PhotonMC;

    // photon cluster calibration
    if (fIsPhotonCalib)
    {
        // get event
        //fInputChain->GetEntry(fEventCounter);

        // set photon 4-vector
        p4PhotonMC.SetPxPyPzE(1000*fPhotonMC_Px*fPhotonMC_Pt, 
                              1000*fPhotonMC_Py*fPhotonMC_Pt, 
                              1000*fPhotonMC_Pz*fPhotonMC_Pt, 
                              1000*fPhotonMC_En);

        // fill generated photon energy
        fH_Photon_Energy_MC->Fill(p4PhotonMC.E());
    }


    ////////////////////////////////////////////////////////////////////////////
    // CB Photon cluster energy correction calibration                        //
    ////////////////////////////////////////////////////////////////////////////
   
    if (fIsPhotonCalib && fNNeutral == 1 && fPartNeutral[0]->GetDetector() == kCBDetector)
    {
        // construct photon 4-vector
        TLorentzVector p4Photon;
        fPartNeutral[0]->Calculate4Vector(&p4Photon, 0);
        Double_t theta = p4Photon.Theta()*TMath::RadToDeg();
        Double_t phi = p4Photon.Phi()*TMath::RadToDeg();
        Double_t theta_mc = p4PhotonMC.Theta()*TMath::RadToDeg();
        Double_t phi_mc = p4PhotonMC.Phi()*TMath::RadToDeg();

        // debug output
        if (fEventCounter < 10) 
        {
            Info("ReconstructPhysics", "\nMC:  %f %f %f %f\n"
                                       "rec: %f %f %f %f\n", 
                 p4PhotonMC.Px(), p4PhotonMC.Py(), p4PhotonMC.Pz(), p4PhotonMC.E(), 
                 p4Photon.Px(), p4Photon.Py(), p4Photon.Pz(), p4Photon.E());
        }
        
        // fill 4-vector differences
        fH_Photon_CB_Diff_Px->Fill(p4PhotonMC.Px() - p4Photon.Px());
        fH_Photon_CB_Diff_Py->Fill(p4PhotonMC.Py() - p4Photon.Py());
        fH_Photon_CB_Diff_Pz->Fill(p4PhotonMC.Pz() - p4Photon.Pz());
        fH_Photon_CB_Diff_E->Fill(p4PhotonMC.E() - p4Photon.E());
        fH_Photon_CB_Diff_Theta->Fill(theta_mc - theta);
        fH_Photon_CB_Diff_Phi->Fill(phi_mc - phi);

        // fill photon energy
        fH_Photon_CB_CorrFac_Tot->Fill(p4PhotonMC.E() / p4Photon.E());
        fH_Photon_CB_CorrFac[Int_t(theta)]->Fill(p4PhotonMC.E() / p4Photon.E());

        // fill theta difference
        fH_Photon_CB_CorrTheta[Int_t(theta)]->Fill(theta_mc - theta);
    }


    ////////////////////////////////////////////////////////////////////////////
    // TAPS Photon cluster energy correction calibration                      //
    ////////////////////////////////////////////////////////////////////////////
   
    if (fIsPhotonCalib && fNNeutral == 1 && fPartNeutral[0]->GetDetector() == kTAPSDetector)
    {
        // construct photon 4-vector
        TLorentzVector p4Photon;
        fPartNeutral[0]->Calculate4Vector(&p4Photon, 0);
        Double_t theta = p4Photon.Theta()*TMath::RadToDeg();
        Double_t phi = p4Photon.Phi()*TMath::RadToDeg();
        Double_t theta_mc = p4PhotonMC.Theta()*TMath::RadToDeg();
        Double_t phi_mc = p4PhotonMC.Phi()*TMath::RadToDeg();

        // debug output
        if (fEventCounter < 10) 
        {
            Info("ReconstructPhysics", "\nMC:  %f %f %f %f\n"
                                       "rec: %f %f %f %f\n", 
                 p4PhotonMC.Px(), p4PhotonMC.Py(), p4PhotonMC.Pz(), p4PhotonMC.E(), 
                 p4Photon.Px(), p4Photon.Py(), p4Photon.Pz(), p4Photon.E());
        }
        
        // fill 4-vector differences
        fH_Photon_TAPS_Diff_Px->Fill(p4PhotonMC.Px() - p4Photon.Px());
        fH_Photon_TAPS_Diff_Py->Fill(p4PhotonMC.Py() - p4Photon.Py());
        fH_Photon_TAPS_Diff_Pz->Fill(p4PhotonMC.Pz() - p4Photon.Pz());
        fH_Photon_TAPS_Diff_E->Fill(p4PhotonMC.E() - p4Photon.E());
        fH_Photon_TAPS_Diff_Theta->Fill(theta_mc - theta);
        fH_Photon_TAPS_Diff_Phi->Fill(phi_mc - phi);

        // fill photon energy
        fH_Photon_TAPS_CorrFac_Tot->Fill(p4PhotonMC.E() / p4Photon.E());
        fH_Photon_TAPS_CorrFac[Int_t(theta)]->Fill(p4PhotonMC.E() / p4Photon.E());
        
        // fill theta difference
        fH_Photon_TAPS_CorrTheta[Int_t(theta)]->Fill(theta_mc - theta);
     }


    ////////////////////////////////////////////////////////////////////////////
    // CB photon energy calibration                                           //
    ////////////////////////////////////////////////////////////////////////////
 
    // iterate over all cluster pairs hits
    for (UInt_t i = 0; i < fCBNCluster; i++)
    {
        // check if particle is neutral
        if (fPartCB[i]->GetPIDEnergy() > 0) continue;

        // find center of first cluster
        Int_t center1 = fPartCB[i]->GetCentralElement();
        
        // calculate photon 4-vector
        TLorentzVector p4Gamma1;
        fPartCB[i]->Calculate4Vector(&p4Gamma1, 0);

        // iterate over second element
        for (UInt_t j = i+1; j < fCBNCluster; j++)
        {
            // check if particle is neutral
            if (fPartCB[j]->GetPIDEnergy() > 0) continue;

            // find center of first cluster
            Int_t center2 = fPartCB[j]->GetCentralElement();
            
            // calculate photon 4-vector
            TLorentzVector p4Gamma2;
            fPartCB[j]->Calculate4Vector(&p4Gamma2, 0);

            // calculate 2-photon invariant mass
            TLorentzVector p4Meson = p4Gamma1 + p4Gamma2;
            Double_t invMass = p4Meson.M();
                
            fH_Calib_CB_2g_IM->Fill(invMass, center1);
            fH_Calib_CB_2g_IM->Fill(invMass, center2);
            fH_Calib_CB_2g_IM2->Fill(invMass*invMass, center1);
            fH_Calib_CB_2g_IM2->Fill(invMass*invMass, center2);
        }
    }


    ////////////////////////////////////////////////////////////////////////////
    // CB 2 gamma invariant mass                                              //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fNNeutral == 2 && fPartNeutral[0]->GetDetector() == kCBDetector
                       && fPartNeutral[1]->GetDetector() == kCBDetector)
    {
        TLorentzVector p4Gamma1;
        TLorentzVector p4Gamma2;
        fPartNeutral[0]->Calculate4Vector(&p4Gamma1, 0);
        fPartNeutral[1]->Calculate4Vector(&p4Gamma2, 0);
        
        // invariant mass
        Double_t im = (p4Gamma1 + p4Gamma2).M();
        fH_CB_2g_IM->Fill(im);
    
        // average photon energies of pion decay photons
        if (im > 110 && im < 160) fH_CB_Pion_Avr_E->Fill(0.5 * (p4Gamma1.E() + p4Gamma2.E()));

        // average photon energies of eta decay photons
        if (im > 510 && im < 600) fH_CB_Eta_Avr_E->Fill(0.5 * (p4Gamma1.E() + p4Gamma2.E()));
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // TAPS + CB 2 gamma invariant mass + photon energy calibration           //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fNNeutral == 2)
    {
        Int_t center;
        Double_t energy;

        // get central element of hit in TAPS - leave if no hit or both hits in TAPS
        if (fPartNeutral[0]->GetDetector() == kTAPSDetector    && fPartNeutral[1]->GetDetector() == kCBDetector) 
        {
            center = fPartNeutral[0]->GetCentralElement();
            energy = fPartNeutral[0]->GetEnergy();
        }
        else if (fPartNeutral[0]->GetDetector() == kCBDetector && fPartNeutral[1]->GetDetector() == kTAPSDetector) 
        {
            center = fPartNeutral[1]->GetCentralElement();
            energy = fPartNeutral[1]->GetEnergy();
        }
        else goto label_end_taps_calib;

        TLorentzVector p4Gamma1;
        TLorentzVector p4Gamma2;
        fPartNeutral[0]->Calculate4Vector(&p4Gamma1, 0);
        fPartNeutral[1]->Calculate4Vector(&p4Gamma2, 0);
        
        // invariant mass
        Double_t im = (p4Gamma1 + p4Gamma2).M();
        fH_TAPS_1g_IM->Fill(im);
        
        fH_Calib_TAPS_1g_IM->Fill(im, center);
        fH_Calib_TAPS_1g_IM2->Fill(im*im, center);
  
        // average photon energies of pion decay photons
        //if (im > 120 && im < 160) fH_TAPS_Pion_Avr_E->Fill(0.5 * (p4Gamma1.E() + p4Gamma2.E()));
        if (im > 120 && im < 160) fH_TAPS_Pion_Avr_E->Fill(energy);
        
        // average photon energies of eta decay photons
        //if (im > 520 && im < 630) fH_TAPS_Eta_Avr_E->Fill(0.5 * (p4Gamma1.E() + p4Gamma2.E()));
        if (im > 520 && im < 600) fH_TAPS_Eta_Avr_E->Fill(energy);
    }

    label_end_taps_calib:


    ////////////////////////////////////////////////////////////////////////////
    // TAPS 2 gamma invariant mass                                            //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fNNeutral == 2 && fPartNeutral[0]->GetDetector() == kTAPSDetector
                       && fPartNeutral[1]->GetDetector() == kTAPSDetector)
    {
        TLorentzVector p4Gamma1;
        TLorentzVector p4Gamma2;
        fPartNeutral[0]->Calculate4Vector(&p4Gamma1, 0);
        fPartNeutral[1]->Calculate4Vector(&p4Gamma2, 0);
        
        // invariant mass
        Double_t im = (p4Gamma1 + p4Gamma2).M();
        fH_TAPS_2g_IM->Fill(im);
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // TAPS 2 gamma time                                                      //
    ////////////////////////////////////////////////////////////////////////////
    
    // iterate over all cluster pairs hits
    for (UInt_t i = 0; i < fTAPSNCluster; i++)
    {
        // find center of first cluster
        Int_t center1 = fPartTAPS[i]->GetCentralElement();
        Double_t time1 = fPartTAPS[i]->GetTime();

        // skip cluster if veto fired
        if (fPartTAPS[i]->GetVetoEnergy() > 0) continue;

        for (UInt_t j = i+1; j < fTAPSNCluster; j++)
        {
            Int_t center2 = fPartTAPS[j]->GetCentralElement();
            Double_t time2 = fPartTAPS[j]->GetTime();
            
            // skip cluster if veto fired
            if (fPartTAPS[j]->GetVetoEnergy() > 0) continue;

            fH_Calib_TAPS_2g_Time->Fill(time1 - time2, center1);
            fH_Calib_TAPS_2g_Time->Fill(time2 - time1, center2);
        }
    }

 
}

