// SVN Info: $Id: TA2MyCalibration.cc 689 2010-11-18 16:21:43Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2007
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyCalibration                                                     //
//                                                                      //
// Calibration class for experiments at MAMI using the Crystal Ball     //
// and the TAPS detector.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyCalibration.h"

ClassImp(TA2MyCalibration)


//______________________________________________________________________________
TA2MyCalibration::TA2MyCalibration(const char* name, TA2Analysis* analysis) : TA2MyPhysics(name, analysis)
{
    // Constructor.

    // command-line recognition for SetConfig()
    AddCmdList(myCalibrationConfigKeys);

    // init variables
    fNCalib          = 0;
    fCB2gCalib       = 0;    
    fTAPS2gCalib     = 0;  
    fTAPS_PSACalib   = 0;
    fTAPS_LEDCalib   = 0;
    fTimeCalib       = 0;
    fPIDCalib        = 0;     
    fCBWalk          = 0;
    fTaggerTimeCalib = 0;
    fTAPSDebugOutput = 0;
    fClusterDebug    = 0;
    fVetoCalib       = 0;

    fCBP4 = 0;
    fTAPSP4 = 0;
}

//______________________________________________________________________________
TA2MyCalibration::~TA2MyCalibration()
{
    // Destructor.
    
    if (fCBP4) delete [] fCBP4;
    if (fTAPSP4) delete [] fTAPSP4;
}

//______________________________________________________________________________
void TA2MyCalibration::SetConfig(Char_t* line, Int_t key)
{
    // Read in analysis configuration parameters and configure the class. 
 
    Bool_t error = kFALSE;
    
    switch (key)
    {
        case EMC_Calib_CB_2GAMMA:
            // Enable CB 2 gamma energy calibration histograms
            if (sscanf(line, "%d", &fCB2gCalib) != 1) error = kTRUE;
            if (fCB2gCalib) fNCalib++;
            break;
        case EMC_Calib_TAPS_2GAMMA:
            // Enable TAPS 2 gamma energy calibration histograms
            if (sscanf(line, "%d", &fTAPS2gCalib) != 1) error = kTRUE;
            if (fTAPS2gCalib) fNCalib++;
            break;
        case EMC_Calib_TAPS_PSA:
            // Enable TAPS PSA calibration histograms
            if (sscanf(line, "%d", &fTAPS_PSACalib) != 1) error = kTRUE;
            if (fTAPS_PSACalib) fNCalib++;
            break;
        case EMC_Calib_TAPS_LED:
            // Enable TAPS LED calibration histograms
            if (sscanf(line, "%d", &fTAPS_LEDCalib) != 1) error = kTRUE;
            if (fTAPS_LEDCalib) fNCalib++;
            break;
        case EMC_Calib_TIME:
            // Enable time calibration histograms
            if (sscanf(line, "%d", &fTimeCalib) != 1) error = kTRUE;
            if (fTimeCalib) fNCalib++;
            break;
        case EMC_Calib_PID:
            // Enable PID calibration histograms
            if (sscanf(line, "%d", &fPIDCalib) != 1) error = kTRUE;
            if (fPIDCalib) fNCalib++;
            break;
        case EMC_Calib_CB_WALK:
            // Enable CB walk correction histograms
            if (sscanf(line, "%d", &fCBWalk) != 1) error = kTRUE;
            if (fCBWalk) fNCalib++;
            break;
        case EMC_Calib_TAGGER_TIME:
            // Enable tagger time calibration
            if (sscanf(line, "%d", &fTaggerTimeCalib) != 1) error = kTRUE;
            if (fTaggerTimeCalib) fNCalib++;
            break;
        case EMC_Calib_TAPS_DEBUG_OUTPUT:
            // Enable TAPS debug output
            if (sscanf(line, "%d", &fTAPSDebugOutput) != 1) error = kTRUE;
            if (fTAPSDebugOutput) fNCalib++;
            break;
         case EMC_Calib_CLUSTER_DEBUG:
            // Enable cluster debugging
            if (sscanf(line, "%d", &fClusterDebug) != 1) error = kTRUE;
            if (fClusterDebug) fNCalib++;
            break;
         case EMC_Calib_VETO:
            // Enable veto calibration
            if (sscanf(line, "%d", &fVetoCalib) != 1) error = kTRUE;
            if (fVetoCalib) fNCalib++;
            break;
        default:
            // default parent class SetConfig()
            TA2MyPhysics::SetConfig(line, key);
            break;
    }

    if (error) PrintError(line);
}

//______________________________________________________________________________ 
void TA2MyCalibration::PostInit()
{
    // Do some further initialisation after all setup parameters have been read in.
    
    Char_t name[256];

    // show information about the enabled calibration histogram creation
    printf("\n\n  ****** ENABLED CALIBRATION HISTOGRAMS ******\n\n");
    printf("   %d calibration(s) enabled.\n\n", fNCalib);
    if (fCB2gCalib)        printf("   - CB 2 gamma energy\n");
    if (fTAPS2gCalib)      printf("   - TAPS 2 gamma energy\n");   
    if (fTAPS_PSACalib)    printf("   - TAPS PSA\n");
    if (fTAPS_LEDCalib)    printf("   - TAPS LED\n"); 
    if (fTimeCalib)        printf("   - Time calibration\n");
    if (fPIDCalib)         printf("   - PID\n"); 
    if (fCBWalk)           printf("   - CB walk correction\n"); 
    if (fTaggerTimeCalib)  printf("   - Tagger time calibration\n");
    if (fTAPSDebugOutput)  printf("   - TAPS debug output\n");
    if (fClusterDebug)     printf("   - cluster debugging\n");
    if (fVetoCalib)        printf("   - veto calibration\n");
    printf("\n");

    // create needed histograms
    fH_Calib_BeamHel = new TH1F("Calib.BeamHel", "Calib.BeamHel", 10, -5, 5);
    fH_Calib_CB_PID = new TH2F("Calib.CB_PID", "Calib.CB_PID", 1000, 0, 1000, 400, 0, 20);
    fH_Calib_Total_E = new TH1F("Calib.CB_Total_E", "Calib.CB_Total_E", 2000, 0, 2000);
    fH_Calib_IM_2g = new TH1F("Calib.IM.2g", "Calib.IM.2g", 1000, 0, 1000);
    fH_Calib_IM_2g_2CB = new TH1F("Calib.IM.2g.2CB", "Calib.IM.2g.2CB", 1000, 0, 1000);
    fH_Calib_IM_2g_1TAPS = new TH1F("Calib.IM.2g.1TAPS", "Calib.IM.2g.1TAPS", 1000, 0, 1000);
    fH_Calib_IM_2g_2TAPS = new TH1F("Calib.IM.2g.2TAPS", "Calib.IM.2g.2TAPS", 1000, 0, 1000);
    fH_Calib_IM_2g_cut = new TH1F("Calib.IM.2g_cut", "Calib.IM.2g_cut", 1000, 0, 1000);
    fH_Calib_IM_2g_IM_Em = new TH2F("Calib.IM.2g.IM_Em", "Calib.IM.2g.IM_Em", 1000, 0, 1000, 1000, 0, 1000);
    fH_Calib_Charged_Theta = new TH1F("Calib.Charged.Theta", "Calib.Charged.Theta", 180, 0, 180);
    fH_Calib_NtotalClusters = new TH1F("Calib.NtotalClusters", "Calib.NtotalClusters", 50, 0, 50);
    fH_Calib_TAPS_Energy = new TH2F("Calib.TAPS.Energy", "Calib.TAPS.Energy", 2000, 0, 100, 384, 0, 384);
    fH_Calib_CB_Energy = new TH2F("Calib.CB.Energy", "Calib.CB.Energy", 2000, 0, 100, 720, 0, 720);
    fH_Calib_CB_ClSize_v_Energy = new TH2F("Calib.CB.ClSize_v_Energy", "Calib.CB.ClSize_v_Energy", 2000, 0, 1000, 100, 0, 100);
    fH_Calib_Tagger_Nhits = new TH1F("Calib.Tagger.Nhits", "Calib.Tagger.Nhits", 100, 0, 100);
    fH_Calib_Tagger_Hits = new TH1F("Calib.Tagger.Hits", "Calib.Tagger.Hits", 352, 0, 352);
    fH_Calib_Tagger_Energy = new TH1F("Calib.Tagger.Energy", "Calib.Tagger.Energy", 1500, 0, 1500);
    fH_Calib_Tagger_Time = new TH1F("Calib.Tagger.Time", "Calib.Tagger.Time", 2000, -1000, 1000);
    fH_Calib_HelTest = new TH1F("HelTest", "HelTest", 65536, 0, 65536);
    fH_Calib_TAPS_VetoCorrel =  new TH2F("Calib.TAPS.VetoCorrel", "Calib.TAPS.VetoCorrel", 438, 0, 438, 438, 0, 438);
    fH_Calib_CB_2g_pi0_Energy = new TH1F("Calib.CB.2g.pi0.Energy", "Calib.CB.2g.pi0.Energy", 1000, 0, 1000);
    fH_Calib_CB_2g_pi0_Angle = new TH1F("Calib.CB.2g.pi0.Angle", "Calib.CB.2g.pi0.Angle", 720, -360, 360);
    fH_Calib_CB_2g_eta_Energy = new TH1F("Calib.CB.2g.eta.Energy", "Calib.CB.2g.eta.Energy", 1000, 0, 1000);
    fH_Calib_CB_2g_eta_Angle = new TH1F("Calib.CB.2g.eta.Angle", "Calib.CB.2g.eta.Angle", 720, -360, 360);
 
    if (fClusterDebug)
    {
        fH_Calib_TAPS_SingleClusterPos = new TH2F("Calib.TAPS.SingleClusterPos", "Calib.TAPS.SingleClusterPos", 1200, -60, 60, 1200, -60, 60);
        fH_Calib_TAPS_ClusterPos = new TH2F("Calib.TAPS.ClusterPos", "Calib.TAPS.ClusterPos", 120, -60, 60, 120, -60, 60);
        fH_Calib_TAPS_ClPhi_v_ClTheta = new TH2F("Calib.TAPS.ClPhi_v_ClTheta", "Calib.TAPS.ClPhi_v_ClTheta", 120, -30, 30, 720, -180, 180);
        fH_Calib_CB_ClPhi_v_ClTheta = new TH2F("Calib.CB.ClPhi_v_ClTheta", "Calib.CB.ClPhi_v_ClTheta", 180, 0, 180, 360, -180, 180);
        fH_Calib_TAPS_TOF = new TH2F("Calib.TAPS.TOF", "Calib.TAPS.TOF", 2000, -20, 20, 1000, 0, 500);
        for (UInt_t i = 0; i < 384; i++)
        {
            sprintf(name, "Calib.TAPS.Walk_%d", i);
            fH_Calib_TAPS_Walk[i] = new TH2F(name, name, 500, 0, 500, 800, -200, 200);
        }
    }

    if (fTAPS_LEDCalib)
    {
        fH_Calib_CFD = new TH1F("Calib.CFD", "Calib.CFD", 384, 0, 384);
        fH_Calib_LED1 = new TH1F("Calib.LED1", "Calib.LED1", 384, 0, 384);
        fH_Calib_LED2 = new TH1F("Calib.LED2", "Calib.LED2", 384, 0, 384);
        for (UInt_t i = 0; i < 384; i++)
        {
            sprintf(name, "Calib.TAPS.LG_%d", i+1);
            fH_Calib_TAPS_LG[i] = new TH1F(name, name, 4096, 0, 4096);
            
            sprintf(name, "Calib.TAPS.LG_CFD_%d", i+1);
            fH_Calib_TAPS_LG_CFD[i] = new TH1F(name, name, 4096, 0, 4096);
            
            sprintf(name, "Calib.TAPS.LG_NOCFD_%d", i+1);
            fH_Calib_TAPS_LG_NOCFD[i] = new TH1F(name, name, 4096, 0, 4096);
            
            sprintf(name, "Calib.TAPS.LG_LED1_%d", i+1);
            fH_Calib_TAPS_LG_LED1[i] = new TH1F(name, name, 4096, 0, 4096);
            
            sprintf(name, "Calib.TAPS.LG_LED2_%d", i+1);
            fH_Calib_TAPS_LG_LED2[i] = new TH1F(name, name, 4096, 0, 4096);
        }
    }
    
    if (fCB2gCalib)
    {
        fH_Calib_CB_2g_IMS = new TH1F("Calib.CB.2g.IMS", "Calib.CB.2g.IMS", 1000, 0, 1000);
        fH_Calib_CB_2g_IM = new TH2F("Calib.CB.2g.IM", "Calib.CB.2g.IM", 2000, 0, 1000, 720, 0, 720);
        fH_Calib_CB_2g_IM2 = new TH2F("Calib.CB.2g.IM2", "Calib.CB.2g.IM2", 1250, 0, 500000, 720, 0, 720);
        fH_Calib_CB_2g_IMS_cut = new TH1F("Calib.CB.2g.IMS.cut", "Calib.CB.2g.IMS.cut", 1000, 0, 1000);
        fH_Calib_CB_2g_IM_cut = new TH2F("Calib.CB.2g.IM.cut", "Calib.CB.2g.IM.cut", 2000, 0, 1000, 720, 0, 720);
        fH_Calib_CB_2g_IM2_cut = new TH2F("Calib.CB.2g.IM2.cut", "Calib.CB.2g.IM2.cut", 1250, 0, 500000, 720, 0, 720);
        fH_Calib_CB_2g_Time = new TH2F("Calib.CB.2g.Time", "Calib.CB.2g.Time", 400, -100, 100, 720, 0, 720);
        fH_Calib_CB_2g_Time_cut = new TH2F("Calib.CB.2g.Time.cut", "Calib.CB.2g.Time.cut", 400, -100, 100, 720, 0, 720);
    }

    if (fTAPS2gCalib)
    {
        fH_Calib_TAPS_1g_IMS = new TH1F("Calib.TAPS.1g.IMS", "Calib.TAPS.1g.IMS", 1000, 0, 1000);
        fH_Calib_TAPS_1g_IM = new TH2F("Calib.TAPS.1g.IM", "Calib.TAPS.1g.IM", 2000, 0, 1000, 402, 0, 402);
        fH_Calib_TAPS_1g_IM2 = new TH2F("Calib.TAPS.1g.IM2", "Calib.TAPS.1g.IM2", 2500, 0, 500000, 402, 0, 402);
        fH_Calib_TAPS_2g_IMS_noV = new TH1F("Calib.TAPS.2g.IMS_noV", "Calib.TAPS.2g.IMS_noV", 1000, 0, 1000);
        fH_Calib_TAPS_2g_IMS = new TH1F("Calib.TAPS.2g.IMS", "Calib.TAPS.2g.IMS", 1000, 0, 1000);
        fH_Calib_TAPS_2g_IM = new TH2F("Calib.TAPS.2g.IM", "Calib.TAPS.2g.IM", 2000, 0, 1000, 402, 0, 402);
        fH_Calib_TAPS_2g_Time = new TH2F("Calib.TAPS.2g.Time", "Calib.TAPS.2g.Time", 1600, -20, 20, 402, 0, 402);
    }

    if (fTimeCalib)
    {
        fH_Calib_Time_Tagger = new TH2F("Calib.Time.Tagger", "Calib.Time.Tagger", 400, -200, 200, 352, 0, 352);
        fH_Calib_Time_TAPS_vs_Tagger_ClnoV = new TH2F("Calib.Time.TAPS_vs_Tagger_ClnoV", ",Calib.Time.TAPS_vs_Tagger_ClnoV", 800, -200, 200, 384, 0, 384);
        fH_Calib_Time_Tagger_vs_TAPS_ClnoV = new TH2F("Calib.Time.Tagger_vs_TAPS_ClnoV", "Calib.Time.Tagger_vs_TAPS_ClnoV", 800, -200, 200, 352, 0, 352);
        fH_Calib_Time_TAPS_vs_Tagger_Cl = new TH2F("Calib.Time.TAPS_vs_Tagger_Cl", "Calib.Time.TAPS_vs_Tagger_Cl", 800, -200, 200, 384, 0, 384);
        fH_Calib_Time_Tagger_vs_TAPS_Cl = new TH2F("Calib.Time.Tagger_vs_TAPS_Cl", "Calib.Time.Tagger_vs_TAPS_Cl", 800, -200, 200, 352, 0, 352);
        fH_Calib_Time_TAPS_vs_Tagger = new TH2F("Calib.Time.TAPS_vs_Tagger",  "Calib.Time.TAPS_vs_Tagger", 800, -200, 200, 384, 0, 384);
        fH_Calib_Time_Tagger_vs_TAPS = new TH2F("Calib.Time.Tagger_vs_TAPS", "Calib.Time.Tagger_vs_TAPS", 800, -200, 200, 352, 0, 352);
    }

    if (fTAPS_PSACalib)
    {
        for (UInt_t i = 0; i < 384; i++)
        {
            sprintf(name, "Calib.TAPS.PSAR_vs_PSAA_Veto_%d", i);
            fH_Calib_TAPS_PSAR_vs_PSAA_Veto[i] = new TH2F(name, name, 1800, 0, 180, 100, 0, 600);
         
            sprintf(name, "Calib.TAPS.PSAR_vs_PSAA_%d", i);
            fH_Calib_TAPS_PSAR_vs_PSAA[i] = new TH2F(name, name, 1800, 0, 180, 100, 0, 600);
        }
    }

    if (fPIDCalib)
    {
        fH_Calib_PID_ID_v_CBAz = new TH2F("Calib.PID.ID_v_CBAz", "Calib.PID.ID_v_CBAz", 720, -360, 360, 24, 0, 24);
        fH_Calib_PID_CB_Phi_Diff = new TH1F("Calib.PID.CB.Phi.Diff", "Calib.PID.CB.Phi.Diff", 720, -360, 360);
        fH_Calib_PID_Energy = new TH2F("Calib.PID.Energy", "Calib.PID.Energy", 1000, 0, 1000, 24, 0, 24);
        fH_Calib_PID_Time = new TH2F("Calib.PID.Time", "Calib.PID.Time", 2000, -1000, 1000, 24, 0, 24);
        fH_Calib_ClSize_v_dE  = new TH2F("Calib.PID.CLSize_v_dE", "Calib.PID.ClSize_v_dE", 1000, 0, 10, 100, 0, 100);
        for (UInt_t i = 0; i < 24; i++)
        {
            sprintf(name, "Calib.PID.dE_v_Th_%d", i);
            fH_Calib_PID_dE_v_Th[i] = new TH2F(name, name, 180, 0, 180, 1000, 0, 10);
        }
  
    }

    if (fCBWalk)
    {
        for (UInt_t i = 0; i < 720; i++)
        {
            sprintf(name, "Calib.CBWalk.Et_%d", i);
            fH_Calib_CBWalk_Et[i] = new TH2F(name, name, 200, 0, 400, 400, -200, 200);
        }
        fH_Calib_CBWalk_EtS = new TH2F("Calib.CBWalk.EtS", "Calib.CBWalk.EtS", 200, 0, 400, 400, -200, 200);
    }

    if (fTaggerTimeCalib)
    {
        fH_Calib_PbGlass_Time = new TH1F("Calib.PbGlass.Time", "Calib.PbGlass.Time", 20000, -100, 100);
        fH_Calib_Tagger_m_PbGlass_Sum = new TH1F("Calib.Tagger_m_PbGlass_Sum", "Calib.Tagger_m_PbGlass_Sum", 8000, -400, 400);
        fH_Calib_Tagger_p_PbGlass_Sum = new TH1F("Calib.Tagger_p_PbGlass_Sum", "Calib.Tagger_p_PbGlass_Sum", 8000, -400, 400);
        fH_Calib_Tagger_m_PbGlass = new TH2F("Calib.Tagger_m_PbGlass", "Calib.Tagger_m_PbGlass", 4000, -400, 400, 352, 0, 352);
        fH_Calib_Tagger_p_PbGlass = new TH2F("Calib.Tagger_p_PbGlass", "Calib.Tagger_p_PbGlass", 4000, -400, 400, 352, 0, 352);
    }

    if (fVetoCalib)
    {

        fH_TAPS_Veto_dE_E_tot = new TH2F("TAPS_Veto_dE_E_tot", "TAPS_Veto_dE_E_tot", 1000, 0, 1000, 500, 0, 10);
        for (UInt_t i = 0; i < 384; i++)
        {
            sprintf(name, "TAPS_Veto_dE_E_%d", i);
            fH_TAPS_Veto_dE_E[i] = new TH2F(name, name, 1000, 0, 1000, 500, 0, 10);
        }
    }
    
    // call PostInit of parent class
    TA2MyPhysics::PostInit();
    
    // create 4-vector arrays
    if (fCB && fNaI) fCBP4 = new TLorentzVector[fNaI->GetMaxCluster()];
    if (fTAPS && fBaF2PWO) fTAPSP4 = new TLorentzVector[fBaF2PWO->GetMaxCluster()];
}

//______________________________________________________________________________
void TA2MyCalibration::ReconstructPhysics()
{
    // Main analysis method that is called for every event.
 

    fH_Calib_HelTest->Fill(*fCP);

    // electron beam helicity
    fH_Calib_BeamHel->Fill(fEBeamHelState);
    
    // CB total energy
    Double_t cb_total_energy;
    if (fNaI) cb_total_energy = fNaI->GetTotalEnergy();
    else cb_total_energy = 0;
    fH_Calib_Total_E->Fill(cb_total_energy);
    
    TLorentzVector* photons[20];
    UInt_t nPhotons = 0;
    UInt_t nTAPS = 0;
    UInt_t nCB = 0;

    // total number of clusters
    fNTotClusters = fCBNCluster + fTAPSNCluster;
    fH_Calib_NtotalClusters->Fill(fNTotClusters);


    // CB hits
    for (UInt_t i = 0; i < fNaINhits; i++)
    {
        Int_t elem = fNaI->GetHits()[i];
        Double_t energy = fNaI->GetEnergy()[elem];

        fH_Calib_CB_Energy->Fill(energy, elem);
    }
 
    // TAPS hits
    for (UInt_t i = 0; i < fBaF2PWONhits; i++)
    {
        Int_t elem = fBaF2PWO->GetHits()[i];
        Double_t energy = fBaF2PWO->GetEnergy()[elem];

        fH_Calib_TAPS_Energy->Fill(energy, elem);
    }
       
    // Tagger hits   
    fH_Calib_Tagger_Nhits->Fill(fTaggerPhotonNhits);
    for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
    {
        Int_t tagg_element = fTaggerPhotonHits[i];
        Double_t tagg_energy = fTaggerPhotonEnergy[i];
        Double_t tagg_time = fTaggerPhotonTime[i];
        
        fH_Calib_Tagger_Hits->Fill(tagg_element);
        fH_Calib_Tagger_Energy->Fill(tagg_energy);
        fH_Calib_Tagger_Time->Fill(tagg_time);
    }        
   

    // CB particles
    for (UInt_t i = 0; i < fCBNCluster; i++)
    {
        Double_t theta = (fCBCluster[fCBClusterHit[i]])->GetTheta();
        SetP4(fCBP4[i], (fCBCluster[fCBClusterHit[i]])->GetEnergy(), 0., (fCBCluster[fCBClusterHit[i]])->GetMeanPosition());

        if (fCBPID_dE[i] == 0) 
        {
            photons[nPhotons++] = &fCBP4[i];
            nCB++;
        }
        else
        {
            if (fCBPID_dE[i] > 0.7) fH_Calib_Charged_Theta->Fill(theta);
        }

        // cluster size
        fH_Calib_CB_ClSize_v_Energy->Fill((fCBCluster[fCBClusterHit[i]])->GetEnergy(), (fCBCluster[fCBClusterHit[i]])->GetNhits());
    }
 
    // TAPS particles
    for (UInt_t i = 0; i < fTAPSNCluster; i++)
    {
        HitCluster_t* clTAPS       = fTAPSCluster[fTAPSClusterHit[i]];
        Double_t theta = (fTAPSCluster[fTAPSClusterHit[i]])->GetTheta();
        SetP4(fTAPSP4[i], clTAPS->GetEnergy(), 0., clTAPS->GetMeanPosition());
        
        if (CheckClusterVeto(clTAPS) == 0)
        {
            photons[nPhotons++] = &fTAPSP4[i];
            nTAPS++;
        }
        else
        {
            fH_Calib_Charged_Theta->Fill(theta);
        }

        // veto calibration
        if (fVetoCalib)
        {
            Int_t id = clTAPS->GetIndex();

            // check if veto of central element fired
            for (UInt_t j = 0; j < fVetoNhits; j++) 
            {    
                if (fVetoHits[j] == id)
                {
                    Int_t vetoMaxIndex;
                    Double_t vetoEnergy = CheckClusterVeto(clTAPS, &vetoMaxIndex);
                    fH_TAPS_Veto_dE_E_tot->Fill(clTAPS->GetEnergy(), vetoEnergy * 
                                                TMath::Abs(TMath::Cos(clTAPS->GetTheta() * TMath::DegToRad())));

                    fH_TAPS_Veto_dE_E[vetoMaxIndex]->Fill(clTAPS->GetEnergy(), vetoEnergy * 
                                                                      TMath::Abs(TMath::Cos(clTAPS->GetTheta() * TMath::DegToRad())));
                } // central veto fired
            } // veto hits loop
        }
    }
 
    if (nPhotons == 2) 
    {
        Double_t im = (*photons[0] + *photons[1]).M();
        fH_Calib_IM_2g->Fill(im);
        if (cb_total_energy > 300) fH_Calib_IM_2g_cut->Fill(im);
        
        if (nTAPS == 0) 
        {
            fH_Calib_IM_2g_IM_Em->Fill(im, photons[0]->E());
            fH_Calib_IM_2g_IM_Em->Fill(im, photons[0]->E());
            fH_Calib_IM_2g_2CB->Fill(im);
            
            // assume pion
            if (im > 110 && im < 160)
            {
                fH_Calib_CB_2g_pi0_Energy->Fill(photons[0]->E());   
                fH_Calib_CB_2g_pi0_Energy->Fill(photons[1]->E());   
                fH_Calib_CB_2g_pi0_Angle->Fill(TMath::RadToDeg()*photons[0]->Angle(photons[1]->Vect()));   
            }

            // assume eta
            if (im > 500 && im < 600)
            {
                fH_Calib_CB_2g_eta_Energy->Fill(photons[0]->E());   
                fH_Calib_CB_2g_eta_Energy->Fill(photons[1]->E());   
                fH_Calib_CB_2g_eta_Angle->Fill(TMath::RadToDeg()*photons[0]->Angle(photons[1]->Vect()));   
             }


        }
        if (nTAPS == 1) fH_Calib_IM_2g_1TAPS->Fill(im);
        if (nTAPS == 2) fH_Calib_IM_2g_2TAPS->Fill(im);
    }

    // check PID
    for (UInt_t i = 0; i < fCBNCluster; i++)
    {
        Double_t clusterEnergy = (fCBCluster[fCBClusterHit[i]])->GetEnergy();
        fH_Calib_CB_PID->Fill(clusterEnergy, fCBPID_dE[i]);
    }

    // ------------------------------ CB 2 gamma energy calibration histograms ------------------------------ 
    if (fCB2gCalib)
    {
        // iterate over all cluster pairs hits
        for (UInt_t i = 0; i < fCBNCluster; i++)
        {
            // find center of first cluster
            Int_t center1 = (fCBCluster[fCBClusterHit[i]])->GetIndex();
            Double_t time1 = (fCBCluster[fCBClusterHit[i]])->GetTime();

            for (UInt_t j = i+1; j < fCBNCluster; j++)
            {
                Int_t center2 = (fCBCluster[fCBClusterHit[j]])->GetIndex();
                Double_t time2 = (fCBCluster[fCBClusterHit[j]])->GetTime();
                
                TLorentzVector p4Meson = fCBP4[i] + fCBP4[j];
                Double_t invMass = p4Meson.M();
                
                // fill uncut histograms
                fH_Calib_CB_2g_IMS->Fill(invMass);
                fH_Calib_CB_2g_IM->Fill(invMass, center1);
                fH_Calib_CB_2g_IM->Fill(invMass, center2);
                fH_Calib_CB_2g_IM2->Fill(invMass*invMass, center1);
                fH_Calib_CB_2g_IM2->Fill(invMass*invMass, center2);
                fH_Calib_CB_2g_Time->Fill(time1 - time2, center1);
                fH_Calib_CB_2g_Time->Fill(time2 - time1, center2);

                // fill cut histograms
                if (fCBPID_dE[i] == 0 && fCBPID_dE[j] == 0 )
                {
                    fH_Calib_CB_2g_IMS_cut->Fill(invMass);
                    fH_Calib_CB_2g_IM_cut->Fill(invMass, center1);
                    fH_Calib_CB_2g_IM_cut->Fill(invMass, center2);
                    fH_Calib_CB_2g_IM2_cut->Fill(invMass*invMass, center1);
                    fH_Calib_CB_2g_IM2_cut->Fill(invMass*invMass, center2);
                    fH_Calib_CB_2g_Time_cut->Fill(time1 - time2, center1);
                    fH_Calib_CB_2g_Time_cut->Fill(time2 - time1, center2);
                }
            }
        }
    }

    

    // ------------------------------ TAPS 2 gamma energy calibration histograms ------------------------------ 
    if (fTAPS2gCalib)
    {
        Double_t clVeto1Energy;
        Double_t clVeto2Energy;
        
        // iterate over all cluster pairs hits
        for (UInt_t i = 0; i < fTAPSNCluster; i++)
        {
            // find center of first cluster
            Int_t center1 = (fTAPSCluster[fTAPSClusterHit[i]])->GetIndex();
            Double_t time1 = (fTAPSCluster[fTAPSClusterHit[i]])->GetTime();

            // skip cluster if veto fired
            clVeto1Energy = CheckClusterVeto(fTAPSCluster[fTAPSClusterHit[i]]);
            
            for (UInt_t j = i+1; j < fTAPSNCluster; j++)
            {
                Int_t center2 = (fTAPSCluster[fTAPSClusterHit[j]])->GetIndex();
                Double_t time2 = (fTAPSCluster[fTAPSClusterHit[j]])->GetTime();
                
                // skip cluster if veto fired
                clVeto2Energy = CheckClusterVeto(fTAPSCluster[fTAPSClusterHit[j]]);
                 
                TLorentzVector p4Meson = fTAPSP4[i] + fTAPSP4[j];
                Double_t invMass = p4Meson.M();

                if (clVeto1Energy == 0 && clVeto2Energy == 0)
                {
                    fH_Calib_TAPS_2g_IMS->Fill(invMass);
                    fH_Calib_TAPS_2g_IM->Fill(invMass, center1);
                    fH_Calib_TAPS_2g_IM->Fill(invMass, center2);
                    fH_Calib_TAPS_2g_Time->Fill(time1 - time2, center1);
                    fH_Calib_TAPS_2g_Time->Fill(time2 - time1, center2);
                }
                
                fH_Calib_TAPS_2g_IMS_noV->Fill(invMass);
            }
        }

        // 1 Cluster CB - 1 Cluster TAPS calibration
        if (fCBNCluster == 1 && fTAPSNCluster == 1)
        {
            if (CheckClusterVeto(fTAPSCluster[fTAPSClusterHit[0]]) == 0)
            {
                Int_t center = (fTAPSCluster[fTAPSClusterHit[0]])->GetIndex();
                TLorentzVector p4Meson = fCBP4[0] + fTAPSP4[0];
                Double_t invMass = p4Meson.M();
                
                fH_Calib_TAPS_1g_IMS->Fill(invMass);
                fH_Calib_TAPS_1g_IM->Fill(invMass, center);
                fH_Calib_TAPS_1g_IM2->Fill(invMass *  invMass, center);
            }
        }
    }

   
    // ------------------------------ Time calibration ------------------------------ 
    if (fTimeCalib)
    {
        // iterate over all tagger hits
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            Int_t tagg_element = fTaggerPhotonHits[i];
            Double_t tagg_time = fTaggerPhotonTime[i];
            
            // fill tagger time
            fH_Calib_Time_Tagger->Fill(tagg_time, tagg_element);

            // iterate over all TAPS clusters
            for (UInt_t j = 0; j < fTAPSNCluster; j++)
            {
                Int_t center = (fTAPSCluster[fTAPSClusterHit[j]])->GetIndex();
                Double_t time = (fTAPSCluster[fTAPSClusterHit[j]])->GetTime();
                fH_Calib_Time_TAPS_vs_Tagger_Cl->Fill(time + tagg_time, center);
                fH_Calib_Time_Tagger_vs_TAPS_Cl->Fill(time + tagg_time, tagg_element);
            
                if (CheckClusterVeto(fTAPSCluster[fTAPSClusterHit[j]]) == 0)
                {
                    fH_Calib_Time_TAPS_vs_Tagger_ClnoV->Fill(time + tagg_time, center);
                    fH_Calib_Time_Tagger_vs_TAPS_ClnoV->Fill(time + tagg_time, tagg_element);
                }
            }
 
            // iterate over all TAPS hits
            for (UInt_t j = 0; j < fBaF2PWONhits; j++)
            {
                Int_t elem = fBaF2PWOHits[j];
                Double_t time = fBaF2PWOTime[elem];
                
                fH_Calib_Time_TAPS_vs_Tagger->Fill(time + tagg_time, elem);
                fH_Calib_Time_Tagger_vs_TAPS->Fill(time + tagg_time, tagg_element);
            }
        }
    }

 
    // ---------------------------- TAPS PSA calibration histograms --------------------------- 
    if (fTAPS_PSACalib)
    {
        Double_t lgE = 0.;
        Double_t sgE = 0.;
        Double_t psaR, psaA;
        Int_t elemTAPS;

        // iterate over hit elements
        for (UInt_t i = 0; i < fBaF2PWONhits; i++)
        {
            // element ID
            elemTAPS = fBaF2PWOHits[i];
            
            // PSA
            lgE = fBaF2PWO->GetLGEnergy(elemTAPS);
            sgE = fBaF2PWO->GetSGEnergy(elemTAPS);
            
            psaR = TMath::Sqrt(sgE*sgE + lgE*lgE);
            psaA = TMath::ATan(sgE/lgE)*TMath::RadToDeg();

            // Veto
            Bool_t vetoFired = kFALSE;
            for (UInt_t j = 0; j < fVetoNhits; j++)
            {
                // check TAPS element
                if (fVetoHits[j] == elemTAPS) vetoFired = kTRUE;
            }

            fH_Calib_TAPS_PSAR_vs_PSAA[elemTAPS]->Fill(psaA, psaR);
            if (vetoFired) fH_Calib_TAPS_PSAR_vs_PSAA_Veto[elemTAPS]->Fill(psaA, psaR);
        }
        
        
        /*
        // iterate over all cluster pairs hits
        for (UInt_t i = 0; i < fTAPSNCluster; i++)
        {
            elemTAPS = (fTAPSCluster[fTAPSClusterHit[i]])->GetIndex();
            
            // PSA
            lgE = fBaF2->GetLGEnergy(elemTAPS);
            sgE = fBaF2->GetSGEnergy(elemTAPS);
            psaR = TMath::Sqrt(sgE*sgE + lgE*lgE);
            psaA = TMath::ATan(sgE/lgE)*TMath::RadToDeg();

            // Veto
            Bool_t vetoFired = kFALSE;
            for (UInt_t j = 0; j < fVetoNhits; j++)
            {
                // check TAPS element
                if (fVetoHits[j] == elemTAPS) vetoFired = kTRUE;
            }

            fHistManager->Fill2D("taps.psar_v_psaa", elemTAPS, psaA, psaR);
            if (vetoFired) fHistManager->Fill2D("taps.psar_v_psaa_veto", elemTAPS, psaA, psaR);
        }
        */

    }



    // ------------------------------ TAPS LED calibration histograms ------------------------------ 
    if (fTAPS_LEDCalib)
    {
        // discriminator pattern
        // CFD
        for (UInt_t i = 0; i < fBaF2CFDNhits; i++) fH_Calib_CFD->Fill(fBaF2CFDHits[i]);
    
        // LED1 
        for (UInt_t i = 0; i < fBaF2LED1Nhits; i++) fH_Calib_LED1->Fill(fBaF2LED1Hits[i]);
    
        // LED2 
        for (UInt_t i = 0; i < fBaF2LED2Nhits; i++) fH_Calib_LED2->Fill(fBaF2LED2Hits[i]);
        
        // fill raw uncut and LED1/2 cut ADC spectra
        if (fBaF2PWO->IsRawHits())
        {
            // loop over ADC hits
            for (UInt_t i = 0; i < fBaF2PWO->GetNADChits(); i++)
            {
                Bool_t cfd_fired = kFALSE;

                Int_t element_adc = fBaF2PWO->GetRawEnergyHits()[i];
                UShort_t value_adc = fBaF2PWO->GetElement(element_adc)->GetRawADCValue();
                
                // fill uncut ADC hits
                fH_Calib_TAPS_LG[element_adc]->Fill((Double_t)value_adc);
                
                // fill hits were CFD fired and didn't fired
                for (UInt_t j = 0; j < fBaF2CFDNhits; j++)
                {
                    if (fBaF2CFDHits[j] == element_adc) 
                    {
                        fH_Calib_TAPS_LG_CFD[element_adc]->Fill((Double_t)value_adc);
                        cfd_fired = kTRUE;
                    }
                }
                if (!cfd_fired) fH_Calib_TAPS_LG_NOCFD[element_adc]->Fill((Double_t)value_adc);

                // fill hits were LED1 fired
                for (UInt_t j = 0; j < fBaF2LED1Nhits; j++)
                {
                    if (fBaF2LED1Hits[j] == element_adc) fH_Calib_TAPS_LG_LED1[element_adc]->Fill((Double_t)value_adc);
                }

                // fill hits were LED2 fired
                for (UInt_t j = 0; j < fBaF2LED2Nhits; j++)
                {
                    if (fBaF2LED2Hits[j] == element_adc) fH_Calib_TAPS_LG_LED2[element_adc]->Fill((Double_t)value_adc);
                }
            }
        }
    }

 
    // ------------------------------ PID calibration histograms ------------------------------ 
    if (fPIDCalib)
    {
        // fill calibration histograms
        
        // iterate over all PID hits
        for (UInt_t i = 0; i < fPIDNhits; i++)
        {
            Int_t pid_element = fPIDHits[i];
            fH_Calib_PID_Energy->Fill(fPIDEnergy[pid_element], pid_element);
            fH_Calib_PID_Time->Fill(fPIDTime[pid_element], pid_element);
        }
       
        // cut on 1 cluster in CB and 1 hit in PID
        if (fCBNCluster == 1 && fPIDNhits == 1)
        {
            // PID element ID versus CB cluster azimuth angle
            fH_Calib_PID_ID_v_CBAz->Fill((fCBCluster[fCBClusterHit[0]])->GetPhi(), fPIDHits[0]);
            
            // PID dE versus CB cluster theta
            if ((fCBCluster[fCBClusterHit[0]])->GetNhits() >= 8)
            {
                fH_Calib_PID_dE_v_Th[fPIDHits[0]]->Fill((fCBCluster[fCBClusterHit[0]])->GetTheta(), fCBPID_dE[0]);
            }
            
            // CB cluster size versus PID dE
            fH_Calib_ClSize_v_dE->Fill(fCBPID_dE[0], (fCBCluster[fCBClusterHit[0]])->GetNhits());
        
            // PID-CB phi difference
            fH_Calib_PID_CB_Phi_Diff->Fill((fCBCluster[fCBClusterHit[0]])->GetPhi() - fPID->GetPosition( fPID->GetHits(0) )->Z());
        }
    }

    
    //
    //
    // Rewrite this with correct particle identification
    //
    //
    /*
    // ------------------------------ CB walk correction histograms ------------------------------ 
    if (fCBWalk)
    {
        Double_t* elemEnergy = fNaI->GetEnergy();
        Double_t* elemTime   = fNaI->GetTime();
        
        // select 3 clusters in CB and no cluster in TAPS
        if (fCBNCluster == 3 && fTAPSNCluster == 0)
        {   
            Int_t photons = 0;
            Int_t protons = 0;
            TLorentzVector p4photon[3];
            TLorentzVector p4proton;
            Int_t center[3];
            
            // get particles
            for (Int_t i = 0; i < 3; i++)
            {
                if (fCBPID_dE[i] == 0) 
                {
                    p4photon[photons] = fCBP4[i];
                    center[photons] = (fCBCluster[fCBClusterHit[i]])->GetIndex();
                    photons++;
                }
                if (fCBPDG_ID[i] == kProton)
                {
                    p4proton = fCBP4[i];
                    protons++;
                }
            }

            // select 2 photon + 1 proton events
            if (photons == 2 && protons == 1)
            {
                TLorentzVector p4pion = p4photon[0] + p4photon[1];
                Double_t invMass2g = p4pion.M();

                // cut on invariant mass to select pions
                if (invMass2g > 100 && invMass2g < 160) 
                {
                    // iterate over all tagger hits
                    for (UInt_t i = 0; i < fTaggerNhits; i++)
                    {
                        Int_t tagg_element = fTaggerHits[i];
                        Double_t tagg_time = fTaggerTime[tagg_element];

                        // Prompt Events
                        if (tagg_time > -7 && tagg_time < 7)
                        {
                            // photon 1
                            fHistManager->Fill2D("Calib.CBWalk.EtS", elemEnergy[center[0]], elemTime[center[0]] - tagg_time);
                            fHistManager->Fill2D("Calib.CBWalk.Et", center[0], elemEnergy[center[0]], elemTime[center[0]] - tagg_time);
                
                            // photon 2
                            fHistManager->Fill2D("Calib.CBWalk.EtS", elemEnergy[center[1]], elemTime[center[1]] - tagg_time);
                            fHistManager->Fill2D("Calib.CBWalk.Et", center[1], elemEnergy[center[1]], elemTime[center[1]] - tagg_time);
                        }
                    }
                }
            }
        }


    }
    */


 
    /*
     * Code to check walk correction values
     *
    for (Int_t i = 0; i < 720; i++)
    {
        HitD2A_t* elemCB = fNaI->GetElement(i);
        TimeWalk_t* tw = elemCB->GetWalk();
        if (tw)
        {
            printf("%d, %8f, %8f, %8f\n", i, elemCB->GetWalk()->GetParam1(),  elemCB->GetWalk()->GetParam2(), elemCB->GetWalk()->GetParam3());
        }
    }
    cout << endl << endl;
    */


    // ------------------------------ Tagger time calibration histograms ------------------------------ 
    if (fTaggerTimeCalib)
    {
        // iterate over all tagger hits
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            Int_t tagg_element = fTaggerPhotonHits[i];
            Double_t tagg_time = fTaggerPhotonTime[i];
        
            // fill tagger-Pb glass times
            if (fPbGlassNhits == 1)
            {
                Double_t pbglass_time = fPbGlassTime[0];
         
                fH_Calib_PbGlass_Time->Fill(pbglass_time);
                fH_Calib_Tagger_m_PbGlass_Sum->Fill(tagg_time - pbglass_time);
                fH_Calib_Tagger_p_PbGlass_Sum->Fill(tagg_time + pbglass_time);
                fH_Calib_Tagger_m_PbGlass->Fill(tagg_time - pbglass_time, tagg_element);
                fH_Calib_Tagger_p_PbGlass->Fill(tagg_time + pbglass_time, tagg_element);
            }
        }
    }


    // ------------------------------ TAPS Debug Output ------------------------------ 
    if (fTAPSDebugOutput)
    {
        if (fBaF2PWONhits > 0)
        {
            // iterate over hit elements
            printf("N_hits: %d\n", fBaF2PWONhits);
            printf("hits: ");
            for (UInt_t i = 0; i < fBaF2PWONhits; i++) printf("%d ", fBaF2PWOHits[i]);
            printf("\n");
 
            // iterate over clusters
            printf("N_clusters: %d\n", fTAPSNCluster);
            for (UInt_t i = 0; i < fTAPSNCluster; i++)
            {
                Int_t center = (fTAPSCluster[fTAPSClusterHit[i]])->GetIndex();
                UInt_t* hits = (fTAPSCluster[fTAPSClusterHit[i]])->GetHits();
                UInt_t nHits = (fTAPSCluster[fTAPSClusterHit[i]])->GetNhits();

                printf("Cluster_Config: %d %d\n", center, nHits);
                printf("Cluster_Elements: ");
                for (UInt_t j = 0; j < nHits; j++) printf("%d ", hits[j]);
                if (i != fTAPSNCluster-1) printf("\n");
            }
            printf("\n");
            
            // iterate over clusters
            printf("N_veto: %d\n", fVetoNhits);
            printf("veto_hits: ");
            for (UInt_t i = 0; i < fVetoNhits; i++) printf("%d ", fVetoHits[i]); 
            printf("\n");

            printf("\n");
        }
    }



    // ------------------------------ Cluster Debugging ------------------------------ 
    if (fClusterDebug)
    {
        Double_t tapsNeutTime[12];
        Double_t tapsCharEnergy[12];
        Double_t tapsCharTime[12];
        UInt_t tapsNCh = 0;
        UInt_t tapsNN = 0;

        // iterate over clusters
        for (UInt_t i = 0; i < fTAPSNCluster; i++)
        {
            Int_t center = (fTAPSCluster[fTAPSClusterHit[i]])->GetIndex();
            //UInt_t* hits = (fTAPSCluster[fTAPSClusterHit[i]])->GetHits();
            Double_t theta = (fTAPSCluster[fTAPSClusterHit[i]])->GetTheta();
            Double_t phi = (fTAPSCluster[fTAPSClusterHit[i]])->GetPhi();
            Double_t time = (fTAPSCluster[fTAPSClusterHit[i]])->GetTime();
            Double_t energy = (fTAPSCluster[fTAPSClusterHit[i]])->GetEnergy();
            Double_t centralFrac = (fTAPSCluster[fTAPSClusterHit[i]])->GetCentralFrac();
            
            Double_t x = (fTAPSCluster[fTAPSClusterHit[i]])->GetMeanPosition()->X();
            Double_t y = (fTAPSCluster[fTAPSClusterHit[i]])->GetMeanPosition()->Y();
            //Double_t z = (fTAPSCluster[fTAPSClusterHit[i]])->GetMeanPosition()->Z();

            UInt_t nHits = (fTAPSCluster[fTAPSClusterHit[i]])->GetNhits();
            
            if (nHits == 1 && center == 0) fH_Calib_TAPS_SingleClusterPos->Fill(x, y); 

            fH_Calib_TAPS_ClusterPos->Fill(x, y);
            fH_Calib_TAPS_ClPhi_v_ClTheta->Fill(theta, phi);
          
            // save taps hits
            if (CheckClusterVeto(fTAPSCluster[fTAPSClusterHit[i]]) > 0)
            {
                tapsCharTime[tapsNCh] = time;
                tapsCharEnergy[tapsNCh] = energy;
                tapsNCh++;
            }
            else
            {
                tapsNeutTime[tapsNN] = time;
                tapsNN++;

                // fill walk correction histograms
                if (centralFrac > 0.8) fH_Calib_TAPS_Walk[center]->Fill(energy*centralFrac, time);
            }
        }

        // iterate over CB clusters
        for (UInt_t i = 0; i < fCBNCluster; i++)
        {
            Double_t theta = (fCBCluster[fCBClusterHit[i]])->GetTheta();
            Double_t phi = (fCBCluster[fCBClusterHit[i]])->GetPhi();
            
            fH_Calib_CB_ClPhi_v_ClTheta->Fill(theta, phi);
        }


        //
        // TOF debugging
        //
        if (tapsNN > 0 && tapsNCh > 0)
        {
            Double_t neutralTime = 0.;

            // calculate neutral hit time
            for (UInt_t i = 0; i < tapsNN; i++) neutralTime += tapsNeutTime[i];
            neutralTime *= 1 / (Double_t)tapsNN;

            // fill charged hits to histo
            for (UInt_t i = 0; i < tapsNCh; i++) 
                fH_Calib_TAPS_TOF->Fill(neutralTime - tapsCharTime[i], tapsCharEnergy[i]);
        }
    
    }

    //
    // TAPS BaF2 Veto correlation
    //
    
    // loop over BaF2 hits
    for (UInt_t i = 0; i < fBaF2PWONhits; i++)
    {
        // loop over veto hits
        for (UInt_t j = 0; j < fVetoNhits; j++) 
        {   
            fH_Calib_TAPS_VetoCorrel->Fill(fBaF2PWOHits[i], fVetoHits[j]);
        }

    }



}

//______________________________________________________________________________ 
Bool_t TA2MyCalibration::IsInOuterTAPSRing(Int_t inElement)
{
    // Checks if the element inElement belongs to the outer TAPS ring.
    
    if (inElement % 64 >= 55) return true;
    else return false;
}

//______________________________________________________________________________ 
Bool_t TA2MyCalibration::IsInInnerTAPSRing(Int_t inElement)
{
    // Checks if the element inElement belongs to the inner TAPS ring.
    
    if (inElement % 64 == 0) return true;
    else return false;
}

