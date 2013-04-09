// SVN Info: $Id: TA2MyAnalysis.cc 761 2011-01-24 14:16:37Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2007-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyAnalysis                                                        //
//                                                                      //
// Analysis class for experiments at MAMI using the Crystal Ball and    //
// the TAPS detector.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyAnalysis.h"

ClassImp(TA2MyAnalysis)


//______________________________________________________________________________
TA2MyAnalysis::TA2MyAnalysis(const char* name, TA2Analysis* analysis) 
    : TA2MyPhysics(name, analysis)
{
    // Constructor.
    
    // command-line recognition for SetConfig()
    AddCmdList(myAnalysisConfigKeys);

    // init members
    fCreateEtaGridEff = kFALSE;
    fCreateEta_n_Eff = kFALSE;
    fCreateEta_p_Eff = kFALSE;
    fCreateComp_p_Eff = kFALSE;
    fCreateComp_n_Eff = kFALSE;
    
    // default number of cos(theta) bins
    f2g_fulinc_NCTBin   = 10;
    f2g_qfinc_NCTBin    = 10;
    f2g_fulexc_p_NCTBin = 10;
    f2g_fulexc_n_NCTBin = 10;
    //f6g_fulinc_NCTBin   = 10;
    //f6g_qfinc_NCTBin    = 10;
    //f6g_fulexc_p_NCTBin = 10;
    //f6g_fulexc_n_NCTBin = 10;
    fComp_p_NCTBin = 1;
    fComp_n_NCTBin = 1;

    // load 2g_fulinc detection efficiency
    TOLoader::LoadObject("/usr/panther_scratch1/werthm/A2/Dec_07/MC/eta2g/eta2g_grideff.root",
                         "Eff", &fH_2g_fulinc_DetEff, "2g_fulinc detection efficiency");

    // load 6g_fulinc detection efficiency
    TOLoader::LoadObject("/usr/panther_scratch1/werthm/A2/Dec_07/MC/eta6g/eta6g_grideff.root",
                         "Eff", &fH_6g_fulinc_DetEff, "6g_fulinc detection efficiency");
    
    // load the 2g_qfinc phase space
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qfinc_phase_space.root",
                         "PhaseSpace", &fH_2g_qfinc_PhaseSpace, "2g_qfinc phase space");

    // load the 2g_fulexc_p phase space
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qf_p_phase_space.root",
                         "PhaseSpace", &fH_2g_fulexc_p_PhaseSpace, "2g_fulexc_p phase space");

    // load the 2g_fulexc_n phase space
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qf_n_phase_space.root",
                         "PhaseSpace", &fH_2g_fulexc_n_PhaseSpace, "2g_fulexc_n phase space");
    
    // load the comp_p detection efficiency
    TOLoader::LoadObject("/usr/users/werthm/src/ROOT/macros/det_efficiency/comp_p_grideff.root",
                         "Eff", &fH_Comp_p_DetEff, "comp_p detection efficiency");

    // load the comp_n detection efficiency
    TOLoader::LoadObject("/usr/users/werthm/src/ROOT/macros/det_efficiency/comp_n_grideff.root",
                         "Eff", &fH_Comp_n_DetEff, "comp_n detection efficiency");

    // load the eta2g_qfinc missing mass cut
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qfinc_MM_cut.root",
                         "MM_Min", &fG_2g_qfinc_MM_Min, "eta2g_qfinc MM minimum value graph");
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qfinc_MM_cut.root",
                         "MM_Max", &fG_2g_qfinc_MM_Max, "eta2g_qfinc MM maximum value graph");

    // load the eta2g_qf_p missing mass cut
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qf_p_MM_cut.root",
                         "MM_Min", &fG_2g_fulexc_p_MM_Min, "eta2g_qf_p MM minimum value graph");
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qf_p_MM_cut.root",
                         "MM_Max", &fG_2g_fulexc_p_MM_Max, "eta2g_qf_p MM maximum value graph");

    // load the eta2g_qf_n missing mass cut
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qf_n_MM_cut.root",
                         "MM_Min", &fG_2g_fulexc_n_MM_Min, "eta2g_qf_n MM minimum value graph");
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta2g_qf_n_MM_cut.root",
                         "MM_Max", &fG_2g_fulexc_n_MM_Max, "eta2g_qf_n MM maximum value graph");
    
    // load the eta6g_qfinc missing mass cut
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta6g_qfinc_MM_cut.root",
                         "MM_Min", &fG_6g_qfinc_MM_Min, "eta6g_qfinc MM minimum value graph");
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta6g_qfinc_MM_cut.root",
                         "MM_Max", &fG_6g_qfinc_MM_Max, "eta6g_qfinc MM maximum value graph");

    // load the eta6g_qf_p missing mass cut
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta6g_qf_p_MM_cut.root",
                         "MM_Min", &fG_6g_fulexc_p_MM_Min, "eta6g_qf_p MM minimum value graph");
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta6g_qf_p_MM_cut.root",
                         "MM_Max", &fG_6g_fulexc_p_MM_Max, "eta6g_qf_p MM maximum value graph");

    // load the eta6g_qf_n missing mass cut
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta6g_qf_n_MM_cut.root",
                         "MM_Min", &fG_6g_fulexc_n_MM_Min, "eta6g_qf_n MM minimum value graph");
    TOLoader::LoadObject("/usr/users/werthm/AcquRoot/acqu/acqu/data/Dec_07/Cuts/eta6g_qf_n_MM_cut.root",
                         "MM_Max", &fG_6g_fulexc_n_MM_Max, "eta6g_qf_n MM maximum value graph");
    
    // load the comp_qf_p classifier
    //fMVA_Reader_Comp_p = new TMVA::Reader("!Color:!Silent");
    //fMVA_Reader_Comp_p->AddVariable("taggCh",      &fMVA_Var_1_Comp_p);
    //fMVA_Reader_Comp_p->AddVariable("ekinGamma",   &fMVA_Var_2_Comp_p);
    //fMVA_Reader_Comp_p->AddVariable("thetaGamma",  &fMVA_Var_3_Comp_p);
    //fMVA_Reader_Comp_p->AddVariable("ekinRecoil",  &fMVA_Var_4_Comp_p);
    //fMVA_Reader_Comp_p->AddVariable("thetaRecoil", &fMVA_Var_5_Comp_p);
    //fMVA_Reader_Comp_p->BookMVA("FisherG", "/usr/users/werthm/src/ROOT/macros/analysis/cuts/mva/weights/comp_qf_p_FisherG.weights.xml");
    
    // load the comp_qf_n classifier
    //fMVA_Reader_Comp_n = new TMVA::Reader("!Color:!Silent");
    //fMVA_Reader_Comp_n->AddVariable("taggCh",      &fMVA_Var_1_Comp_n);
    //fMVA_Reader_Comp_n->AddVariable("ekinGamma",   &fMVA_Var_2_Comp_n);
    //fMVA_Reader_Comp_n->AddVariable("thetaGamma",  &fMVA_Var_3_Comp_n);
    //fMVA_Reader_Comp_n->AddVariable("ekinRecoil",  &fMVA_Var_4_Comp_n);
    //fMVA_Reader_Comp_n->AddVariable("thetaRecoil", &fMVA_Var_5_Comp_n);
    //fMVA_Reader_Comp_n->BookMVA("FisherG", "/usr/users/werthm/src/ROOT/macros/analysis/cuts/mva/weights/comp_qf_n_FisherG.weights.xml");
}

//______________________________________________________________________________
TA2MyAnalysis::~TA2MyAnalysis()
{
    // Destructor.

}

//______________________________________________________________________________
void TA2MyAnalysis::SetConfig(Char_t* line, Int_t key)
{
    // Read in analysis configuration parameters and configure the class. 

    switch (key)
    {
        case EMA_CREATE_ETA_GRID_EFF:
        {
            // activate eta grid efficiency determination
            fCreateEtaGridEff = kTRUE;
            
            Info("SetConfig", "Switching on the eta grid efficiency determination");
            break;
        }
        case EMA_CREATE_ETA_P_EFF:
        {
            // activate quasi-free eta proton efficiency determination
            fCreateEta_p_Eff = kTRUE;
            
            Info("SetConfig", "Switching on the quasi-free eta proton efficiency determination");
            break;
        }
        case EMA_CREATE_ETA_N_EFF:
        {
            // activate quasi-free eta neutron efficiency determination
            fCreateEta_n_Eff = kTRUE;
            
            Info("SetConfig", "Switching on the quasi-free eta neutron efficiency determination");
            break;
        }
        case EMA_CREATE_COMP_P_EFF:
        {
            // activate quasi-free compton proton efficiency determination
            fCreateComp_p_Eff = kTRUE;
            
            Info("SetConfig", "Switching on the quasi-free Compton proton efficiency determination");
            break;
        }
        case EMA_CREATE_COMP_N_EFF:
        {
            // activate quasi-free compton neutron efficiency determination
            fCreateComp_n_Eff = kTRUE;
            
            Info("SetConfig", "Switching on the quasi-free Compton neutron efficiency determination");
            break;
        }
        case EMA_N_CT_BINS:
        {
            // read number of cos(theta) bins for the different channels
            if (sscanf(line, "%d%d%d%d", &f2g_fulinc_NCTBin, 
                                         &f2g_qfinc_NCTBin, 
                                         &f2g_fulexc_p_NCTBin, 
                                         &f2g_fulexc_n_NCTBin) == 4)
            {
                Info("SetConfig", "Number of cos(theta) bins: 2g_fulinc: %d, 2g_qfinc: %d, 2g_fulexc_p: %d, 2g_fulexc_n: %d",
                                   f2g_fulinc_NCTBin, f2g_qfinc_NCTBin, f2g_fulexc_p_NCTBin, f2g_fulexc_n_NCTBin);
            }
            else
            {
                Error("SetConfig", "Could not read correctly the number of cos(theta) bins!");
            }
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
void TA2MyAnalysis::PostInit()
{
    // Do some further initialisation after all setup parameters have been read in.
    
    // call PostInit of parent class
    TA2MyPhysics::PostInit();
    
    ////////////////////////////////////////////////////////////////////////////
    // Create histograms                                                      //
    ////////////////////////////////////////////////////////////////////////////
 
    Char_t name[256];
    
    Int_t bins[5]    = { 272,   80,  45,   80,  45 };
    Double_t xmin[5] = {   0,    0,   0,    0,   0 };
    Double_t xmax[5] = { 272, 1600, 180, 1600, 180 };
  
    
    //
    // fully inclusive eta -> 2g
    //
    fH_2g_fulinc_Photon_TOF_TAPS    = new TH2F("2g_fulinc_Photon_TOF_TAPS",    "2g_fulinc_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_2g_fulinc_Photon_TOF_CB      = new TH2F("2g_fulinc_Photon_TOF_CB",      "2g_fulinc_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    
    fH_2g_fulinc_Photon_PSA         = new TH2F("2g_fulinc_Photon_PSA",         "2g_fulinc_Photon_PSA",         600, 0, 60, 600, 0, 600);
    
    fH_2g_fulinc_Eta_E_Theta        = new TH2F("2g_fulinc_Eta_E_Theta",        "2g_fulinc_Eta_E_Theta",        600, 0, 1200, 90, 0, 180);
    fH_2g_fulinc_Eta_E_Theta_Gen    = new TH2F("2g_fulinc_Eta_E_Theta_Gen",    "2g_fulinc_Eta_E_Theta_Gen",    600, 0, 1200, 90, 0, 180);
    
    fH_2g_fulinc_Coinc_Time_CB      = new TH1F("2g_fulinc_Coinc_Time_CB",      "2g_fulinc_Coinc_Time_CB",      1600, -400, 400);
    fH_2g_fulinc_Coinc_Time_CB_TAPS = new TH1F("2g_fulinc_Coinc_Time_CB_TAPS", "2g_fulinc_Coinc_Time_CB_TAPS", 1600, -400, 400);
    fH_2g_fulinc_Coinc_Time_TAPS    = new TH1F("2g_fulinc_Coinc_Time_TAPS",    "2g_fulinc_Coinc_Time_TAPS",    2000, -100, 100);
    fH_2g_fulinc_Rand_Time_CB       = new TH1F("2g_fulinc_Rand_Time_CB",       "2g_fulinc_Rand_Time_CB",       12000, -800, 400);
    fH_2g_fulinc_Rand_Time_TAPS     = new TH1F("2g_fulinc_Rand_Time_TAPS",     "2g_fulinc_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_2g_fulinc_IM_Tot             = new TH2F("2g_fulinc_IM_Tot",             "2g_fulinc_IM_Tot",             500, 0, 1000, 272, 0, 272);
    fH_2g_fulinc_IM_E               = new TH2F("2g_fulinc_IM_E",               "2g_fulinc_IM_E",               500, 0, 1000, 500, 0, 1000);
    fH_2g_fulinc_MM_Tot             = new TH2F("2g_fulinc_MM_Tot",             "2g_fulinc_MM_Tot",             1000, -1000, 1000, 272, 0, 272);
    fH_2g_fulinc_MM_Tot->Sumw2();

    fH_2g_fulinc_IM = new TH2F*[f2g_fulinc_NCTBin];
    fH_2g_fulinc_E = new TH2F*[f2g_fulinc_NCTBin];
    fH_2g_fulinc_ExFunc = new TH1F*[f2g_fulinc_NCTBin];
    for (Int_t i = 0; i < f2g_fulinc_NCTBin; i++)
    {
        sprintf(name, "2g_fulinc_IM_%d", i);
        fH_2g_fulinc_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_2g_fulinc_IM[i]->Sumw2();
        sprintf(name, "2g_fulinc_E_%d", i);
        fH_2g_fulinc_E[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        sprintf(name, "2g_fulinc_ExFunc_%d", i);
        fH_2g_fulinc_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_2g_fulinc_ExFunc[i]->Sumw2();
    }
    
    
    //
    // inclusive quasi-free g + X -> [eta->2g] + X
    //
    fH_2g_qfinc_Photon_TOF_TAPS    = new TH2F("2g_qfinc_Photon_TOF_TAPS",    "2g_qfinc_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_2g_qfinc_Photon_TOF_CB      = new TH2F("2g_qfinc_Photon_TOF_CB",      "2g_qfinc_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    
    fH_2g_qfinc_Photon_PSA         = new TH2F("2g_qfinc_Photon_PSA",         "2g_qfinc_Photon_PSA",         600, 0, 60, 600, 0, 600);
    
    fH_2g_qfinc_Coinc_Time_CB      = new TH1F("2g_qfinc_Coinc_Time_CB",      "2g_qfinc_Coinc_Time_CB",      1600, -400, 400);
    fH_2g_qfinc_Coinc_Time_CB_TAPS = new TH1F("2g_qfinc_Coinc_Time_CB_TAPS", "2g_qfinc_Coinc_Time_CB_TAPS", 1600, -400, 400);
    fH_2g_qfinc_Coinc_Time_TAPS    = new TH1F("2g_qfinc_Coinc_Time_TAPS",    "2g_qfinc_Coinc_Time_TAPS",    2000, -100, 100);
    fH_2g_qfinc_Rand_Time_CB       = new TH1F("2g_qfinc_Rand_Time_CB",       "2g_qfinc_Rand_Time_CB",       12000, -800, 400);
    fH_2g_qfinc_Rand_Time_TAPS     = new TH1F("2g_qfinc_Rand_Time_TAPS",     "2g_qfinc_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_2g_qfinc_IM_Tot             = new TH2F("2g_qfinc_IM_Tot",             "2g_qfinc_IM_Tot",              500, 0, 1000, 272, 0, 272);
    fH_2g_qfinc_MM_Tot             = new TH2F("2g_qfinc_MM_Tot",             "2g_qfinc_MM_Tot",              1000, -1000, 1000, 272, 0, 272);
    fH_2g_qfinc_MM_Tot->Sumw2();

    fH_2g_qfinc_IM = new TH2F*[f2g_qfinc_NCTBin];
    fH_2g_qfinc_ExFunc = new TH1F*[f2g_qfinc_NCTBin];
    if (fCreateEta_p_Eff || fCreateEta_n_Eff)
    {
        fH_2g_qfinc_ExFunc_Gen = new TH1F*[f2g_qfinc_NCTBin];
    }
    for (Int_t i = 0; i < f2g_qfinc_NCTBin; i++)
    {
        sprintf(name, "2g_qfinc_IM_%d", i);
        fH_2g_qfinc_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_2g_qfinc_IM[i]->Sumw2();
        sprintf(name, "2g_qfinc_ExFunc_%d", i);
        fH_2g_qfinc_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_2g_qfinc_ExFunc[i]->Sumw2();
        if (fCreateEta_p_Eff || fCreateEta_n_Eff)
        {
            sprintf(name, "2g_qfinc_ExFunc_Gen_%d", i);
            fH_2g_qfinc_ExFunc_Gen[i] = new TH1F(name, name, 272, 0, 272);
        }
    }
    
    fH_2g_qfinc_Det = new THnSparseF("2g_qfinc_Det", "2g_qfinc_Det", 5, bins, xmin, xmax);
    fH_2g_qfinc_Det->Sumw2();
    gDirectory->Add(fH_2g_qfinc_Det);


    //
    // fully exclusive quasi-free g + p -> [eta->2g] + p
    //
    fH_2g_fulexc_p_Photon_TOF_TAPS    = new TH2F("2g_fulexc_p_Photon_TOF_TAPS",    "2g_fulexc_p_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_2g_fulexc_p_Proton_TOF_TAPS    = new TH2F("2g_fulexc_p_Proton_TOF_TAPS",    "2g_fulexc_p_Proton_TOF_TAPS",    1000, -50, 50, 500, 0, 500);
    
    fH_2g_fulexc_p_Photon_TOF_CB      = new TH2F("2g_fulexc_p_Photon_TOF_CB",      "2g_fulexc_p_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    fH_2g_fulexc_p_Proton_TOF_CB      = new TH2F("2g_fulexc_p_Proton_TOF_CB",      "2g_fulexc_p_Proton_TOF_CB",      1000, -50, 50, 500, 0, 500);
    
    fH_2g_fulexc_p_Photon_PSA         = new TH2F("2g_fulexc_p_Photon_PSA",         "2g_fulexc_p_Photon_PSA",         600, 0, 60, 600, 0, 600);
    fH_2g_fulexc_p_Proton_PSA         = new TH2F("2g_fulexc_p_Proton_PSA",         "2g_fulexc_p_Proton_PSA",         600, 0, 60, 600, 0, 600);
    
    fH_2g_fulexc_p_Proton_dE_E_CB     = new TH2F("2g_fulexc_p_Proton_dE_E_CB",     "2g_fulexc_p_Proton_dE_E_CB",     1000, 0, 1000, 400, 0, 10);
    fH_2g_fulexc_p_Proton_dE_E_TAPS   = new TH2F("2g_fulexc_p_Proton_dE_E_TAPS",   "2g_fulexc_p_Proton_dE_E_TAPS",   1000, 0, 1000, 400, 0, 10);
    
    fH_2g_fulexc_p_PhiDiff_Eta_Recoil = new TH1F("2g_fulexc_p_PhiDiff_Eta_Recoil", "2g_fulexc_p_PhiDiff_Eta_Recoil", 360, 0, 360);
    fH_2g_fulexc_p_Recoil_Fermi       = new TH1F("2g_fulexc_p_Recoil_Fermi",       "2g_fulexc_p_Recoil_Fermi",       1000, 0, 1000);
   
    fH_2g_fulexc_p_Coinc_Time_CB      = new TH1F("2g_fulexc_p_Coinc_Time_CB",      "2g_fulexc_p_Coinc_Time_CB",      1600, -400, 400);
    fH_2g_fulexc_p_Coinc_Time_CB_TAPS = new TH1F("2g_fulexc_p_Coinc_Time_CB_TAPS", "2g_fulexc_p_Coinc_Time_CB_TAPS", 1600, -400, 400);
    fH_2g_fulexc_p_Coinc_Time_TAPS    = new TH1F("2g_fulexc_p_Coinc_Time_TAPS",    "2g_fulexc_p_Coinc_Time_TAPS",    2000, -100, 100);
    fH_2g_fulexc_p_Rand_Time_CB       = new TH1F("2g_fulexc_p_Rand_Time_CB",       "2g_fulexc_p_Rand_Time_CB",       12000, -800, 400);
    fH_2g_fulexc_p_Rand_Time_TAPS     = new TH1F("2g_fulexc_p_Rand_Time_TAPS",     "2g_fulexc_p_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_2g_fulexc_p_IM_Tot             = new TH2F("2g_fulexc_p_IM_Tot",             "2g_fulexc_p_IM_Tot",             500, 0, 1000, 272, 0, 272);
    fH_2g_fulexc_p_MM_Tot             = new TH2F("2g_fulexc_p_MM_Tot",             "2g_fulexc_p_MM_Tot",             1000, -1000, 1000, 272, 0, 272);
    fH_2g_fulexc_p_MM_Tot->Sumw2();

    fH_2g_fulexc_p_IM = new TH2F*[f2g_fulexc_p_NCTBin];
    fH_2g_fulexc_p_ExFunc = new TH1F*[f2g_fulexc_p_NCTBin];
    fH_2g_fulexc_p_IM_W = new TH2F*[f2g_fulexc_p_NCTBin];
    fH_2g_fulexc_p_ExFunc_W = new TH2F*[f2g_fulexc_p_NCTBin];
    fH_2g_fulexc_p_IM_W_TOF = new TH2F*[f2g_fulexc_p_NCTBin];
    fH_2g_fulexc_p_ExFunc_W_TOF = new TH2F*[f2g_fulexc_p_NCTBin];
    if (fCreateEta_p_Eff)
    {
        fH_2g_fulexc_p_ExFunc_Gen = new TH1F*[f2g_fulexc_p_NCTBin];
        fH_2g_fulexc_p_ExFunc_W_Gen = new TH2F*[f2g_fulexc_p_NCTBin];
        fH_2g_fulexc_p_ExFunc_W_TOF_Gen = new TH2F*[f2g_fulexc_p_NCTBin];
    }
    for (Int_t i = 0; i < f2g_fulexc_p_NCTBin; i++)
    {
        sprintf(name, "2g_fulexc_p_IM_%d", i);
        fH_2g_fulexc_p_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_2g_fulexc_p_IM[i]->Sumw2();
        sprintf(name, "2g_fulexc_p_ExFunc_%d", i);
        fH_2g_fulexc_p_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_2g_fulexc_p_ExFunc[i]->Sumw2();
        sprintf(name, "2g_fulexc_p_IM_W_%d", i);
        fH_2g_fulexc_p_IM_W[i] = new TH2F(name, name, 500, 0, 1000, 1000, 1000, 3000);
        fH_2g_fulexc_p_IM_W[i]->Sumw2();
        sprintf(name, "2g_fulexc_p_ExFunc_W_%d", i);
        fH_2g_fulexc_p_ExFunc_W[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_2g_fulexc_p_ExFunc_W[i]->Sumw2();
        sprintf(name, "2g_fulexc_p_IM_W_TOF_%d", i);
        fH_2g_fulexc_p_IM_W_TOF[i] = new TH2F(name, name, 500, 0, 1000, 1000, 1000, 3000);
        fH_2g_fulexc_p_IM_W_TOF[i]->Sumw2();
        sprintf(name, "2g_fulexc_p_ExFunc_W_TOF_%d", i);
        fH_2g_fulexc_p_ExFunc_W_TOF[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_2g_fulexc_p_ExFunc_W_TOF[i]->Sumw2();
        if (fCreateEta_p_Eff)
        {
            sprintf(name, "2g_fulexc_p_ExFunc_Gen_%d", i);
            fH_2g_fulexc_p_ExFunc_Gen[i] = new TH1F(name, name, 272, 0, 272);
            sprintf(name, "2g_fulexc_p_ExFunc_W_Gen_%d", i);
            fH_2g_fulexc_p_ExFunc_W_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
            sprintf(name, "2g_fulexc_p_ExFunc_W_TOF_Gen_%d", i);
            fH_2g_fulexc_p_ExFunc_W_TOF_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        }
    }

    fH_2g_fulexc_p_Det = new THnSparseF("2g_fulexc_p_Det", "2g_fulexc_p_Det", 5, bins, xmin, xmax);
    fH_2g_fulexc_p_Det->Sumw2();
    gDirectory->Add(fH_2g_fulexc_p_Det);


    //
    // fully exclusive quasi-free g + n -> [eta->2g] + n
    //
    fH_2g_fulexc_n_Photon_TOF_TAPS    = new TH2F("2g_fulexc_n_Photon_TOF_TAPS",    "2g_fulexc_n_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_2g_fulexc_n_Neutron_TOF_TAPS   = new TH2F("2g_fulexc_n_Neutron_TOF_TAPS",   "2g_fulexc_n_Neutron_TOF_TAPS",   1000, -50, 50, 500, 0, 500);
    
    fH_2g_fulexc_n_Photon_TOF_CB      = new TH2F("2g_fulexc_n_Photon_TOF_CB",      "2g_fulexc_n_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    fH_2g_fulexc_n_Neutron_TOF_CB     = new TH2F("2g_fulexc_n_Neutron_TOF_CB",     "2g_fulexc_n_Neutron_TOF_CB",     1000, -50, 50, 500, 0, 500);
    
    fH_2g_fulexc_n_Photon_PSA         = new TH2F("2g_fulexc_n_Photon_PSA",         "2g_fulexc_n_Photon_PSA",         600, 0, 60, 600, 0, 600);
    fH_2g_fulexc_n_Neutron_PSA        = new TH2F("2g_fulexc_n_Neutron_PSA",        "2g_fulexc_n_Neutron_PSA",        600, 0, 60, 600, 0, 600);
    
    fH_2g_fulexc_n_PhiDiff_Eta_Recoil = new TH1F("2g_fulexc_n_PhiDiff_Eta_Recoil", "2g_fulexc_n_PhiDiff_Eta_Recoil", 360, 0, 360);
    fH_2g_fulexc_n_Recoil_Fermi       = new TH1F("2g_fulexc_n_Recoil_Fermi",       "2g_fulexc_n_Recoil_Fermi",       1000, 0, 1000);
    
    fH_2g_fulexc_n_Coinc_Time_CB      = new TH1F("2g_fulexc_n_Coinc_Time_CB",      "2g_fulexc_n_Coinc_Time_CB",      1600, -400, 400);
    fH_2g_fulexc_n_Coinc_Time_CB_TAPS = new TH1F("2g_fulexc_n_Coinc_Time_CB_TAPS", "2g_fulexc_n_Coinc_Time_CB_TAPS", 1600, -400, 400);
    fH_2g_fulexc_n_Coinc_Time_TAPS    = new TH1F("2g_fulexc_n_Coinc_Time_TAPS",    "2g_fulexc_n_Coinc_Time_TAPS",    2000, -100, 100);
    fH_2g_fulexc_n_Rand_Time_CB       = new TH1F("2g_fulexc_n_Rand_Time_CB",       "2g_fulexc_n_Rand_Time_CB",       12000, -800, 400);
    fH_2g_fulexc_n_Rand_Time_TAPS     = new TH1F("2g_fulexc_n_Rand_Time_TAPS",     "2g_fulexc_n_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_2g_fulexc_n_IM_Tot             = new TH2F("2g_fulexc_n_IM_Tot",             "2g_fulexc_n_IM_Tot",             500, 0, 1000, 272, 0, 272);
    fH_2g_fulexc_n_MM_Tot             = new TH2F("2g_fulexc_n_MM_Tot",             "2g_fulexc_n_MM_Tot",             1000, -1000, 1000, 272, 0, 272);
    fH_2g_fulexc_n_MM_Tot->Sumw2();

    fH_2g_fulexc_n_IM = new TH2F*[f2g_fulexc_n_NCTBin];
    fH_2g_fulexc_n_ExFunc = new TH1F*[f2g_fulexc_n_NCTBin];
    fH_2g_fulexc_n_IM_W = new TH2F*[f2g_fulexc_n_NCTBin];
    fH_2g_fulexc_n_ExFunc_W = new TH2F*[f2g_fulexc_n_NCTBin];
    fH_2g_fulexc_n_IM_W_TOF = new TH2F*[f2g_fulexc_n_NCTBin];
    fH_2g_fulexc_n_ExFunc_W_TOF = new TH2F*[f2g_fulexc_n_NCTBin];
    if (fCreateEta_n_Eff)
    {
        fH_2g_fulexc_n_ExFunc_Gen = new TH1F*[f2g_fulexc_n_NCTBin];
        fH_2g_fulexc_n_ExFunc_W_Gen = new TH2F*[f2g_fulexc_n_NCTBin];
        fH_2g_fulexc_n_ExFunc_W_TOF_Gen = new TH2F*[f2g_fulexc_n_NCTBin];
    }
    for (Int_t i = 0; i < f2g_fulexc_n_NCTBin; i++)
    {
        sprintf(name, "2g_fulexc_n_IM_%d", i);
        fH_2g_fulexc_n_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_2g_fulexc_n_IM[i]->Sumw2();
        sprintf(name, "2g_fulexc_n_ExFunc_%d", i);
        fH_2g_fulexc_n_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_2g_fulexc_n_ExFunc[i]->Sumw2();
        sprintf(name, "2g_fulexc_n_IM_W_%d", i);
        fH_2g_fulexc_n_IM_W[i] = new TH2F(name, name, 500, 0, 1000, 1000, 1000, 3000);
        fH_2g_fulexc_n_IM_W[i]->Sumw2();
        sprintf(name, "2g_fulexc_n_ExFunc_W_%d", i);
        fH_2g_fulexc_n_ExFunc_W[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_2g_fulexc_n_ExFunc_W[i]->Sumw2();
        sprintf(name, "2g_fulexc_n_IM_W_TOF_%d", i);
        fH_2g_fulexc_n_IM_W_TOF[i] = new TH2F(name, name, 500, 0, 1000, 1000, 1000, 3000);
        fH_2g_fulexc_n_IM_W_TOF[i]->Sumw2();
        sprintf(name, "2g_fulexc_n_ExFunc_W_TOF_%d", i);
        fH_2g_fulexc_n_ExFunc_W_TOF[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_2g_fulexc_n_ExFunc_W_TOF[i]->Sumw2();
        if (fCreateEta_n_Eff)
        {
            sprintf(name, "2g_fulexc_n_ExFunc_Gen_%d", i);
            fH_2g_fulexc_n_ExFunc_Gen[i] = new TH1F(name, name, 272, 0, 272);
            sprintf(name, "2g_fulexc_n_ExFunc_W_Gen_%d", i);
            fH_2g_fulexc_n_ExFunc_W_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
            sprintf(name, "2g_fulexc_n_ExFunc_W_TOF_Gen_%d", i);
            fH_2g_fulexc_n_ExFunc_W_TOF_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        }
    }
     
    fH_2g_fulexc_n_Det = new THnSparseF("2g_fulexc_n_Det", "2g_fulexc_n_Det", 5, bins, xmin, xmax);
    fH_2g_fulexc_n_Det->Sumw2();
    gDirectory->Add(fH_2g_fulexc_n_Det);
    
    /*
    //
    // fully inclusive eta -> 6g
    //
    fH_6g_fulinc_Photon_TOF_TAPS    = new TH2F("6g_fulinc_Photon_TOF_TAPS",    "6g_fulinc_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_6g_fulinc_Photon_TOF_CB      = new TH2F("6g_fulinc_Photon_TOF_CB",      "6g_fulinc_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
  
    fH_6g_fulinc_Photon_PSA         = new TH2F("6g_fulinc_Photon_PSA",         "6g_fulinc_Photon_PSA",         600, 0, 60, 600, 0, 600);
    
    fH_6g_fulinc_Eta_E_Theta        = new TH2F("6g_fulinc_Eta_E_Theta",        "6g_fulinc_Eta_E_Theta",        600, 0, 1200, 90, 0, 180);
    fH_6g_fulinc_Eta_E_Theta_Gen    = new TH2F("6g_fulinc_Eta_E_Theta_Gen",    "6g_fulinc_Eta_E_Theta_Gen",    600, 0, 1200, 90, 0, 180);
    
    fH_6g_fulinc_Coinc_Time_CB      = new TH1F("6g_fulinc_Coinc_Time_CB",      "6g_fulinc_Coinc_Time_CB",      800, -400, 400);
    fH_6g_fulinc_Coinc_Time_CB_TAPS = new TH1F("6g_fulinc_Coinc_Time_CB_TAPS", "6g_fulinc_Coinc_Time_CB_TAPS", 800, -400, 400);
    fH_6g_fulinc_Coinc_Time_TAPS    = new TH1F("6g_fulinc_Coinc_Time_TAPS",    "6g_fulinc_Coinc_Time_TAPS",    1000, -100, 100);
    fH_6g_fulinc_Rand_Time_CB       = new TH1F("6g_fulinc_Rand_Time_CB",       "6g_fulinc_Rand_Time_CB",       12000, -800, 400);
    fH_6g_fulinc_Rand_Time_TAPS     = new TH1F("6g_fulinc_Rand_Time_TAPS",     "6g_fulinc_Rand_Time_TAPS",     12000, -400, 800);
    
    fH_6g_fulinc_IM_Tot             = new TH2F("6g_fulinc_IM_Tot",             "6g_fulinc_IM_Tot",             500, 0, 1000, 272, 0, 272);
    fH_6g_fulinc_IM_E               = new TH2F("6g_fulinc_IM_E",               "6g_fulinc_IM_E",               500, 0, 1000, 500, 0, 1000);
    fH_6g_fulinc_MM_Tot             = new TH2F("6g_fulinc_MM_Tot",             "6g_fulinc_MM_Tot",             1000, -1000, 1000, 272, 0, 272);
    fH_6g_fulinc_MM_Tot->Sumw2();

    for (UInt_t i = 0; i < 6; i++)
    {
        sprintf(name, "6g_fulinc_IM_Debug_%d", i);
        fH_6g_fulinc_IM_Debug[i] = new TH1F(name, name, 500, 0, 1000);
    }
     
    fH_6g_fulinc_IM = new TH2F*[f6g_fulinc_NCTBin];
    fH_6g_fulinc_E = new TH2F*[f6g_fulinc_NCTBin];
    fH_6g_fulinc_ExFunc = new TH1F*[f6g_fulinc_NCTBin];
    for (Int_t i = 0; i < f6g_fulinc_NCTBin; i++)
    {
        sprintf(name, "6g_fulinc_IM_%d", i);
        fH_6g_fulinc_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_6g_fulinc_IM[i]->Sumw2();
        sprintf(name, "6g_fulinc_E_%d", i);
        fH_6g_fulinc_E[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        sprintf(name, "6g_fulinc_ExFunc_%d", i);
        fH_6g_fulinc_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_6g_fulinc_ExFunc[i]->Sumw2();
    }
 

    //
    // inclusive quasi-free g + X -> [eta->6g] + X
    //
    fH_6g_qfinc_Photon_TOF_TAPS    = new TH2F("6g_qfinc_Photon_TOF_TAPS",    "6g_qfinc_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_6g_qfinc_Photon_TOF_CB      = new TH2F("6g_qfinc_Photon_TOF_CB",      "6g_qfinc_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    
    fH_6g_qfinc_Photon_PSA         = new TH2F("6g_qfinc_Photon_PSA",         "6g_qfinc_Photon_PSA",         600, 0, 60, 600, 0, 600);
    
    fH_6g_qfinc_Coinc_Time_CB      = new TH1F("6g_qfinc_Coinc_Time_CB",      "6g_qfinc_Coinc_Time_CB",      1600, -400, 400);
    fH_6g_qfinc_Coinc_Time_CB_TAPS = new TH1F("6g_qfinc_Coinc_Time_CB_TAPS", "6g_qfinc_Coinc_Time_CB_TAPS", 1600, -400, 400);
    fH_6g_qfinc_Coinc_Time_TAPS    = new TH1F("6g_qfinc_Coinc_Time_TAPS",    "6g_qfinc_Coinc_Time_TAPS",    2000, -100, 100);
    fH_6g_qfinc_Rand_Time_CB       = new TH1F("6g_qfinc_Rand_Time_CB",       "6g_qfinc_Rand_Time_CB",       12000, -800, 400);
    fH_6g_qfinc_Rand_Time_TAPS     = new TH1F("6g_qfinc_Rand_Time_TAPS",     "6g_qfinc_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_6g_qfinc_IM_Tot             = new TH2F("6g_qfinc_IM_Tot",             "6g_qfinc_IM_Tot",              500, 0, 1000, 272, 0, 272);
    fH_6g_qfinc_MM_Tot             = new TH2F("6g_qfinc_MM_Tot",             "6g_qfinc_MM_Tot",              1000, -1000, 1000, 272, 0, 272);
    fH_6g_qfinc_MM_Tot->Sumw2();

    for (UInt_t i = 0; i < 6; i++)
    {
        sprintf(name, "6g_qfinc_IM_Debug_%d", i);
        fH_6g_qfinc_IM_Debug[i] = new TH1F(name, name, 500, 0, 1000);
    }
  
    fH_6g_qfinc_IM = new TH2F*[f6g_qfinc_NCTBin];
    fH_6g_qfinc_ExFunc = new TH1F*[f6g_qfinc_NCTBin];
    if (fCreateEta_p_Eff || fCreateEta_n_Eff)
    {
        fH_6g_qfinc_ExFunc_Gen = new TH1F*[f6g_qfinc_NCTBin];
    }
    for (Int_t i = 0; i < f6g_qfinc_NCTBin; i++)
    {
        sprintf(name, "6g_qfinc_IM_%d", i);
        fH_6g_qfinc_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_6g_qfinc_IM[i]->Sumw2();
        sprintf(name, "6g_qfinc_ExFunc_%d", i);
        fH_6g_qfinc_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_6g_qfinc_ExFunc[i]->Sumw2();
        if (fCreateEta_p_Eff || fCreateEta_n_Eff)
        {
            sprintf(name, "6g_qfinc_ExFunc_Gen_%d", i);
            fH_6g_qfinc_ExFunc_Gen[i] = new TH1F(name, name, 272, 0, 272);
        }
    }


    //
    // fully exclusive quasi-free g + p -> [eta->6g] + p
    //
    fH_6g_fulexc_p_Photon_TOF_TAPS    = new TH2F("6g_fulexc_p_Photon_TOF_TAPS",    "6g_fulexc_p_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_6g_fulexc_p_Proton_TOF_TAPS    = new TH2F("6g_fulexc_p_Proton_TOF_TAPS",    "6g_fulexc_p_Proton_TOF_TAPS",    1000, -50, 50, 500, 0, 500);
    
    fH_6g_fulexc_p_Photon_TOF_CB      = new TH2F("6g_fulexc_p_Photon_TOF_CB",      "6g_fulexc_p_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    fH_6g_fulexc_p_Proton_TOF_CB      = new TH2F("6g_fulexc_p_Proton_TOF_CB",      "6g_fulexc_p_Proton_TOF_CB",      1000, -50, 50, 500, 0, 500);
    
    fH_6g_fulexc_p_Photon_PSA         = new TH2F("6g_fulexc_p_Photon_PSA",         "6g_fulexc_p_Photon_PSA",         600, 0, 60, 600, 0, 600);
    fH_6g_fulexc_p_Proton_PSA         = new TH2F("6g_fulexc_p_Proton_PSA",         "6g_fulexc_p_Proton_PSA",         600, 0, 60, 600, 0, 600);
    
    fH_6g_fulexc_p_Proton_dE_E_CB     = new TH2F("6g_fulexc_p_Proton_dE_E_CB",     "6g_fulexc_p_Proton_dE_E_CB",     1000, 0, 1000, 400, 0, 10);
    fH_6g_fulexc_p_Proton_dE_E_TAPS   = new TH2F("6g_fulexc_p_Proton_dE_E_TAPS",   "6g_fulexc_p_Proton_dE_E_TAPS",   1000, 0, 1000, 400, 0, 10);
    
    fH_6g_fulexc_p_PhiDiff_Eta_Recoil = new TH1F("6g_fulexc_p_PhiDiff_Eta_Recoil", "6g_fulexc_p_PhiDiff_Eta_Recoil", 360, 0, 360);
    fH_6g_fulexc_p_Recoil_Fermi       = new TH1F("6g_fulexc_p_Recoil_Fermi",       "6g_fulexc_p_Recoil_Fermi",       1000, 0, 1000);
    
    fH_6g_fulexc_p_Coinc_Time_CB      = new TH1F("6g_fulexc_p_Coinc_Time_CB",      "6g_fulexc_p_Coinc_Time_CB",      800, -400, 400);
    fH_6g_fulexc_p_Coinc_Time_CB_TAPS = new TH1F("6g_fulexc_p_Coinc_Time_CB_TAPS", "6g_fulexc_p_Coinc_Time_CB_TAPS", 800, -400, 400);
    fH_6g_fulexc_p_Coinc_Time_TAPS    = new TH1F("6g_fulexc_p_Coinc_Time_TAPS",    "6g_fulexc_p_Coinc_Time_TAPS",    1000, -100, 100);
    fH_6g_fulexc_p_Rand_Time_CB       = new TH1F("6g_fulexc_p_Rand_Time_CB",       "6g_fulexc_p_Rand_Time_CB",       12000, -800, 400);
    fH_6g_fulexc_p_Rand_Time_TAPS     = new TH1F("6g_fulexc_p_Rand_Time_TAPS",     "6g_fulexc_p_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_6g_fulexc_p_IM_Tot             = new TH2F("6g_fulexc_p_IM_Tot",             "6g_fulexc_p_IM_Tot",             500, 0, 1000, 272, 0, 272);
    fH_6g_fulexc_p_MM_Tot             = new TH2F("6g_fulexc_p_MM_Tot",             "6g_fulexc_p_MM_Tot",             1000, -1000, 1000, 272, 0, 272);
    fH_6g_fulexc_p_MM_Tot->Sumw2();

    for (UInt_t i = 0; i < 6; i++)
    {
        sprintf(name, "6g_fulexc_p_IM_Debug_%d", i);
        fH_6g_fulexc_p_IM_Debug[i] = new TH1F(name, name, 500, 0, 1000);
    }
        
    fH_6g_fulexc_p_IM = new TH2F*[f6g_fulexc_p_NCTBin];
    fH_6g_fulexc_p_ExFunc = new TH1F*[f6g_fulexc_p_NCTBin];
    fH_6g_fulexc_p_IM_W = new TH2F*[f6g_fulexc_p_NCTBin];
    fH_6g_fulexc_p_ExFunc_W = new TH2F*[f6g_fulexc_p_NCTBin];
    if (fCreateEta_p_Eff)
    {
        fH_6g_fulexc_p_ExFunc_Gen = new TH1F*[f6g_fulexc_p_NCTBin];
        fH_6g_fulexc_p_ExFunc_W_Gen = new TH2F*[f6g_fulexc_p_NCTBin];
    }
    for (Int_t i = 0; i < f6g_fulexc_p_NCTBin; i++)
    {
        sprintf(name, "6g_fulexc_p_IM_%d", i);
        fH_6g_fulexc_p_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_6g_fulexc_p_IM[i]->Sumw2();
        sprintf(name, "6g_fulexc_p_ExFunc_%d", i);
        fH_6g_fulexc_p_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_6g_fulexc_p_ExFunc[i]->Sumw2();
        sprintf(name, "6g_fulexc_p_IM_W_%d", i);
        fH_6g_fulexc_p_IM_W[i] = new TH2F(name, name, 500, 0, 1000, 1000, 1000, 3000);
        fH_6g_fulexc_p_IM_W[i]->Sumw2();
        sprintf(name, "6g_fulexc_p_ExFunc_W_%d", i);
        fH_6g_fulexc_p_ExFunc_W[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_6g_fulexc_p_ExFunc_W[i]->Sumw2();
        if (fCreateEta_p_Eff)
        {
            sprintf(name, "6g_fulexc_p_ExFunc_Gen_%d", i);
            fH_6g_fulexc_p_ExFunc_Gen[i] = new TH1F(name, name, 272, 0, 272);
             sprintf(name, "6g_fulexc_p_ExFunc_W_Gen_%d", i);
            fH_6g_fulexc_p_ExFunc_W_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        }
    }
  
 
    //
    // fully exclusive quasi-free g + n -> [eta->6g] + n
    //
    fH_6g_fulexc_n_Photon_TOF_TAPS    = new TH2F("6g_fulexc_n_Photon_TOF_TAPS",    "6g_fulexc_n_Photon_TOF_TAPS",    1000, -50, 50, 1500, 0, 1500);
    fH_6g_fulexc_n_Neutron_TOF_TAPS   = new TH2F("6g_fulexc_n_Neutron_TOF_TAPS",   "6g_fulexc_n_Neutron_TOF_TAPS",   1000, -50, 50, 500, 0, 500);
    
    fH_6g_fulexc_n_Photon_TOF_CB      = new TH2F("6g_fulexc_n_Photon_TOF_CB",      "6g_fulexc_n_Photon_TOF_CB",      1000, -50, 50, 1500, 0, 1500);
    fH_6g_fulexc_n_Neutron_TOF_CB     = new TH2F("6g_fulexc_n_Neutron_TOF_CB",     "6g_fulexc_n_Neutron_TOF_CB",     1000, -50, 50, 500, 0, 500);
    
    fH_6g_fulexc_n_Photon_PSA         = new TH2F("6g_fulexc_n_Photon_PSA",         "6g_fulexc_n_Photon_PSA",         600, 0, 60, 600, 0, 600);
    fH_6g_fulexc_n_Neutron_PSA        = new TH2F("6g_fulexc_n_Neutron_PSA",        "6g_fulexc_n_Neutron_PSA",        600, 0, 60, 600, 0, 600);
    
    fH_6g_fulexc_n_PhiDiff_Eta_Recoil = new TH1F("6g_fulexc_n_PhiDiff_Eta_Recoil", "6g_fulexc_n_PhiDiff_Eta_Recoil", 360, 0, 360);
    fH_6g_fulexc_n_Recoil_Fermi       = new TH1F("6g_fulexc_n_Recoil_Fermi",       "6g_fulexc_n_Recoil_Fermi",       1000, 0, 1000);
    
    fH_6g_fulexc_n_Coinc_Time_CB      = new TH1F("6g_fulexc_n_Coinc_Time_CB",      "6g_fulexc_n_Coinc_Time_CB",      800, -400, 400);
    fH_6g_fulexc_n_Coinc_Time_CB_TAPS = new TH1F("6g_fulexc_n_Coinc_Time_CB_TAPS", "6g_fulexc_n_Coinc_Time_CB_TAPS", 800, -400, 400);
    fH_6g_fulexc_n_Coinc_Time_TAPS    = new TH1F("6g_fulexc_n_Coinc_Time_TAPS",    "6g_fulexc_n_Coinc_Time_TAPS",    1000, -100, 100);
    fH_6g_fulexc_n_Rand_Time_CB       = new TH1F("6g_fulexc_n_Rand_Time_CB",       "6g_fulexc_n_Rand_Time_CB",       12000, -800, 400);
    fH_6g_fulexc_n_Rand_Time_TAPS     = new TH1F("6g_fulexc_n_Rand_Time_TAPS",     "6g_fulexc_n_Rand_Time_TAPS",     12000, -400, 800);
      
    fH_6g_fulexc_n_IM_Tot             = new TH2F("6g_fulexc_n_IM_Tot",             "6g_fulexc_n_IM_Tot",             500, 0, 1000, 272, 0, 272);
    fH_6g_fulexc_n_MM_Tot             = new TH2F("6g_fulexc_n_MM_Tot",             "6g_fulexc_n_MM_Tot",             1000, -1000, 1000, 272, 0, 272);
    fH_6g_fulexc_n_MM_Tot->Sumw2();

    for (UInt_t i = 0; i < 6; i++)
    {
        sprintf(name, "6g_fulexc_n_IM_Debug_%d", i);
        fH_6g_fulexc_n_IM_Debug[i] = new TH1F(name, name, 500, 0, 1000);
    }
    
    fH_6g_fulexc_n_IM = new TH2F*[f6g_fulexc_n_NCTBin];
    fH_6g_fulexc_n_ExFunc = new TH1F*[f6g_fulexc_n_NCTBin];
    fH_6g_fulexc_n_IM_W = new TH2F*[f6g_fulexc_n_NCTBin];
    fH_6g_fulexc_n_ExFunc_W = new TH2F*[f6g_fulexc_n_NCTBin];
    if (fCreateEta_n_Eff)
    {
        fH_6g_fulexc_n_ExFunc_Gen = new TH1F*[f6g_fulexc_n_NCTBin];
        fH_6g_fulexc_n_ExFunc_W_Gen = new TH2F*[f6g_fulexc_n_NCTBin];
    }
    for (Int_t i = 0; i < f6g_fulexc_n_NCTBin; i++)
    {
        sprintf(name, "6g_fulexc_n_IM_%d", i);
        fH_6g_fulexc_n_IM[i] = new TH2F(name, name, 500, 0, 1000, 272, 0, 272);
        fH_6g_fulexc_n_IM[i]->Sumw2();
        sprintf(name, "6g_fulexc_n_ExFunc_%d", i);
        fH_6g_fulexc_n_ExFunc[i] = new TH1F(name, name, 272, 0, 272);
        fH_6g_fulexc_n_ExFunc[i]->Sumw2();
        sprintf(name, "6g_fulexc_n_IM_W_%d", i);
        fH_6g_fulexc_n_IM_W[i] = new TH2F(name, name, 500, 0, 1000, 1000, 1000, 3000);
        fH_6g_fulexc_n_IM_W[i]->Sumw2();
        sprintf(name, "6g_fulexc_n_ExFunc_W_%d", i);
        fH_6g_fulexc_n_ExFunc_W[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_6g_fulexc_n_ExFunc_W[i]->Sumw2();
        if (fCreateEta_n_Eff)
        {
            sprintf(name, "6g_fulexc_n_ExFunc_Gen_%d", i);
            fH_6g_fulexc_n_ExFunc_Gen[i] = new TH1F(name, name, 272, 0, 272);
            sprintf(name, "6g_fulexc_n_ExFunc_W_Gen_%d", i);
            fH_6g_fulexc_n_ExFunc_W_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        }
    }
    */
    
    //
    // exclusive quasi-free compton scattering g + p -> g + p
    //
    fH_Comp_p_Proton_TOF_TAPS      = new TH2F("Comp_p_Proton_TOF_TAPS",      "Comp_p_Proton_TOF_TAPS",      1000, -50, 50, 500, 0, 500);
    fH_Comp_p_Photon_TOF_CB        = new TH2F("Comp_p_Photon_TOF_CB",        "Comp_p_Photon_TOF_CB",        1000, -50, 50, 1500, 0, 1500);
    
    fH_Comp_p_Proton_PSA           = new TH2F("Comp_p_Proton_PSA",           "Comp_p_Proton_PSA",           600, 0, 60, 600, 0, 600);
    
    fH_Comp_p_Proton_dE_E_TAPS     = new TH2F("Comp_p_Proton_dE_E_TAPS",     "Comp_p_Proton_dE_E_TAPS",     1000, 0, 1000, 400, 0, 10);
    
    fH_Comp_p_2g_IM                = new TH1F("Comp_p_2g_IM",                "Comp_p_2g_IM",                1000, 0, 1000);
    fH_Comp_p_PhiDiff_Gamma_Recoil = new TH1F("Comp_p_PhiDiff_Gamma_Recoil", "Comp_p_PhiDiff_Gamma_Recoil", 360, 0, 360);
    fH_Comp_p_Recoil_Fermi         = new TH1F("Comp_p_Recoil_Fermi",         "Comp_p_Recoil_Fermi",         1000, 0, 1000);
    fH_Comp_p_ENuclKin_ENuclTOF    = new TH2F("Comp_p_ENuclKin_ENuclTOF",    "Comp_p_ENuclKin_ENuclTOF",    3000, 0, 3000, 3000, 0, 3000);
    fH_Comp_p_EBeam_CosThetaCM     = new TH2F("Comp_p_EBeam_CosThetaCM",     "Comp_p_EBeam_CosThetaCM",     272, 0, 272, 80, -1, 1);
    fH_Comp_p_E_Theta_Gamma        = new TH2F("Comp_p_E_Theta_Gamma",        "Comp_p_E_Theta_Gamma",        500, 0, 1000, 90, 0, 180);
    fH_Comp_p_E_Theta_Gamma_Miss   = new TH2F("Comp_p_E_Theta_Gamma_Miss",   "Comp_p_E_Theta_Gamma_Miss",   500, 0, 1000, 90, 0, 180);
   
    fH_Comp_p_Rand_Time_CB         = new TH1F("Comp_p_Rand_Time_CB",         "Comp_p_Rand_Time_CB",         2400, -800, 400);
      
    fH_Comp_p_ME_W_Tot             = new TH2F("Comp_p_ME_W_Tot",             "Comp_p_ME_W_Tot",             1000, -1000, 1000, 1000, 1000, 3000);
    
    fH_Comp_p_ME_W = new TH2F*[fComp_p_NCTBin];
    fH_Comp_p_ExFunc_W = new TH2F*[fComp_p_NCTBin];
    fH_Comp_p_ExFunc_W_90_110 = new TH2F*[fComp_p_NCTBin];
    fH_Comp_p_ExFunc_W_110_130 = new TH2F*[fComp_p_NCTBin];
    fH_Comp_p_ExFunc_W_130_150 = new TH2F*[fComp_p_NCTBin];
    fH_Comp_p_ME_W_TOF = new TH2F*[fComp_p_NCTBin];
    fH_Comp_p_ExFunc_W_TOF = new TH2F*[fComp_p_NCTBin];
    if (fCreateComp_p_Eff)
    {
        fH_Comp_p_ExFunc_W_Gen = new TH2F*[fComp_p_NCTBin];
        fH_Comp_p_Gen = new THnSparseF("Comp_p_Gen", "Comp_p_Gen", 5, bins, xmin, xmax);
        fH_Comp_p_Gen->Sumw2();
        gDirectory->Add(fH_Comp_p_Gen);
    }
    for (Int_t i = 0; i < fComp_p_NCTBin; i++)
    {
        sprintf(name, "Comp_p_ME_W_%d", i);
        fH_Comp_p_ME_W[i] = new TH2F(name, name, 1000, -1000, 1000, 1000, 1000, 3000);
        sprintf(name, "Comp_p_ExFunc_W_%d", i);
        fH_Comp_p_ExFunc_W[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_p_ExFunc_W[i]->Sumw2();
        sprintf(name, "Comp_p_ExFunc_W_90_110_%d", i);
        fH_Comp_p_ExFunc_W_90_110[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_p_ExFunc_W_90_110[i]->Sumw2();
        sprintf(name, "Comp_p_ExFunc_W_110_130_%d", i);
        fH_Comp_p_ExFunc_W_110_130[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_p_ExFunc_W_110_130[i]->Sumw2();
        sprintf(name, "Comp_p_ExFunc_W_130_150_%d", i);
        fH_Comp_p_ExFunc_W_130_150[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_p_ExFunc_W_130_150[i]->Sumw2();
        sprintf(name, "Comp_p_ME_W_TOF_%d", i);
        fH_Comp_p_ME_W_TOF[i] = new TH2F(name, name, 1000, -1000, 1000, 1000, 1000, 3000);
        sprintf(name, "Comp_p_ExFunc_W_TOF_%d", i);
        fH_Comp_p_ExFunc_W_TOF[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_p_ExFunc_W_TOF[i]->Sumw2();
        if (fCreateComp_p_Eff)
        {
            sprintf(name, "Comp_p_ExFunc_W_Gen_%d", i);
            fH_Comp_p_ExFunc_W_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        }
    }

    fH_Comp_p_Det = new THnSparseF("Comp_p_Det", "Comp_p_Det", 5, bins, xmin, xmax);
    fH_Comp_p_Det->Sumw2();
    gDirectory->Add(fH_Comp_p_Det);
    fH_Comp_p_MVA_Value            = new TH1F("Comp_p_MVA_Value"      ,      "Comp_p_MVA_Value",            200, -10, 10);


    //
    // exclusive quasi-free compton scattering g + n -> g + n
    //
    fH_Comp_n_Neutron_TOF_TAPS     = new TH2F("Comp_n_Neutron_TOF_TAPS",     "Comp_n_Neutron_TOF_TAPS",     1000, -50, 50, 500, 0, 500);
    
    fH_Comp_n_Photon_TOF_CB        = new TH2F("Comp_n_Photon_TOF_CB",        "Comp_n_Photon_TOF_CB",        1000, -50, 50, 1500, 0, 1500);
    
    fH_Comp_n_Neutron_PSA          = new TH2F("Comp_n_Neutron_PSA",          "Comp_n_Neutron_PSA",          600, 0, 60, 600, 0, 600);
    
    fH_Comp_n_2g_IM                = new TH1F("Comp_n_2g_IM",                "Comp_n_2g_IM",                1000, 0, 1000);
    fH_Comp_n_PhiDiff_Gamma_Recoil = new TH1F("Comp_n_PhiDiff_Gamma_Recoil", "Comp_n_PhiDiff_Gamma_Recoil", 360, 0, 360);
    fH_Comp_n_Recoil_Fermi         = new TH1F("Comp_n_Recoil_Fermi",         "Comp_n_Recoil_Fermi",         1000, 0, 1000);
    fH_Comp_n_ENuclKin_ENuclTOF    = new TH2F("Comp_n_ENuclKin_ENuclTOF",    "Comp_n_ENuclKin_ENuclTOF",    3000, 0, 3000, 3000, 0, 3000);
    fH_Comp_n_EBeam_CosThetaCM     = new TH2F("Comp_n_EBeam_CosThetaCM",     "Comp_n_EBeam_CosThetaCM",     272, 0, 272, 80, -1, 1);
    fH_Comp_n_E_Theta_Gamma        = new TH2F("Comp_n_E_Theta_Gamma",        "Comp_n_E_Theta_Gamma",        500, 0, 1000, 90, 0, 180);
    fH_Comp_n_E_Theta_Gamma_Miss   = new TH2F("Comp_n_E_Theta_Gamma_Miss",   "Comp_n_E_Theta_Gamma_Miss",   500, 0, 1000, 90, 0, 180);
  
    fH_Comp_n_Rand_Time_CB         = new TH1F("Comp_n_Rand_Time_CB",         "Comp_n_Rand_Time_CB",         2400, -800, 400);
      
    fH_Comp_n_ME_W_Tot             = new TH2F("Comp_n_ME_W_Tot",             "Comp_n_ME_W_Tot",             1000, -1000, 1000, 1000, 1000, 3000);
  
    fH_Comp_n_ME_W = new TH2F*[fComp_n_NCTBin];
    fH_Comp_n_ExFunc_W = new TH2F*[fComp_n_NCTBin];
    fH_Comp_n_ExFunc_W_90_110 = new TH2F*[fComp_n_NCTBin];
    fH_Comp_n_ExFunc_W_110_130 = new TH2F*[fComp_n_NCTBin];
    fH_Comp_n_ExFunc_W_130_150 = new TH2F*[fComp_n_NCTBin];
    fH_Comp_n_ME_W_TOF = new TH2F*[fComp_n_NCTBin];
    fH_Comp_n_ExFunc_W_TOF = new TH2F*[fComp_n_NCTBin];
    if (fCreateComp_n_Eff)
    {
        fH_Comp_n_ExFunc_W_Gen = new TH2F*[fComp_n_NCTBin];
        fH_Comp_n_Gen = new THnSparseF("Comp_n_Gen", "Comp_n_Gen", 5, bins, xmin, xmax);
        fH_Comp_n_Gen->Sumw2();
        gDirectory->Add(fH_Comp_n_Gen);
    }
    for (Int_t i = 0; i < fComp_n_NCTBin; i++)
    {
        sprintf(name, "Comp_n_ME_W_%d", i);
        fH_Comp_n_ME_W[i] = new TH2F(name, name, 1000, -1000, 1000, 1000, 1000, 3000);
        sprintf(name, "Comp_n_ExFunc_W_%d", i);
        fH_Comp_n_ExFunc_W[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_n_ExFunc_W[i]->Sumw2();
        sprintf(name, "Comp_n_ExFunc_W_90_110_%d", i);
        fH_Comp_n_ExFunc_W_90_110[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_n_ExFunc_W_90_110[i]->Sumw2();
        sprintf(name, "Comp_n_ExFunc_W_110_130_%d", i);
        fH_Comp_n_ExFunc_W_110_130[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_n_ExFunc_W_110_130[i]->Sumw2();
        sprintf(name, "Comp_n_ExFunc_W_130_150_%d", i);
        fH_Comp_n_ExFunc_W_130_150[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_n_ExFunc_W_130_150[i]->Sumw2();
        sprintf(name, "Comp_n_ME_W_TOF_%d", i);
        fH_Comp_n_ME_W_TOF[i] = new TH2F(name, name, 1000, -1000, 1000, 1000, 1000, 3000);
        sprintf(name, "Comp_n_ExFunc_W_TOF_%d", i);
        fH_Comp_n_ExFunc_W_TOF[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        fH_Comp_n_ExFunc_W_TOF[i]->Sumw2();
        if (fCreateComp_n_Eff)
        {
            sprintf(name, "Comp_n_ExFunc_W_Gen_%d", i);
            fH_Comp_n_ExFunc_W_Gen[i] = new TH2F(name, name, 1000, 1000, 3000, 272, 0, 272);
        }
    }
    
    fH_Comp_n_Det = new THnSparseF("Comp_n_Det", "Comp_n_Det", 5, bins, xmin, xmax);
    fH_Comp_n_Det->Sumw2();
    gDirectory->Add(fH_Comp_n_Det);
    fH_Comp_n_MVA_Value            = new TH1F("Comp_n_MVA_Value"      ,      "Comp_n_MVA_Value",            200, -10, 10);
    

    //hTestP = new TH1F("hTestP", "hTestP", 1000, 0, 1000);
    //hTestN = new TH1F("hTestN", "hTestN", 1000, 0, 1000);
}

//______________________________________________________________________________
void TA2MyAnalysis::ReconstructPhysics()
{
    // Main analysis method that is called for every event.
    
    //FILE* outText;
    //outText = fopen("/tmp/analysis_out.dat", "a");

    
    ////////////////////////////////////////////////////////////////////////////
    // Set MC generated particle 4-vectors for efficiency determination       //
    // and fill the generated events into histograms                          //
    ////////////////////////////////////////////////////////////////////////////
    
    TLorentzVector* p4GammaMC = 0;
    TLorentzVector* p4EtaMC = 0;
    TLorentzVector* p4RecoilMC = 0;
    Double_t cosThetaCM_MC = 2;
    Double_t cosThetaCMTrue_MC = 2;
    Double_t w_true_mc = 0;

    // grid efficiency for single etas
    if (fCreateEtaGridEff)
    {
        // set MC eta 4-vector
        p4EtaMC = fPartMC[0]->GetP4();
    
        // fill histogram of generated etas
        fH_2g_fulinc_Eta_E_Theta_Gen->Fill(p4EtaMC->E() - p4EtaMC->M(), p4EtaMC->Theta()*TMath::RadToDeg());
        //fH_6g_fulinc_Eta_E_Theta_Gen->Fill(p4EtaMC->E() - p4EtaMC->M(), p4EtaMC->Theta()*TMath::RadToDeg());
    }

    // efficiency for quasi-free eta proton/neutron
    if (fCreateEta_p_Eff || fCreateEta_n_Eff)
    {
        // set the recoil nucleon and the eta 4-vectors
        p4RecoilMC = fPartMC[2]->GetP4();
        p4EtaMC    = fPartMC[3]->GetP4();
        
        // set the target 4-vector
        TLorentzVector p4Target;
        if (fCreateEta_p_Eff) p4Target.SetPxPyPzE(0., 0., 0., TOGlobals::kProtonMass);
        else p4Target.SetPxPyPzE(0., 0., 0., TOGlobals::kNeutronMass);
        
        // beam 4-vector
        TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[0], fTaggerPhotonEnergy[0]);
        
        // calculate center-of-mass energy
        TLorentzVector p4CM = p4Beam + p4Target; 
        
        // calculate true center-of-mass energy
        TLorentzVector p4CMTrue = *p4RecoilMC + *p4EtaMC;
        w_true_mc = p4CMTrue.M();

        // eta 4-vector in c.m. frame
        TLorentzVector p4EtaCM(*p4EtaMC);
        p4EtaCM.Boost(-p4CM.BoostVector());
        cosThetaCM_MC = p4EtaCM.CosTheta();
     
        // eta 4-vector in true c.m. frame
        TLorentzVector p4EtaCMTrue(*p4EtaMC);
        p4EtaCMTrue.Boost(-p4CMTrue.BoostVector());
        cosThetaCMTrue_MC = p4EtaCMTrue.CosTheta();
           
        // fill the excitation function histograms
        if (fCreateEta_p_Eff) 
        {
            fH_2g_qfinc_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f2g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            fH_2g_fulexc_p_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f2g_fulexc_p_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            fH_2g_fulexc_p_ExFunc_W_Gen[GetCTBin(cosThetaCMTrue_MC, f2g_fulexc_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[0]);
            
            //fH_6g_qfinc_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f6g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            //fH_6g_fulexc_p_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f6g_fulexc_p_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            //fH_6g_fulexc_p_ExFunc_W_Gen[GetCTBin(cosThetaCMTrue_MC, f6g_fulexc_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[0]);
        }
        else
        {
            fH_2g_qfinc_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f2g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            fH_2g_fulexc_n_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f2g_fulexc_n_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            fH_2g_fulexc_n_ExFunc_W_Gen[GetCTBin(cosThetaCMTrue_MC, f2g_fulexc_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[0]);
            
            //fH_6g_qfinc_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f6g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            //fH_6g_fulexc_n_ExFunc_Gen[GetCTBin(cosThetaCM_MC, f6g_fulexc_n_NCTBin)]->Fill(fTaggerPhotonHits[0]);
            //fH_6g_fulexc_n_ExFunc_W_Gen[GetCTBin(cosThetaCMTrue_MC, f6g_fulexc_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[0]);
        }
    }

    // efficiency for quasi-free Compton proton/neutron
    if (fCreateComp_p_Eff || fCreateComp_n_Eff)
    {
        // set the recoil nucleon and the Compton gamma 4-vectors
        p4RecoilMC = fPartMC[2]->GetP4();
        p4GammaMC  = fPartMC[3]->GetP4();
        
        // set the target 4-vector
        TLorentzVector p4Target;
        if (fCreateComp_p_Eff) p4Target.SetPxPyPzE(0., 0., 0., TOGlobals::kProtonMass);
        else p4Target.SetPxPyPzE(0., 0., 0., TOGlobals::kNeutronMass);
        
        // beam 4-vector
        TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[0], fTaggerPhotonEnergy[0]);
        
        // calculate true c.m. boost vector
        TLorentzVector p4CMTrue = *p4RecoilMC + *p4GammaMC;
        w_true_mc = p4CMTrue.M();

        // Compton gamma 4-vector in true c.m. frame
        TLorentzVector p4GammaCMTrueMC(*p4GammaMC);
        p4GammaCMTrueMC.Boost(-p4CMTrue.BoostVector());
        cosThetaCMTrue_MC = p4GammaCMTrueMC.CosTheta();
           
        // fill the excitation function histograms
        if (fCreateComp_p_Eff) 
        {
            fH_Comp_p_ExFunc_W_Gen[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[0]);
            Double_t xtup[5] = { fTaggerPhotonHits[0], p4GammaMC->E(), p4GammaMC->Theta() * TMath::RadToDeg(),
                                 p4RecoilMC->E() - TOGlobals::kProtonMass, p4RecoilMC->Theta() * TMath::RadToDeg() };
            fH_Comp_p_Gen->Fill(xtup);
        }
        else 
        {
            fH_Comp_n_ExFunc_W_Gen[GetCTBin(cosThetaCMTrue_MC, fComp_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[0]);
            Double_t xtup[5] = { fTaggerPhotonHits[0], p4GammaMC->E(), p4GammaMC->Theta() * TMath::RadToDeg(),
                                 p4RecoilMC->E() - TOGlobals::kNeutronMass, p4RecoilMC->Theta() * TMath::RadToDeg() };
            fH_Comp_n_Gen->Fill(xtup);
         }
    }


       
    ////////////////////////////////////////////////////////////////////////////
    // fully inclusive eta -> 2g + X reconstruction                           //
    ////////////////////////////////////////////////////////////////////////////
 
    if (fNNeutral == 2 || fNNeutral == 3 || fNNeutral == 4 || fNNeutral == 5)
    {
        // accept only 2 clusters from the simulation
        if (fIsMC && fNNeutral != 2) goto label_end_2g_fulinc;

        // reconstruct eta
        TOA2RecEta2g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_2g_fulinc;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();
        
        // check software trigger
        if (!IsTrigger(eta)) goto label_end_2g_fulinc;
         
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-5.18, 3.63, fH_2g_fulinc_Coinc_Time_CB,
        //                                           -1,    1,    fH_2g_fulinc_Coinc_Time_TAPS,
        //                                           -3.51, 3.43, fH_2g_fulinc_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_2g_fulinc;
        
        // get invariant mass, kinetic energy and theta
        TLorentzVector* p4Eta = eta.Get4Vector();
        TLorentzVector* p4EtaConstr = eta.GetConstrained4Vector(TOGlobals::kEtaMass);
        Double_t im = p4Eta->M();
        Double_t ekinEta = p4EtaConstr->E() - p4EtaConstr->M();
        Double_t thetaEta = p4EtaConstr->Theta()*TMath::RadToDeg();
        
        // get detection efficiency
        Int_t binX = fH_2g_fulinc_DetEff->GetXaxis()->FindBin(ekinEta);
        Int_t binY = fH_2g_fulinc_DetEff->GetYaxis()->FindBin(thetaEta);
        Double_t eff;
        if (!fIsMC) eff = fH_2g_fulinc_DetEff->GetBinContent(binX, binY);
        else eff = 1.;
        if (eff == 0) goto label_end_2g_fulinc;

        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 420 && im < 620) imOk = kTRUE;
    
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -4.5, 4.5, -650, -50, 50, 250,
                                                           -4.5, 3.5, -250, -50, 50, 650);
            
            // fill tagger coincidence time
            if (imOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_2g_fulinc_Rand_Time_CB, fH_2g_fulinc_Rand_Time_TAPS);

            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;

            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
            
            // missing mass
            Double_t mm = (p4Beam + p4Target - *p4EtaConstr).M() - TOGlobals::kProtonMass;
            
            // calculate center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 

            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(*p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();

            // fill invariant mass
            fH_2g_fulinc_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight / eff);
            fH_2g_fulinc_IM_E->Fill(im, ekinEta, subtr_weight);
            fH_2g_fulinc_IM[GetCTBin(cosThetaCM, f2g_fulinc_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight / eff);
            
            // invariant mass cut
            if (imOk)  
            {
                // missing mass
                fH_2g_fulinc_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
                
                // kinetic energy
                fH_2g_fulinc_E[GetCTBin(cosThetaCM, f2g_fulinc_NCTBin)]->Fill(ekinEta, fTaggerPhotonHits[i], subtr_weight);

                // check TOF of photons
                for (Int_t j = 0; j < 2; j++) 
                    FillTOF(fH_2g_fulinc_Photon_TOF_CB, fH_2g_fulinc_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);

                // check PSA of photons
                for (Int_t j = 0; j < 2; j++) FillPSA(fH_2g_fulinc_Photon_PSA, decay_photons[j], subtr_weight);
   
                // excitation function
                FillExFunc(fTaggerPhotonHits[i], fH_2g_fulinc_ExFunc[GetCTBin(cosThetaCM, f2g_fulinc_NCTBin)], subtr_weight, ekinEta, thetaEta, fH_2g_fulinc_DetEff);

                // eta energy and theta: fill for efficiency determination or normal analysis
                if (fCreateEtaGridEff) 
                {
                    fH_2g_fulinc_Eta_E_Theta->Fill(p4EtaMC->E() - p4EtaMC->M(), p4EtaMC->Theta()*TMath::RadToDeg());
                    
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 30)
                    {
                        printf("generated: %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected : %f %f %f %f\n", p4EtaConstr->E(), p4EtaConstr->Px(), p4EtaConstr->Py(), p4EtaConstr->Pz());
                        printf("\n");
                    }
                }
                else fH_2g_fulinc_Eta_E_Theta->Fill(ekinEta, thetaEta, subtr_weight);
            }

        } // tagger loop

    } // end fully inclusive eta -> 2g reconstruction
    label_end_2g_fulinc:
 

    
    ////////////////////////////////////////////////////////////////////////////
    // q.f. inclusive g + X -> [eta->2g] + X reconstruction                   //
    ////////////////////////////////////////////////////////////////////////////
 
    if ((fNNeutral == 2 && fNCharged == 0) ||
        (fNNeutral == 2 && fNCharged == 1) ||
        (fNNeutral == 3 && fNCharged == 0))

    {
        // reconstruct eta
        TOA2RecEta2g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_2g_qfinc;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();
        
        // check software trigger
        if (!IsTrigger(eta)) goto label_end_2g_qfinc;
        
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-4.79, 3.31, fH_2g_qfinc_Coinc_Time_CB,
        //                                           -1,    1,    fH_2g_qfinc_Coinc_Time_TAPS,
        //                                           -3.12, 3.22, fH_2g_qfinc_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_2g_qfinc;
    
        // get invariant mass, kinetic energy and theta
        TLorentzVector* p4Eta = eta.Get4Vector();
        TLorentzVector* p4EtaConstr = eta.GetConstrained4Vector(TOGlobals::kEtaMass);
        Double_t im = p4Eta->M();
        Double_t ekinEta = p4EtaConstr->E() - p4EtaConstr->M();
        Double_t thetaEta = p4EtaConstr->Theta()*TMath::RadToDeg();

        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 480 && im < 620) imOk = kTRUE;
        
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
           
            // missing mass cut
            Double_t mm = (p4Beam + p4Target - *p4EtaConstr).M() - TOGlobals::kProtonMass;
            //Int_t mm_tagg_element;
            //if (fTaggerPhotonHits[i] < 210) mm_tagg_element = fTaggerPhotonHits[i];
            //else mm_tagg_element = 210;
            Bool_t mmOk = kTRUE;
            if (mm > 0) mmOk = kFALSE;
            //if ((mm < fG_2g_qfinc_MM_Min->Eval(mm_tagg_element)) ||
            //    (mm > fG_2g_qfinc_MM_Max->Eval(mm_tagg_element))) mmOk = kFALSE;
            
            // fill tagger coincidence time
            if (imOk && mmOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_2g_qfinc_Rand_Time_CB, fH_2g_qfinc_Rand_Time_TAPS);
  
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -4.5, 4.5, -650, -50, 50, 250,
                                                           -4.5, 3.5, -250, -50, 50, 650);
            
            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
          
            // calculate center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 
            
            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(*p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();
            
            // apply phase space cut
            Double_t xtup[5] = { fTaggerPhotonHits[i], ekinEta, thetaEta, 
                                 (p4Beam + p4Target - *p4EtaConstr).E() - TOGlobals::kProtonMass,
                                 (p4Beam + p4Target - *p4EtaConstr).Theta() * TMath::RadToDeg()
                               };
            Double_t ps_cont = TOHUtils::GetBinContent(fH_2g_qfinc_PhaseSpace, xtup);
            //if (ps_cont == 0 && ctBin != f2g_qfinc_NCTBin-1) continue;
 
            // fill invariant mass
            if (mmOk)
            {
                fH_2g_qfinc_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                fH_2g_qfinc_IM[GetCTBin(cosThetaCM, f2g_qfinc_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight);
            }

            // fill missing mass
            if (imOk)  
            {
                fH_2g_qfinc_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
            }
            
            // fill phase space
            if ((fCreateEta_p_Eff || fCreateEta_n_Eff) && imOk) 
            {
                Double_t xtup[5] = { fTaggerPhotonHits[i], ekinEta, thetaEta,
                                     (p4Beam + p4Target - *p4EtaConstr).E() - TOGlobals::kProtonMass, 
                                     (p4Beam + p4Target - *p4EtaConstr).Theta() * TMath::RadToDeg()
                                   };
                fH_2g_qfinc_Det->Fill(xtup, subtr_weight);
            }

            // fill good events
            if (imOk && mmOk)
            {
                // check TOF of photons
                for (Int_t j = 0; j < 2; j++) 
                    FillTOF(fH_2g_qfinc_Photon_TOF_CB, fH_2g_qfinc_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check PSA of photons
                for (Int_t j = 0; j < 2; j++) FillPSA(fH_2g_qfinc_Photon_PSA, decay_photons[j], subtr_weight);
                
                // MC efficiency determination 
                if (fCreateEta_p_Eff || fCreateEta_n_Eff) 
                {
                    // excitation function
                    fH_2g_qfinc_ExFunc[GetCTBin(cosThetaCM_MC, f2g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[i]);
     
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated eta   : %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected eta    : %f %f %f %f\n", p4EtaConstr->E(), p4EtaConstr->Px(), p4EtaConstr->Py(), p4EtaConstr->Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function
                    fH_2g_qfinc_ExFunc[GetCTBin(cosThetaCM, f2g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[i], subtr_weight);
                }
            }

        } // tagger loop

    } // end q.f. inclusive g + X -> [eta->2g] + X reconstruction
    label_end_2g_qfinc:



    ////////////////////////////////////////////////////////////////////////////
    // fully exclusive g + p -> [eta->2g] + p reconstruction                  //
    ////////////////////////////////////////////////////////////////////////////
 
    if (fNNeutral == 2 && fNCharged == 1)
    {
        // reconstruct eta
        TOA2RecEta2g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_2g_fulexc_p;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();
        
        // recoil proton
        TOA2DetParticle* recoil_proton = fPartCharged[0];
        TLorentzVector p4RecoilProton;
        SetP4(p4RecoilProton, recoil_proton->GetEnergy(), TOGlobals::kProtonMass, 
              recoil_proton->GetX(), recoil_proton->GetY(), recoil_proton->GetZ());

        // check software trigger
        if (!IsTrigger(eta)) goto label_end_2g_fulexc_p;
        
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-4.33, 2.95, fH_2g_fulexc_p_Coinc_Time_CB,
        //                                           -1,    1,    fH_2g_fulexc_p_Coinc_Time_TAPS,
        //                                           -3.13, 3.19, fH_2g_fulexc_p_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_2g_fulexc_p;
        
        // get invariant mass, kinetic energy and theta
        TLorentzVector* p4Eta = eta.Get4Vector();
        TLorentzVector* p4EtaConstr = eta.GetConstrained4Vector(TOGlobals::kEtaMass);
        Double_t im = p4Eta->M();
        Double_t ekinEta = p4EtaConstr->E() - p4EtaConstr->M();
        Double_t thetaEta = p4EtaConstr->Theta()*TMath::RadToDeg();
        
        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 480 && im < 620) imOk = kTRUE;
        
        // check eta-proton kinematics
        Double_t phi_diff = TMath::Abs(p4EtaConstr->Phi() - p4RecoilProton.Phi())*TMath::RadToDeg();
        if (imOk) fH_2g_fulexc_p_PhiDiff_Eta_Recoil->Fill(phi_diff);
        Bool_t copOk = kTRUE;
        //if (phi_diff < 130 || phi_diff > 220) copOk = kFALSE;
        if (phi_diff < 170 || phi_diff > 190) copOk = kFALSE;
        
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
           
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
   
            // calculate kinetic energy of recoil proton
            Double_t ekinRecoil = TOPhysics::CalcQFThreeBodyRecoilPartT(fTaggerPhotonEnergy[i], p4EtaConstr, &p4RecoilProton,
                                                                        TOGlobals::kDeuteronMass, TOGlobals::kProtonMass, TOGlobals::kNeutronMass);
            
            // calculate real 4-vector of recoil proton
            TLorentzVector p4RecoilProtonCalc;
            SetP4(p4RecoilProtonCalc, ekinRecoil, TOGlobals::kProtonMass, p4RecoilProton.X(), p4RecoilProton.Y(), p4RecoilProton.Z());
            Double_t thetaRecoil = p4RecoilProtonCalc.Theta() * TMath::RadToDeg();
            
            // calculate kinetic energy of the recoil proton using TOF
            Double_t ekinRecoilTOF = recoil_proton->CalculateEkinTOFTagger(TOGlobals::kProtonMass, fTaggerPhotonTime[i], fIsMC);

            // calculate real 4-vector of recoil proton using TOF
            TLorentzVector p4RecoilProtonCalcTOF;
            SetP4(p4RecoilProtonCalcTOF, ekinRecoilTOF, TOGlobals::kProtonMass, p4RecoilProton.X(), p4RecoilProton.Y(), p4RecoilProton.Z());
 
            // skip unphysical solutions of the recoil energy calculation
            Bool_t true_cm_ok = kTRUE;
            if (ekinRecoil <= 0 || TMath::IsNaN(ekinRecoil)) true_cm_ok = kFALSE;

            // skip unphysical solutions of the recoil energy calculation using TOF
            Bool_t true_cm_tof_ok = kTRUE;
            if (recoil_proton->GetDetector() != kTAPSDetector || ekinRecoilTOF <= 0) true_cm_tof_ok = kFALSE;
            
            // missing mass cut
            Double_t mm = (p4Beam + p4Target - *p4EtaConstr).M() - TOGlobals::kProtonMass;
            //Int_t mm_tagg_element;
            //if (fTaggerPhotonHits[i] < 210) mm_tagg_element = fTaggerPhotonHits[i];
            //else mm_tagg_element = 210;
            Bool_t mmOk = kTRUE;
            if (mm > 0) mm = kFALSE;
            //if ((mm < fG_2g_fulexc_p_MM_Min->Eval(mm_tagg_element)) ||
            //    (mm > fG_2g_fulexc_p_MM_Max->Eval(mm_tagg_element))) mmOk = kFALSE;
             
            // fill tagger coincidence time
            if (imOk && mmOk && copOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_2g_fulexc_p_Rand_Time_CB, fH_2g_fulexc_p_Rand_Time_TAPS);
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -4.5, 4.5, -650, -50, 50, 250,
                                                           -4.5, 3.5, -250, -50, 50, 650);
             
            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
            
            // calculate center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 

            // calculate true center-of-mass energy
            TLorentzVector p4CMTrue = p4RecoilProtonCalc + *p4EtaConstr;
            Double_t w_true = p4CMTrue.M();
            
            // calculate true center-of-mass energy using TOF
            TLorentzVector p4CMTrueTOF = p4RecoilProtonCalcTOF + *p4EtaConstr;
            Double_t w_true_tof = p4CMTrueTOF.M();
        
            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(*p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();
  
            // eta 4-vector in true c.m. frame
            TLorentzVector p4EtaCMTrue(*p4EtaConstr);
            p4EtaCMTrue.Boost(-p4CMTrue.BoostVector());
            Double_t cosThetaCMTrue = p4EtaCMTrue.CosTheta();
            
            // eta 4-vector in true c.m. frame using TOF
            TLorentzVector p4EtaCMTrueTOF(*p4EtaConstr);
            p4EtaCMTrueTOF.Boost(-p4CMTrueTOF.BoostVector());
            Double_t cosThetaCMTrueTOF = p4EtaCMTrueTOF.CosTheta();
             
            // apply phase space cut
            Double_t xtup[5] = { fTaggerPhotonHits[i], ekinEta, thetaEta, 
                                 (p4Beam + p4Target - *p4EtaConstr).E() - TOGlobals::kProtonMass,
                                 (p4Beam + p4Target - *p4EtaConstr).Theta() * TMath::RadToDeg()
                               };
            Double_t ps_cont = TOHUtils::GetBinContent(fH_2g_fulexc_p_PhaseSpace, xtup);
            //if (ps_cont == 0 && ctBin != f2g_fulexc_p_NCTBin-1 && ctBinTrue != f2g_fulexc_p_NCTBin-1) continue;
            
            // fill invariant mass
            if (mmOk && copOk)
            {
                fH_2g_fulexc_p_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                fH_2g_fulexc_p_IM[GetCTBin(cosThetaCM, f2g_fulexc_p_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                if (true_cm_ok) fH_2g_fulexc_p_IM_W[GetCTBin(cosThetaCMTrue, f2g_fulexc_p_NCTBin)]->Fill(im, w_true, subtr_weight);
                if (true_cm_tof_ok) fH_2g_fulexc_p_IM_W_TOF[GetCTBin(cosThetaCMTrueTOF, f2g_fulexc_p_NCTBin)]->Fill(im, w_true_tof, subtr_weight);
            }

            // fill missing mass
            if (imOk && copOk)  
            {
                fH_2g_fulexc_p_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
            }
            
            // fill good events
            if (imOk && mmOk && copOk)
            {
                // check TOF of photons
                for (Int_t j = 0; j < 2; j++) 
                    FillTOF(fH_2g_fulexc_p_Photon_TOF_CB, fH_2g_fulexc_p_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check TOF and dE/E of recoil proton
                FillTOF(fH_2g_fulexc_p_Proton_TOF_CB, fH_2g_fulexc_p_Proton_TOF_TAPS, recoil_proton, fTaggerPhotonTime[i], fIsMC, subtr_weight);
                FilldE_E(fH_2g_fulexc_p_Proton_dE_E_CB, fH_2g_fulexc_p_Proton_dE_E_TAPS, recoil_proton, subtr_weight);

                // check PSA
                for (Int_t j = 0; j < 2; j++) FillPSA(fH_2g_fulexc_p_Photon_PSA, decay_photons[j], subtr_weight);
                FillPSA(fH_2g_fulexc_p_Proton_PSA, recoil_proton, subtr_weight);
                
                // check Fermi momentum
                fH_2g_fulexc_p_Recoil_Fermi->Fill((p4Beam.Vect() - p4RecoilProtonCalc.Vect() - p4EtaConstr->Vect()).Mag(), subtr_weight);
                
                // MC efficiency determination 
                if (fCreateEta_p_Eff) 
                {
                    // excitation function
                    fH_2g_fulexc_p_ExFunc[GetCTBin(cosThetaCM_MC, f2g_fulexc_p_NCTBin)]->Fill(fTaggerPhotonHits[i]);
 
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok) fH_2g_fulexc_p_ExFunc_W[GetCTBin(cosThetaCMTrue_MC, f2g_fulexc_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                    
                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_2g_fulexc_p_ExFunc_W_TOF[GetCTBin(cosThetaCMTrue_MC, f2g_fulexc_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                     
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated eta    : %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected eta     : %f %f %f %f\n", p4EtaConstr->E(), p4EtaConstr->Px(), p4EtaConstr->Py(), p4EtaConstr->Pz());
                        printf("generated proton : %f %f %f %f\n", p4RecoilMC->E(), p4RecoilMC->Px(), p4RecoilMC->Py(), p4RecoilMC->Pz());
                        printf("detected proton  : %f %f %f %f\n", p4RecoilProtonCalc.E(), p4RecoilProtonCalc.Px(), p4RecoilProtonCalc.Py(), p4RecoilProtonCalc.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function
                    fH_2g_fulexc_p_ExFunc[GetCTBin(cosThetaCM, f2g_fulexc_p_NCTBin)]->Fill(fTaggerPhotonHits[i], subtr_weight);
                    
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok) fH_2g_fulexc_p_ExFunc_W[GetCTBin(cosThetaCMTrue, f2g_fulexc_p_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight);
                    
                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_2g_fulexc_p_ExFunc_W_TOF[GetCTBin(cosThetaCMTrueTOF, f2g_fulexc_p_NCTBin)]->Fill(w_true_tof, fTaggerPhotonHits[i], subtr_weight);
                    
                    // fill phase space
                    Double_t xtup[5] = { fTaggerPhotonHits[i], ekinEta, thetaEta, ekinRecoil, thetaRecoil };
                    fH_2g_fulexc_p_Det->Fill(xtup, subtr_weight);
                }
            }

        } // tagger loop
        
    } // end fully exclusive g + p -> [eta->2g] + p reconstruction
    label_end_2g_fulexc_p:



    ////////////////////////////////////////////////////////////////////////////
    // fully exclusive g + n -> [eta->2g] + n reconstruction                  //
    ////////////////////////////////////////////////////////////////////////////
 
    if (fNNeutral == 3 && fNCharged == 0)
    {
        // reconstruct eta
        TOA2RecEta2g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_2g_fulexc_n;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();
        
        // recoil neutron
        TOA2DetParticle* recoil_neutron = eta.GetNeutronCandidate();
        TLorentzVector p4RecoilNeutron;
        SetP4(p4RecoilNeutron, recoil_neutron->GetEnergy(), TOGlobals::kNeutronMass, 
              recoil_neutron->GetX(), recoil_neutron->GetY(), recoil_neutron->GetZ());
        
        // check software trigger
        if (!IsTrigger(eta)) goto label_end_2g_fulexc_n;
        
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-6.72, 4.38, fH_2g_fulexc_n_Coinc_Time_CB,
        //                                           -1,    1,    fH_2g_fulexc_n_Coinc_Time_TAPS,
        //                                           -4.32, 3.62, fH_2g_fulexc_n_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_2g_fulexc_n;
        
        // get invariant mass, kinetic energy and theta
        TLorentzVector* p4Eta = eta.Get4Vector();
        TLorentzVector* p4EtaConstr = eta.GetConstrained4Vector(TOGlobals::kEtaMass);
        Double_t im = p4Eta->M();
        Double_t ekinEta = p4EtaConstr->E() - p4EtaConstr->M();
        Double_t thetaEta = p4EtaConstr->Theta()*TMath::RadToDeg();

        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 480 && im < 620) imOk = kTRUE;
        
        // check eta-neutron kinematics
        Double_t phi_diff = TMath::Abs(p4EtaConstr->Phi() - p4RecoilNeutron.Phi())*TMath::RadToDeg();
        if (imOk) fH_2g_fulexc_n_PhiDiff_Eta_Recoil->Fill(phi_diff);
        Bool_t copOk = kTRUE;
        //if (phi_diff < 130 || phi_diff > 220) copOk = kFALSE;
        if (phi_diff < 170 || phi_diff > 190) copOk = kFALSE;
  
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kNeutronMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
         
            // calculate kinetic energy of recoil neutron
            Double_t ekinRecoil = TOPhysics::CalcQFThreeBodyRecoilPartT(fTaggerPhotonEnergy[i], p4EtaConstr, &p4RecoilNeutron,
                                                                        TOGlobals::kDeuteronMass, TOGlobals::kNeutronMass, TOGlobals::kProtonMass);
            
            // calculate real 4-vector of recoil neutron
            TLorentzVector p4RecoilNeutronCalc;
            SetP4(p4RecoilNeutronCalc, ekinRecoil, TOGlobals::kNeutronMass, p4RecoilNeutron.X(), p4RecoilNeutron.Y(), p4RecoilNeutron.Z());
            Double_t thetaRecoil = p4RecoilNeutronCalc.Theta() * TMath::RadToDeg();
            
            // calculate kinetic energy of the recoil neutron using TOF
            Double_t ekinRecoilTOF = recoil_neutron->CalculateEkinTOFTagger(TOGlobals::kNeutronMass, fTaggerPhotonTime[i], fIsMC);

            // calculate real 4-vector of recoil neutron using TOF
            TLorentzVector p4RecoilNeutronCalcTOF;
            SetP4(p4RecoilNeutronCalcTOF, ekinRecoilTOF, TOGlobals::kNeutronMass, p4RecoilNeutron.X(), p4RecoilNeutron.Y(), p4RecoilNeutron.Z());
 
            // skip unphysical solutions of the recoil energy calculation
            Bool_t true_cm_ok = kTRUE;
            if (ekinRecoil <= 0 || TMath::IsNaN(ekinRecoil)) true_cm_ok = kFALSE;

            // skip unphysical solutions of the recoil energy calculation using TOF
            Bool_t true_cm_tof_ok = kTRUE;
            if (recoil_neutron->GetDetector() != kTAPSDetector || ekinRecoilTOF <= 0) true_cm_tof_ok = kFALSE;
  
            // missing mass cut
            Double_t mm = (p4Beam + p4Target - *p4EtaConstr).M() - TOGlobals::kNeutronMass;
            //Int_t mm_tagg_element;
            //if (fTaggerPhotonHits[i] < 210) mm_tagg_element = fTaggerPhotonHits[i];
            //else mm_tagg_element = 210;
            Bool_t mmOk = kTRUE;
            if (mm > 0) mmOk = kFALSE;
            //if ((mm < fG_2g_fulexc_n_MM_Min->Eval(mm_tagg_element)) ||
            //    (mm > fG_2g_fulexc_n_MM_Max->Eval(mm_tagg_element))) mmOk = kFALSE;
            
            // fill tagger coincidence time
            if (imOk && mmOk && copOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_2g_fulexc_n_Rand_Time_CB, fH_2g_fulexc_n_Rand_Time_TAPS);

            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -4.5, 4.5, -650, -50, 50, 250,
                                                           -4.5, 3.5, -250, -50, 50, 650);
            
            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
            
            // calculate center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 
  
            // calculate true center-of-mass energy
            TLorentzVector p4CMTrue = p4RecoilNeutronCalc + *p4EtaConstr;
            Double_t w_true = p4CMTrue.M();
            
            // calculate true center-of-mass energy using TOF
            TLorentzVector p4CMTrueTOF = p4RecoilNeutronCalcTOF + *p4EtaConstr;
            Double_t w_true_tof = p4CMTrueTOF.M();
             
            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(*p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();

            // eta 4-vector in true c.m. frame
            TLorentzVector p4EtaCMTrue(*p4EtaConstr);
            p4EtaCMTrue.Boost(-p4CMTrue.BoostVector());
            Double_t cosThetaCMTrue = p4EtaCMTrue.CosTheta();
            
            // eta 4-vector in true c.m. frame using TOF
            TLorentzVector p4EtaCMTrueTOF(*p4EtaConstr);
            p4EtaCMTrueTOF.Boost(-p4CMTrueTOF.BoostVector());
            Double_t cosThetaCMTrueTOF = p4EtaCMTrueTOF.CosTheta();
             
            // apply phase space cut
            Double_t xtup[5] = { fTaggerPhotonHits[i], ekinEta, thetaEta,
                                 (p4Beam + p4Target - *p4EtaConstr).E() - TOGlobals::kNeutronMass,
                                 (p4Beam + p4Target - *p4EtaConstr).Theta() * TMath::RadToDeg()
                               };
            Double_t ps_cont = TOHUtils::GetBinContent(fH_2g_fulexc_n_PhaseSpace, xtup);
            //if (ps_cont == 0 && ctBin != f2g_fulexc_n_NCTBin-1 && ctBinTrue != f2g_fulexc_n_NCTBin-1) continue;
             
            // fill invariant mass
            if (mmOk && copOk)
            {
                fH_2g_fulexc_n_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                if (true_cm_ok) fH_2g_fulexc_n_IM[GetCTBin(cosThetaCM, f2g_fulexc_n_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                if (true_cm_tof_ok) fH_2g_fulexc_n_IM_W_TOF[GetCTBin(cosThetaCMTrueTOF, f2g_fulexc_n_NCTBin)]->Fill(im, w_true_tof, subtr_weight);
            }

            // fill missing mass
            if (imOk && copOk)  
            {
                fH_2g_fulexc_n_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
            }
 
            // fill good events
            if (imOk && mmOk && copOk)
            {
                // check TOF of photons
                for (Int_t j = 0; j < 2; j++) 
                    FillTOF(fH_2g_fulexc_n_Photon_TOF_CB, fH_2g_fulexc_n_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check TOF of recoil neutron
                FillTOF(fH_2g_fulexc_n_Neutron_TOF_CB, fH_2g_fulexc_n_Neutron_TOF_TAPS, recoil_neutron, fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check PSA
                for (Int_t j = 0; j < 2; j++) FillPSA(fH_2g_fulexc_n_Photon_PSA, decay_photons[j], subtr_weight);
                FillPSA(fH_2g_fulexc_n_Neutron_PSA, recoil_neutron, subtr_weight);
                
                // check Fermi momentum
                fH_2g_fulexc_n_Recoil_Fermi->Fill((p4Beam.Vect() - p4RecoilNeutronCalc.Vect() - p4EtaConstr->Vect()).Mag(), subtr_weight);
  
                // MC efficiency determination 
                if (fCreateEta_n_Eff) 
                {
                    // excitation function
                    fH_2g_fulexc_n_ExFunc[GetCTBin(cosThetaCM_MC, f2g_fulexc_n_NCTBin)]->Fill(fTaggerPhotonHits[i]);
  
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok) fH_2g_fulexc_n_ExFunc_W[GetCTBin(cosThetaCMTrue_MC, f2g_fulexc_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                    
                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_ok) fH_2g_fulexc_n_ExFunc_W_TOF[GetCTBin(cosThetaCMTrue_MC, f2g_fulexc_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                      
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated eta     : %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected eta      : %f %f %f %f\n", p4EtaConstr->E(), p4EtaConstr->Px(), p4EtaConstr->Py(), p4EtaConstr->Pz());
                        printf("generated neutron : %f %f %f %f\n", p4RecoilMC->E(), p4RecoilMC->Px(), p4RecoilMC->Py(), p4RecoilMC->Pz());
                        printf("detected neutron  : %f %f %f %f\n", p4RecoilNeutronCalc.E(), p4RecoilNeutronCalc.Px(), p4RecoilNeutronCalc.Py(), p4RecoilNeutronCalc.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function
                    fH_2g_fulexc_n_ExFunc[GetCTBin(cosThetaCM, f2g_fulexc_n_NCTBin)]->Fill(fTaggerPhotonHits[i], subtr_weight);
                    
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok) fH_2g_fulexc_n_ExFunc_W[GetCTBin(cosThetaCMTrue, f2g_fulexc_n_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight);
                    
                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_2g_fulexc_n_ExFunc_W_TOF[GetCTBin(cosThetaCMTrueTOF, f2g_fulexc_n_NCTBin)]->Fill(w_true_tof, fTaggerPhotonHits[i], subtr_weight);
                    // fill phase space
                    Double_t xtup[5] = { fTaggerPhotonHits[i], ekinEta, thetaEta, ekinRecoil, thetaRecoil };
                    fH_2g_fulexc_n_Det->Fill(xtup, subtr_weight);
                }
            }

        } // tagger loop

    } // end fully exclusive g + n -> [eta->2g] + n reconstruction
    label_end_2g_fulexc_n:


    /*          
    ////////////////////////////////////////////////////////////////////////////
    // fully inclusive eta -> 6g + X reconstruction                           //
    ////////////////////////////////////////////////////////////////////////////
 
    if (fNNeutral == 6 || fNNeutral == 7 || fNNeutral == 8 || fNNeutral == 9)
    {
        // accept only 6 clusters from the simulation
        if (fIsMC && fNNeutral != 6) goto label_end_6g_fulinc;

        // reconstruct eta
        TOA2RecEta6g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_6g_fulinc;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();

        // check software trigger
        if (!IsTrigger(eta)) goto label_end_6g_fulinc;
        
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-5.74, 5.04, fH_6g_fulinc_Coinc_Time_CB,
        //                                           -1.16, 1.14, fH_6g_fulinc_Coinc_Time_TAPS,
        //                                           -1.39, 3.89, fH_6g_fulinc_Coinc_Time_CB_TAPS);
        
        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_6g_fulinc;
        
        // check PSA of decay photons
        //Bool_t psaOk = kTRUE;
        //for (Int_t i = 0; i < 6; i++) 
        //{
        //    Double_t psaR, psaA;
        //    if (GetPSA(decay_photons[i], &psaR, &psaA))
        //        if (psaA < 40) psaOk = kFALSE;
        //}
        //if (!psaOk) goto label_end_6g_fulinc;

        // get invariant mass
        TLorentzVector* p4EtaRaw = eta.Get4Vector();
        Double_t imRaw = p4EtaRaw->M();
        fH_6g_fulinc_IM_Debug[0]->Fill(imRaw);
        
        // check intermediate pi0
        TOA2RecParticle** intermed_pi0 = eta.GetIntermedProducts();
        TLorentzVector* p4Pion1Raw = intermed_pi0[0]->Get4Vector();
        TLorentzVector* p4Pion2Raw = intermed_pi0[1]->Get4Vector();
        TLorentzVector* p4Pion3Raw = intermed_pi0[2]->Get4Vector();
        fH_6g_fulinc_IM_Debug[1]->Fill((*p4Pion1Raw + *p4Pion2Raw + *p4Pion3Raw).M());
        
        fH_6g_fulinc_IM_Debug[2]->Fill(p4Pion1Raw->M());
        fH_6g_fulinc_IM_Debug[2]->Fill(p4Pion2Raw->M());
        fH_6g_fulinc_IM_Debug[2]->Fill(p4Pion3Raw->M());
        
        // intermediate pi0 invariant mass cut
        if (!eta.IntermedProductsFulFillIMCut(110, 160)) goto label_end_6g_fulinc;

        fH_6g_fulinc_IM_Debug[3]->Fill(imRaw);
        fH_6g_fulinc_IM_Debug[4]->Fill(p4Pion1Raw->M());
        fH_6g_fulinc_IM_Debug[4]->Fill(p4Pion2Raw->M());
        fH_6g_fulinc_IM_Debug[4]->Fill(p4Pion3Raw->M());
        
        // constrain pi0 4-vectors and calculate new eta 4-vector
        TLorentzVector* p4Pion1 = intermed_pi0[0]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion2 = intermed_pi0[1]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion3 = intermed_pi0[2]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector p4Eta = *p4Pion1 + *p4Pion2 + *p4Pion3;
        TLorentzVector p4EtaConstr(p4Eta);
        p4EtaConstr *= TOGlobals::kEtaMass / p4Eta.M();
        
        // get invariant mass, kinetic energy and theta
        Double_t im = p4Eta.M();
        Double_t ekin = p4EtaConstr.E() - p4EtaConstr.M();
        Double_t theta = p4EtaConstr.Theta()*TMath::RadToDeg();
        fH_6g_fulinc_IM_Debug[5]->Fill(im);
        
        // get detection efficiency
        Int_t binX = fH_6g_fulinc_DetEff->GetXaxis()->FindBin(ekin);
        Int_t binY = fH_6g_fulinc_DetEff->GetYaxis()->FindBin(theta);
        Double_t eff;
        if (!fIsMC) eff = fH_6g_fulinc_DetEff->GetBinContent(binX, binY);
        else eff = 1.;
        if (eff == 0) goto label_end_6g_fulinc;

        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 460 && im < 620) imOk = kTRUE;
       
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -3.0,  6.0, -650, -50, 50, 250,
                                                           -10.0, 5.0, -250, -50, 50, 650);
            
            // fill tagger coincidence time
            if (imOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_6g_fulinc_Rand_Time_CB, fH_6g_fulinc_Rand_Time_TAPS);
            
            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
 
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
            
            // missing mass
            Double_t mm = (p4Beam + p4Target - p4EtaConstr).M() - TOGlobals::kProtonMass;
            
            // center-of-mass state 4-vector
            TLorentzVector p4CM = p4Beam + p4Target; 

            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();

            // fill invariant mass
            fH_6g_fulinc_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight / eff);
            fH_6g_fulinc_IM_E->Fill(im, ekin, subtr_weight);
            fH_6g_fulinc_IM[GetCTBin(cosThetaCM, f6g_fulinc_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight / eff);

            // invariant mass cut
            if (imOk)  
            {
                // missing mass
                fH_6g_fulinc_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
                
                // kinetic energy
                fH_6g_fulinc_E[GetCTBin(cosThetaCM, f6g_fulinc_NCTBin)]->Fill(ekin, fTaggerPhotonHits[i], subtr_weight);
                
                // check TOF of photons
                for (Int_t j = 0; j < 6; j++) 
                    FillTOF(fH_6g_fulinc_Photon_TOF_CB, fH_6g_fulinc_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);

                // check PSA of photons
                for (Int_t j = 0; j < 6; j++) FillPSA(fH_6g_fulinc_Photon_PSA, decay_photons[j], subtr_weight);
 
                // excitation function
                FillExFunc(fTaggerPhotonHits[i], fH_6g_fulinc_ExFunc[GetCTBin(cosThetaCM, f6g_fulinc_NCTBin)], subtr_weight, ekin, theta, fH_6g_fulinc_DetEff);

                // eta energy and theta: fill for efficiency determination or normal analysis
                if (fCreateEtaGridEff) 
                {
                    fH_6g_fulinc_Eta_E_Theta->Fill(p4EtaMC->E() - p4EtaMC->M(), p4EtaMC->Theta()*TMath::RadToDeg());
                    
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 30)
                    {
                        printf("generated: %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected : %f %f %f %f\n", p4EtaConstr.E(), p4EtaConstr.Px(), p4EtaConstr.Py(), p4EtaConstr.Pz());
                        printf("\n");
                    }
                }
                else fH_6g_fulinc_Eta_E_Theta->Fill(ekin, theta, subtr_weight);
            }

        } // tagger loop

    } // end fully inclusive eta -> 6g reconstruction
    label_end_6g_fulinc:

    

    ////////////////////////////////////////////////////////////////////////////
    // q.f. inclusive g + X -> [eta->6g] + X reconstruction                   //
    ////////////////////////////////////////////////////////////////////////////
 
    if ((fNNeutral == 6 && fNCharged == 0) ||
        (fNNeutral == 6 && fNCharged == 1) ||
        (fNNeutral == 7 && fNCharged == 0))

    {
        // reconstruct eta
        TOA2RecEta6g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_6g_qfinc;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();
        
        // recoil proton
        TOA2DetParticle* recoil_proton = 0;
        if (fNCharged == 1) recoil_proton = fPartCharged[0];

        // recoil neutron
        TOA2DetParticle* recoil_neutron = 0;
        if (fNNeutral == 7) recoil_neutron = eta.GetNeutronCandidate();

        // check software trigger
        if (!IsTrigger(eta)) goto label_end_6g_qfinc;
        
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-5.93, 5.20, fH_6g_qfinc_Coinc_Time_CB,
        //                                           -1.16, 1.14, fH_6g_qfinc_Coinc_Time_TAPS,
        //                                           -0.84, 3.49, fH_6g_qfinc_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_6g_qfinc;

        // get invariant mass
        TLorentzVector* p4EtaRaw = eta.Get4Vector();
        Double_t imRaw = p4EtaRaw->M();
        fH_6g_qfinc_IM_Debug[0]->Fill(imRaw);
        
        // check intermediate pi0
        TOA2RecParticle** intermed_pi0 = eta.GetIntermedProducts();
        TLorentzVector* p4Pion1Raw = intermed_pi0[0]->Get4Vector();
        TLorentzVector* p4Pion2Raw = intermed_pi0[1]->Get4Vector();
        TLorentzVector* p4Pion3Raw = intermed_pi0[2]->Get4Vector();
        fH_6g_qfinc_IM_Debug[1]->Fill((*p4Pion1Raw + *p4Pion2Raw + *p4Pion3Raw).M());
        
        fH_6g_qfinc_IM_Debug[2]->Fill(p4Pion1Raw->M());
        fH_6g_qfinc_IM_Debug[2]->Fill(p4Pion2Raw->M());
        fH_6g_qfinc_IM_Debug[2]->Fill(p4Pion3Raw->M());
        
        // intermediate pi0 invariant mass cut
        if (!eta.IntermedProductsFulFillIMCut(110, 160)) goto label_end_6g_qfinc;

        fH_6g_qfinc_IM_Debug[3]->Fill(imRaw);
        fH_6g_qfinc_IM_Debug[4]->Fill(p4Pion1Raw->M());
        fH_6g_qfinc_IM_Debug[4]->Fill(p4Pion2Raw->M());
        fH_6g_qfinc_IM_Debug[4]->Fill(p4Pion3Raw->M());
         
        // constrain pi0 4-vectors and calculate new eta 4-vector
        TLorentzVector* p4Pion1 = intermed_pi0[0]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion2 = intermed_pi0[1]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion3 = intermed_pi0[2]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector p4Eta = *p4Pion1 + *p4Pion2 + *p4Pion3;
        TLorentzVector p4EtaConstr(p4Eta);
        p4EtaConstr *= TOGlobals::kEtaMass / p4Eta.M();
        
        // get invariant mass, kinetic energy and theta
        Double_t im = p4Eta.M();
        fH_6g_qfinc_IM_Debug[5]->Fill(im);
        
        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 510 && im < 580) imOk = kTRUE;
  
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -3.0,  6.0, -650, -50, 50, 250,
                                                           -10.0, 5.0, -250, -50, 50, 650);
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
           
            // center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 

            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();

            // missing mass cut
            Double_t mm = (p4Beam + p4Target - p4EtaConstr).M() - TOGlobals::kProtonMass;
            Bool_t mmOk = kTRUE;
            //Int_t mm_fTaggerPhotonHits[i];
            //if (fTaggerPhotonHits[i] < 210) mm_fTaggerPhotonHits[i] = fTaggerPhotonHits[i];
            //else mm_fTaggerPhotonHits[i] = 210;
            //if ((mm < fG_6g_qfinc_MM_Min->Eval(mm_fTaggerPhotonHits[i])) ||
            //    (mm > fG_6g_qfinc_MM_Max->Eval(mm_fTaggerPhotonHits[i]))) mmOk = kFALSE;
            if (mm > 0) mmOk = kFALSE;

            // fill tagger coincidence time
            if (imOk && mmOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_6g_qfinc_Rand_Time_CB, fH_6g_qfinc_Rand_Time_TAPS);

            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
            
            // fill invariant mass
            if (mmOk)
            {
                fH_6g_qfinc_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                fH_6g_qfinc_IM[GetCTBin(cosThetaCM, f6g_qfinc_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight);
            }

            // fill missing mass
            if (imOk)  
            {
                fH_6g_qfinc_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
            }

            // fill good events
            if (imOk && mmOk)
            {
                // check TOF of photons
                for (Int_t j = 0; j < 6; j++)
                    FillTOF(fH_6g_qfinc_Photon_TOF_CB, fH_6g_qfinc_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);

                // check PSA of photons
                for (Int_t j = 0; j < 6; j++) FillPSA(fH_6g_qfinc_Photon_PSA, decay_photons[j], subtr_weight);
                
                // MC efficiency determination 
                if (fCreateEta_p_Eff || fCreateEta_n_Eff) 
                {
                    // excitation function
                    fH_6g_qfinc_ExFunc[GetCTBin(cosThetaCM_MC, f6g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[i]);
                    
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated eta   : %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected eta    : %f %f %f %f\n", p4EtaConstr.E(), p4EtaConstr.Px(), p4EtaConstr.Py(), p4EtaConstr.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function
                    fH_6g_qfinc_ExFunc[GetCTBin(cosThetaCM, f6g_qfinc_NCTBin)]->Fill(fTaggerPhotonHits[i], subtr_weight);
                }
            }

        } // tagger loop

    } // end q.f. inclusive g + X -> [eta->6g] + X reconstruction
    label_end_6g_qfinc:



    ////////////////////////////////////////////////////////////////////////////
    // fully exclusive g + p -> [eta->6g] + p reconstruction                  //
    ////////////////////////////////////////////////////////////////////////////
 
    if (fNNeutral == 6 && fNCharged == 1)
    {
        // reconstruct eta
        TOA2RecEta6g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_6g_fulexc_p;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();
  
        // recoil proton
        TOA2DetParticle* recoil_proton = fPartCharged[0];
        TLorentzVector p4RecoilProton;
        SetP4(p4RecoilProton, recoil_proton->GetEnergy(), TOGlobals::kProtonMass, 
              recoil_proton->GetX(), recoil_proton->GetY(), recoil_proton->GetZ());

        // check software trigger
        if (!IsTrigger(eta)) goto label_end_6g_fulexc_p;
 
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-4.78, 4.18, fH_6g_fulexc_p_Coinc_Time_CB,
        //                                           -1.13, 1.12, fH_6g_fulexc_p_Coinc_Time_TAPS,
        //                                           -0.86, 3.55, fH_6g_fulexc_p_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_6g_fulexc_p;

        // get invariant mass
        TLorentzVector* p4EtaRaw = eta.Get4Vector();
        Double_t imRaw = p4EtaRaw->M();
        fH_6g_fulexc_p_IM_Debug[0]->Fill(imRaw);
        
        // check intermediate pi0
        TOA2RecParticle** intermed_pi0 = eta.GetIntermedProducts();
        TLorentzVector* p4Pion1Raw = intermed_pi0[0]->Get4Vector();
        TLorentzVector* p4Pion2Raw = intermed_pi0[1]->Get4Vector();
        TLorentzVector* p4Pion3Raw = intermed_pi0[2]->Get4Vector();
        fH_6g_fulexc_p_IM_Debug[1]->Fill((*p4Pion1Raw + *p4Pion2Raw + *p4Pion3Raw).M());
        
        fH_6g_fulexc_p_IM_Debug[2]->Fill(p4Pion1Raw->M());
        fH_6g_fulexc_p_IM_Debug[2]->Fill(p4Pion2Raw->M());
        fH_6g_fulexc_p_IM_Debug[2]->Fill(p4Pion3Raw->M());
        
        // intermediate pi0 invariant mass cut
        if (!eta.IntermedProductsFulFillIMCut(110, 160)) goto label_end_6g_fulexc_p;

        fH_6g_fulexc_p_IM_Debug[3]->Fill(imRaw);
        fH_6g_fulexc_p_IM_Debug[4]->Fill(p4Pion1Raw->M());
        fH_6g_fulexc_p_IM_Debug[4]->Fill(p4Pion2Raw->M());
        fH_6g_fulexc_p_IM_Debug[4]->Fill(p4Pion3Raw->M());
         
        // constrain pi0 4-vectors and calculate new eta 4-vector
        TLorentzVector* p4Pion1 = intermed_pi0[0]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion2 = intermed_pi0[1]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion3 = intermed_pi0[2]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector p4Eta = *p4Pion1 + *p4Pion2 + *p4Pion3;
        TLorentzVector p4EtaConstr(p4Eta);
        p4EtaConstr *= TOGlobals::kEtaMass / p4Eta.M();
        
        // get invariant mass, kinetic energy and theta
        Double_t im = p4Eta.M();
        fH_6g_fulexc_p_IM_Debug[5]->Fill(im);
               
        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 510 && im < 580) imOk = kTRUE;
        
        // check eta-proton kinematics
        Double_t phi_diff = TMath::Abs(p4EtaConstr.Phi() - p4RecoilProton.Phi())*TMath::RadToDeg();
        if (imOk) fH_6g_fulexc_p_PhiDiff_Eta_Recoil->Fill(phi_diff);
        Bool_t copOk = kTRUE;
        if (phi_diff < 130 || phi_diff > 220) copOk = kFALSE;
        //if (phi_diff < 170 || phi_diff > 190) copOk = kFALSE;
         
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -3.0,  6.0, -650, -50, 50, 250,
                                                           -10.0, 5.0, -250, -50, 50, 650);

            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
            
            // center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 

            // calculate kinetic energy of recoil proton
            Double_t rec_ekin = TOPhysics::CalcQFThreeBodyRecoilPartT(fTaggerPhotonEnergy[i], &p4EtaConstr, &p4RecoilProton,
                                                                      TOGlobals::kDeuteronMass, TOGlobals::kProtonMass, TOGlobals::kNeutronMass);
            
            // calculate real 4-vector of recoil proton
            TLorentzVector p4RecoilProtonCalc;
            SetP4(p4RecoilProtonCalc, rec_ekin, TOGlobals::kProtonMass, p4RecoilProton.X(), p4RecoilProton.Y(), p4RecoilProton.Z());
 
            // calculate true center-of-mass energy
            TLorentzVector p4CMTrue = p4RecoilProtonCalc + p4EtaConstr;
            Double_t w_true = p4CMTrue.M();
  
            // set kinematical cuts due to detection efficiency
            Bool_t true_cm_ok = kTRUE;
            if (rec_ekin <= 0 || TMath::IsNaN(rec_ekin)) true_cm_ok = kFALSE;
            
            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();

            // eta 4-vector in true c.m. frame
            TLorentzVector p4EtaCMTrue(p4EtaConstr);
            p4EtaCMTrue.Boost(-p4CMTrue.BoostVector());
            Double_t cosThetaCMTrue = p4EtaCMTrue.CosTheta();

            // missing mass cut
            Double_t mm = (p4Beam + p4Target - p4EtaConstr).M() - TOGlobals::kProtonMass;
            Bool_t mmOk = kTRUE;
            //Int_t mm_fTaggerPhotonHits[i];
            //if (fTaggerPhotonHits[i] < 210) mm_fTaggerPhotonHits[i] = fTaggerPhotonHits[i];
            //else mm_fTaggerPhotonHits[i] = 210;
            //if ((mm < fG_6g_fulexc_p_MM_Min->Eval(mm_fTaggerPhotonHits[i])) ||
            //    (mm > fG_6g_fulexc_p_MM_Max->Eval(mm_fTaggerPhotonHits[i]))) mmOk = kFALSE;
            if (mm > 0) mmOk = kFALSE;

            // calculate fermi momentum of initial state proton
            if (true_cm_ok && imOk && mmOk && copOk) fH_6g_fulexc_p_Recoil_Fermi->Fill((p4CMTrue - p4Beam).P());
  
            // fill tagger coincidence time
            if (imOk && mmOk && copOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_6g_fulexc_p_Rand_Time_CB, fH_6g_fulexc_p_Rand_Time_TAPS);

            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
  
            // fill invariant mass
            if (mmOk && copOk)
            {
                fH_6g_fulexc_p_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                fH_6g_fulexc_p_IM[GetCTBin(cosThetaCM, f6g_fulexc_p_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                if (true_cm_ok) fH_6g_fulexc_p_IM_W[GetCTBin(cosThetaCMTrue, f6g_fulexc_p_NCTBin)]->Fill(im, w_true, subtr_weight);
            }

            // fill missing mass
            if (imOk && copOk)  
            {
                fH_6g_fulexc_p_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
            }

            // fill good events
            if (imOk && mmOk && copOk)
            {
                // check TOF of photons
                for (Int_t j = 0; j < 6; j++) 
                    FillTOF(fH_6g_fulexc_p_Photon_TOF_CB, fH_6g_fulexc_p_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check TOF and dE/E of recoil proton
                FillTOF(fH_6g_fulexc_p_Proton_TOF_CB, fH_6g_fulexc_p_Proton_TOF_TAPS, recoil_proton, fTaggerPhotonTime[i], fIsMC, subtr_weight);
                FilldE_E(fH_6g_fulexc_p_Proton_dE_E_CB, fH_6g_fulexc_p_Proton_dE_E_TAPS, recoil_proton, subtr_weight);

                // check PSA
                for (Int_t j = 0; j < 6; j++) FillPSA(fH_6g_fulexc_p_Photon_PSA, decay_photons[j], subtr_weight);
                FillPSA(fH_6g_fulexc_p_Proton_PSA, recoil_proton, subtr_weight);
                
                // MC efficiency determination 
                if (fCreateEta_p_Eff) 
                {
                    // excitation function
                    fH_6g_fulexc_p_ExFunc[GetCTBin(cosThetaCM_MC, f6g_fulexc_p_NCTBin)]->Fill(fTaggerPhotonHits[i]);
                    
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_6g_fulexc_p_ExFunc_W[GetCTBin(cosThetaCMTrue_MC, f6g_fulexc_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                    }
                     
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated eta    : %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected eta     : %f %f %f %f\n", p4EtaConstr.E(), p4EtaConstr.Px(), p4EtaConstr.Py(), p4EtaConstr.Pz());
                        printf("generated proton : %f %f %f %f\n", p4RecoilMC->E(), p4RecoilMC->Px(), p4RecoilMC->Py(), p4RecoilMC->Pz());
                        printf("detected proton  : %f %f %f %f\n", p4RecoilProtonCalc.E(), p4RecoilProtonCalc.Px(), p4RecoilProtonCalc.Py(), p4RecoilProtonCalc.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function
                    fH_6g_fulexc_p_ExFunc[GetCTBin(cosThetaCM, f6g_fulexc_p_NCTBin)]->Fill(fTaggerPhotonHits[i], subtr_weight);
                    
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_6g_fulexc_p_ExFunc_W[GetCTBin(cosThetaCMTrue, f6g_fulexc_p_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight);
                    }
                }
            }

        } // tagger loop

    } // end fully exclusive g + p -> [eta->6g] + p reconstruction
    label_end_6g_fulexc_p:
    


    ////////////////////////////////////////////////////////////////////////////
    // fully exclusive g + n -> [eta->6g] + n reconstruction                  //
    ////////////////////////////////////////////////////////////////////////////
 
    if (fNNeutral == 7 && fNCharged == 0)
    {
        // reconstruct eta
        TOA2RecEta6g eta(fNNeutral);
        if (!eta.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_6g_fulexc_n;
        TOA2DetParticle** decay_photons = eta.GetDetectedProducts();

        // recoil neutron 
        TOA2DetParticle* recoil_neutron = eta.GetNeutronCandidate();
        TLorentzVector p4RecoilNeutron;
        SetP4(p4RecoilNeutron, recoil_neutron->GetEnergy(), TOGlobals::kNeutronMass, 
              recoil_neutron->GetX(), recoil_neutron->GetY(), recoil_neutron->GetZ());
 
        // check software trigger
        if (!IsTrigger(eta)) goto label_end_6g_fulexc_n;
  
        // save event in reduced file
        fSaveEvent = 1;
        
        // check decay photons coincidence
        //Bool_t coincDecProd = eta.CheckCoincidence(-5.91, 5.22, fH_6g_fulexc_n_Coinc_Time_CB,
        //                                           -1.11, 1.10, fH_6g_fulexc_n_Coinc_Time_TAPS,
        //                                           -1.68, 3.99, fH_6g_fulexc_n_Coinc_Time_CB_TAPS);

        // leave if decay photons not coincident and if data is not MC
        //if (!coincDecProd && !fIsMC) goto label_end_6g_fulexc_n;

        // get invariant mass
        TLorentzVector* p4EtaRaw = eta.Get4Vector();
        Double_t imRaw = p4EtaRaw->M();
        fH_6g_fulexc_n_IM_Debug[0]->Fill(imRaw);
        
        // check intermediate pi0
        TOA2RecParticle** intermed_pi0 = eta.GetIntermedProducts();
        TLorentzVector* p4Pion1Raw = intermed_pi0[0]->Get4Vector();
        TLorentzVector* p4Pion2Raw = intermed_pi0[1]->Get4Vector();
        TLorentzVector* p4Pion3Raw = intermed_pi0[2]->Get4Vector();
        fH_6g_fulexc_n_IM_Debug[1]->Fill((*p4Pion1Raw + *p4Pion2Raw + *p4Pion3Raw).M());
        
        fH_6g_fulexc_n_IM_Debug[2]->Fill(p4Pion1Raw->M());
        fH_6g_fulexc_n_IM_Debug[2]->Fill(p4Pion2Raw->M());
        fH_6g_fulexc_n_IM_Debug[2]->Fill(p4Pion3Raw->M());
        
        // intermediate pi0 invariant mass cut
        if (!eta.IntermedProductsFulFillIMCut(110, 160)) goto label_end_6g_fulexc_n;

        fH_6g_fulexc_n_IM_Debug[3]->Fill(imRaw);
        fH_6g_fulexc_n_IM_Debug[4]->Fill(p4Pion1Raw->M());
        fH_6g_fulexc_n_IM_Debug[4]->Fill(p4Pion2Raw->M());
        fH_6g_fulexc_n_IM_Debug[4]->Fill(p4Pion3Raw->M());
         
        // constrain pi0 4-vectors and calculate new eta 4-vector
        TLorentzVector* p4Pion1 = intermed_pi0[0]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion2 = intermed_pi0[1]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector* p4Pion3 = intermed_pi0[2]->GetConstrained4Vector(TOGlobals::kPi0Mass);
        TLorentzVector p4Eta = *p4Pion1 + *p4Pion2 + *p4Pion3;
        TLorentzVector p4EtaConstr(p4Eta);
        p4EtaConstr *= TOGlobals::kEtaMass / p4Eta.M();
        
        // get invariant mass, kinetic energy and theta
        Double_t im = p4Eta.M();
        fH_6g_fulexc_n_IM_Debug[5]->Fill(im);
        
        // invariant mass cut
        Bool_t imOk = kFALSE;
        if (im > 510 && im < 580) imOk = kTRUE;
        
        // check eta-neutron kinematics
        Double_t phi_diff = TMath::Abs(p4EtaConstr.Phi() - p4RecoilNeutron.Phi())*TMath::RadToDeg();
        if (imOk) fH_6g_fulexc_n_PhiDiff_Eta_Recoil->Fill(phi_diff);
        Bool_t copOk = kTRUE;
        if (phi_diff < 130 || phi_diff > 220) copOk = kFALSE;
        //if (phi_diff < 170 || phi_diff > 190) copOk = kFALSE;
         
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kNeutronMass);
        
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(eta, fTaggerPhotonTime[i], 
                                                           -3.0,  6.0, -650, -50, 50, 250,
                                                           -10.0, 5.0, -250, -50, 50, 650);
            
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
 
            // center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target; 

            // calculate kinetic energy of recoil neutron
            Double_t rec_ekin = TOPhysics::CalcQFThreeBodyRecoilPartT(fTaggerPhotonEnergy[i], &p4EtaConstr, &p4RecoilNeutron,
                                                                      TOGlobals::kDeuteronMass, TOGlobals::kNeutronMass, TOGlobals::kProtonMass);
            
            // calculate real 4-vector of recoil neutron
            TLorentzVector p4RecoilNeutronCalc;
            SetP4(p4RecoilNeutronCalc, rec_ekin, TOGlobals::kNeutronMass, p4RecoilNeutron.X(), p4RecoilNeutron.Y(), p4RecoilNeutron.Z());
 
            // calculate center-of-mass energy
            TLorentzVector p4CMTrue = p4RecoilNeutronCalc + p4EtaConstr;
            Double_t w_true = p4CMTrue.M();
            
            // set kinematical cuts due to detection efficiency
            Bool_t true_cm_ok = kTRUE;
            if (rec_ekin <= 0 || TMath::IsNaN(rec_ekin)) true_cm_ok = kFALSE;

            // eta 4-vector in c.m. frame
            TLorentzVector p4EtaCM(p4EtaConstr);
            p4EtaCM.Boost(-p4CM.BoostVector());
            Double_t cosThetaCM = p4EtaCM.CosTheta();

            // eta 4-vector in true c.m. frame
            TLorentzVector p4EtaCMTrue(p4EtaConstr);
            p4EtaCMTrue.Boost(-p4CMTrue.BoostVector());
            Double_t cosThetaCMTrue = p4EtaCMTrue.CosTheta();

            // missing mass cut
            Double_t mm = (p4Beam + p4Target - p4EtaConstr).M() - TOGlobals::kNeutronMass;
            Bool_t mmOk = kTRUE;
            //Int_t mm_fTaggerPhotonHits[i];
            //if (fTaggerPhotonHits[i] < 210) mm_fTaggerPhotonHits[i] = fTaggerPhotonHits[i];
            //else mm_fTaggerPhotonHits[i] = 210;
            //if ((mm < fG_6g_fulexc_n_MM_Min->Eval(mm_fTaggerPhotonHits[i])) ||
            //    (mm > fG_6g_fulexc_n_MM_Max->Eval(mm_fTaggerPhotonHits[i]))) mmOk = kFALSE;
            if (mm > 0) mmOk = kFALSE;

            // calculate fermi momentum of initial state neutron
            if (true_cm_ok && imOk && mmOk && copOk) fH_6g_fulexc_n_Recoil_Fermi->Fill((p4CMTrue - p4Beam).P());
            
            // fill tagger coincidence time
            if (imOk && mmOk && copOk) FillTaggerCoincidence(eta, fTaggerPhotonTime[i], fH_6g_fulexc_n_Rand_Time_CB, fH_6g_fulexc_n_Rand_Time_TAPS);

            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
            
            // fill invariant mass
            if (mmOk && copOk)
            {
                fH_6g_fulexc_n_IM_Tot->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                fH_6g_fulexc_n_IM[GetCTBin(cosThetaCM, f6g_fulexc_n_NCTBin)]->Fill(im, fTaggerPhotonHits[i], subtr_weight);
                if (true_cm_ok) fH_6g_fulexc_n_IM_W[ctBinTrue = GetCTBin(cosThetaCMTrue, f6g_fulexc_n_NCTBin)]->Fill(im, w_true, subtr_weight);
            }

            // fill missing mass
            if (imOk && copOk)  
            {
                fH_6g_fulexc_n_MM_Tot->Fill(mm, fTaggerPhotonHits[i], subtr_weight);
            }

            // fill good events
            if (imOk && mmOk && copOk)
            {
                // check TOF of photons
                for (Int_t j = 0; j < 6; j++) 
                    FillTOF(fH_6g_fulexc_n_Photon_TOF_CB, fH_6g_fulexc_n_Photon_TOF_TAPS, decay_photons[j], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check TOF of recoil neutron
                FillTOF(fH_6g_fulexc_n_Neutron_TOF_CB, fH_6g_fulexc_n_Neutron_TOF_TAPS, recoil_neutron, fTaggerPhotonTime[i], fIsMC, subtr_weight);

                // check PSA
                for (Int_t j = 0; j < 6; j++) FillPSA(fH_6g_fulexc_n_Photon_PSA, decay_photons[j], subtr_weight);
                FillPSA(fH_6g_fulexc_n_Neutron_PSA, recoil_neutron, subtr_weight);
                
                // MC efficiency determination 
                if (fCreateEta_n_Eff) 
                {
                    // excitation function
                    fH_6g_fulexc_n_ExFunc[GetCTBin(cosThetaCM_MC, f6g_fulexc_n_NCTBin)]->Fill(fTaggerPhotonHits[i]);
                    
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_6g_fulexc_n_ExFunc_W[GetCTBin(cosThetaCMTrue_MC, f6g_fulexc_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                    }
                     
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 500)
                    {
                        printf("generated eta     : %f %f %f %f\n", p4EtaMC->E(), p4EtaMC->Px(), p4EtaMC->Py(), p4EtaMC->Pz());
                        printf("detected eta      : %f %f %f %f\n", p4EtaConstr.E(), p4EtaConstr.Px(), p4EtaConstr.Py(), p4EtaConstr.Pz());
                        printf("generated neutron : %f %f %f %f\n", p4RecoilMC->E(), p4RecoilMC->Px(), p4RecoilMC->Py(), p4RecoilMC->Pz());
                        printf("detected neutron  : %f %f %f %f\n", p4RecoilNeutronCalc.E(), p4RecoilNeutronCalc.Px(), p4RecoilNeutronCalc.Py(), p4RecoilNeutronCalc.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function
                    fH_6g_fulexc_n_ExFunc[GetCTBin(cosThetaCM, f6g_fulexc_n_NCTBin)]->Fill(fTaggerPhotonHits[i], subtr_weight);
                    
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_6g_fulexc_n_ExFunc_W[ctBinTrue = GetCTBin(cosThetaCMTrue, f6g_fulexc_n_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight);
                    }
                }
            }

        } // tagger loop

    } // end fully exclusive g + n -> [eta->6g] + n reconstruction
    label_end_6g_fulexc_n:
    */


    ////////////////////////////////////////////////////////////////////////////
    // exclusive quasi-free compton scattering g + p -> g + p reconstruction  //
    ////////////////////////////////////////////////////////////////////////////
  
    if (fNNeutral == 1 && fNCharged == 1)
    {
        // check detectors
        if (fPartNeutral[0]->GetDetector() != kCBDetector) goto label_end_Comp_p;
        if (fPartCharged[0]->GetDetector() != kTAPSDetector) goto label_end_Comp_p;
        
        // compton gamma
        TOA2RecParticle gamma(1);
        if (!gamma.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_Comp_p;
        TLorentzVector* p4Gamma = gamma.Get4Vector();
        Double_t thetaGamma = p4Gamma->Theta() * TMath::RadToDeg();

        // recoil proton
        TOA2DetParticle* recoil_proton = fPartCharged[0];
        TLorentzVector p4RecoilProton;
        SetP4(p4RecoilProton, recoil_proton->GetEnergy(), TOGlobals::kProtonMass, 
              recoil_proton->GetX(), recoil_proton->GetY(), recoil_proton->GetZ());
        
        // save event in reduced file
        fSaveEvent = 1;
  
        // check software trigger
        if (!IsTrigger(gamma)) goto label_end_Comp_p;
 
        // cut on clean photons at backward angles
        if (thetaGamma < 90 || thetaGamma > 150) goto label_end_Comp_p;
 
        // remove background by requesting no signals in the other detectors
        // no hit in the PID
        if (fPIDNhits > 0) goto label_end_Comp_p;
        
        // no Veto hits other than the hits in the cluster center and its neighbours
        if (GetForeignVetoHits(recoil_proton) > 0) goto label_end_Comp_p;
    
        // get invariant mass for background reduction
        TLorentzVector p4FalseGamma;
        SetP4(p4FalseGamma, recoil_proton->GetEnergy(), 0, 
              recoil_proton->GetX(), recoil_proton->GetY(), recoil_proton->GetZ());
        Double_t im = (*p4Gamma + p4FalseGamma).M();

        // invariant mass cut
        Bool_t imOk = kTRUE;
        //if (im < 160) imOk = kFALSE;
        
        // check gamma-proton kinematics
        Double_t phi_diff = TMath::Abs(p4Gamma->Phi() - p4RecoilProton.Phi())*TMath::RadToDeg();
        Bool_t copOk = kTRUE;
        if (phi_diff < 170 || phi_diff > 190) copOk = kFALSE;
        
        //fprintf(outText, "%10f  %10f  %10d  %10f\n", p4Gamma->E(), thetaGamma, fPartNeutral[0]->GetClusterSize(),
        //                                             p4RecoilProton.Theta()*TMath::RadToDeg());
  
        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
  
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
  
            // calculate kinetic energy of recoil proton
            Double_t ekinRecoil = TOPhysics::CalcQFThreeBodyRecoilPartT(fTaggerPhotonEnergy[i], p4Gamma, &p4RecoilProton,
                                                                        TOGlobals::kDeuteronMass, TOGlobals::kProtonMass, TOGlobals::kNeutronMass);
            
            // calculate real 4-vector of recoil proton
            TLorentzVector p4RecoilProtonCalc;
            SetP4(p4RecoilProtonCalc, ekinRecoil, TOGlobals::kProtonMass, p4RecoilProton.X(), p4RecoilProton.Y(), p4RecoilProton.Z());
  
            // calculate kinetic energy of the recoil proton using TOF
            Double_t ekinRecoilTOF = recoil_proton->CalculateEkinTOFTagger(TOGlobals::kProtonMass, fTaggerPhotonTime[i], fIsMC);
            
            // calculate real 4-vector of recoil proton using TOF
            TLorentzVector p4RecoilProtonCalcTOF;
            SetP4(p4RecoilProtonCalcTOF, ekinRecoilTOF, TOGlobals::kProtonMass, p4RecoilProton.X(), p4RecoilProton.Y(), p4RecoilProton.Z());
            
            // skip unphysical solutions of the recoil energy calculation
            Bool_t true_cm_ok = kTRUE;
            if (ekinRecoil <= 0 || TMath::IsNaN(ekinRecoil)) true_cm_ok = kFALSE;

            // skip unphysical solutions of the recoil energy calculation using TOF
            Bool_t true_cm_tof_ok = kTRUE;
            if (recoil_proton->GetDetector() != kTAPSDetector || ekinRecoilTOF <= 0) true_cm_tof_ok = kFALSE;
            
            // calculate the 4-vector of the undetected gamma for the pi0 production hypothesis
            TLorentzVector p4GammaMissing = p4Beam + p4Target - p4RecoilProtonCalc - *p4Gamma;

            // skip events with possible undetectable gamma in the backward hole
            Bool_t missGammaOk = kTRUE;
            //if (p4GammaMissing.Theta()*TMath::RadToDeg() > 120) missGammaOk = kFALSE;
  
            // fill tagger coincidence time
            if (imOk && copOk && missGammaOk) FillTaggerCoincidence(gamma, fTaggerPhotonTime[i], fH_Comp_p_Rand_Time_CB, 0);
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(gamma, fTaggerPhotonTime[i], 
                                                           -5.0, 5.0, -650, -50, 50, 250,
                                                              0,   0,    0,   0,  0,   0);
            
            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
            
            ///////////////////////////////

            //TLorentzVector* p4G1 = fPartMC[4]->GetP4();
            //TLorentzVector* p4G2 = fPartMC[5]->GetP4();
            //Double_t thetaDiff1 = TMath::Abs(p4Gamma->Theta() - p4G1->Theta());
            //Double_t thetaDiff2 = TMath::Abs(p4Gamma->Theta() - p4G2->Theta());
            //TLorentzVector* p4GLost;
            //
            //// gamma 1 was detected -> gamma 2 was lost
            //if (thetaDiff1 < thetaDiff2) p4GLost = p4G2;
            //// gamma 2 was detected -> gamma 1 was lost
            //else p4GLost = p4G1;
            //
            ////if (copOk) hTestP->Fill(TMath::RadToDeg()*p4GLost->Theta());
            //Double_t thetaLost = p4GLost->Theta()*TMath::RadToDeg();
            //if (thetaLost > 90 && thetaLost < 150) 
            //    hTestP->Fill(TMath::Abs(TMath::RadToDeg()*p4GLost->Vect().Angle(p4Gamma->Vect())));
    
            ///////////////////////////////

            // proton TOF cut
            Double_t tof = recoil_proton->CalculateTOFTagger(fTaggerPhotonTime[i], fIsMC);
            //if (tof < 3.5) continue;
            
            // calculate center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target;
            Double_t w = p4CM.M();
            
            // calculate true center-of-mass energy
            TLorentzVector p4CMTrue = p4RecoilProtonCalc + *p4Gamma;
            Double_t w_true = p4CMTrue.M();
            
            // calculate true center-of-mass energy using TOF
            TLorentzVector p4CMTrueTOF = p4RecoilProtonCalcTOF + *p4Gamma;
            Double_t w_true_tof = p4CMTrueTOF.M();
            
            // gamma 4-vector in c.m. frame
            TLorentzVector p4GammaCM(*p4Gamma);
            p4GammaCM.Boost(-p4CM.BoostVector());
            
            // gamma 4-vector in true c.m. frame
            TLorentzVector p4GammaCMTrue(*p4Gamma);
            p4GammaCMTrue.Boost(-p4CMTrue.BoostVector());
            Double_t cosThetaCMTrue = p4GammaCMTrue.CosTheta();
            
            // gamma 4-vector in true c.m. frame using TOF
            TLorentzVector p4GammaCMTrueTOF(*p4Gamma);
            p4GammaCMTrueTOF.Boost(-p4CMTrueTOF.BoostVector());
            Double_t cosThetaCMTrueTOF = p4GammaCMTrueTOF.CosTheta();
            
            // apply phase space cut
            //Double_t xtup[5] = { fTaggerPhotonHits[i], p4Gamma->E(), p4Gamma->Theta() * TMath::RadToDeg(), 
            //                     (p4Beam + p4Target - *p4Gamma).E() - TOGlobals::kProtonMass,
            //                     (p4Beam + p4Target - *p4Gamma).Theta() * TMath::RadToDeg()
            //                   };
            //Double_t ps_cont = TOHUtils::GetBinContent(fH_Comp_p_PhaseSpace, xtup);
            //if (ps_cont == 0) continue;
            
            // get detection efficiency
            Double_t xtup[5] = { fTaggerPhotonHits[i], p4Gamma->E(), p4Gamma->Theta() * TMath::RadToDeg(),
                                 p4RecoilProtonCalc.E() - TOGlobals::kProtonMass, p4RecoilProtonCalc.Theta() * TMath::RadToDeg() };
            Double_t eff;
            if (!fIsMC) eff = TOHUtils::GetBinContent(fH_Comp_p_DetEff, xtup);
            else eff = 1.;
            if (eff == 0) continue;
 
            // MVA classification
            //fMVA_Var_1_Comp_p = fTaggerPhotonHits[i];
            //fMVA_Var_2_Comp_p = p4Gamma->E();
            //fMVA_Var_3_Comp_p = p4Gamma->Theta() * TMath::RadToDeg();
            //fMVA_Var_4_Comp_p = (p4Beam + p4Target - *p4Gamma).E() - TOGlobals::kProtonMass;
            //fMVA_Var_5_Comp_p = (p4Beam + p4Target - *p4Gamma).Theta() * TMath::RadToDeg();
            //Double_t mva_value = fMVA_Reader_Comp_p->EvaluateMVA("FisherG");
            //fH_Comp_p_MVA_Value->Fill(mva_value);
            //if (mva_value < 2) continue;

            // missing energy
            Double_t eGammaCMCalc = (w*w - TOGlobals::kProtonMass*TOGlobals::kProtonMass) / (2.*w);
            //Double_t me = eGammaCMCalc - p4GammaCM.E();
            //Double_t me = (p4Beam + p4Target - *p4Gamma).M() - TOGlobals::kProtonMass;
            //Double_t me = (p4Beam + p4Target - p4RecoilProtonCalc).M();
            Double_t me = p4GammaMissing.Theta()*TMath::RadToDeg();
            Bool_t meOk = kTRUE;
            //if (me < -100 || me > 100) meOk = kFALSE;
            
            // check energy vs. theta of the missing gamma
            if (imOk && meOk && copOk)
            {
                fH_Comp_p_E_Theta_Gamma_Miss->Fill(p4GammaMissing.E(), p4GammaMissing.Theta()*TMath::RadToDeg(), subtr_weight);
            }

            // fill coplanarity
            if (imOk && meOk && missGammaOk)
            {
                fH_Comp_p_PhiDiff_Gamma_Recoil->Fill(phi_diff, subtr_weight);
            }

            // fill invariant mass
            if (meOk && copOk && missGammaOk)
            {
                fH_Comp_p_2g_IM->Fill(im, subtr_weight);    
            }

            // fill missing energy
            if (imOk && copOk && missGammaOk)  
            {
                fH_Comp_p_ME_W_Tot->Fill(me, w_true, subtr_weight);
                if (true_cm_ok) fH_Comp_p_ME_W[GetCTBin(cosThetaCMTrue, fComp_p_NCTBin)]->Fill(me, w_true, subtr_weight / eff);
                if (true_cm_tof_ok) fH_Comp_p_ME_W_TOF[GetCTBin(cosThetaCMTrueTOF, fComp_p_NCTBin)]->Fill(me, w_true_tof, subtr_weight);
            }
            
            // fill good events
            if (imOk && meOk && copOk && missGammaOk)
            {
                // check TOF of gamma
                FillTOF(fH_Comp_p_Photon_TOF_CB, 0, fPartNeutral[0], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check TOF and dE/E of recoil proton
                FillTOF(0, fH_Comp_p_Proton_TOF_TAPS, recoil_proton, fTaggerPhotonTime[i], fIsMC, subtr_weight);
                FilldE_E(0, fH_Comp_p_Proton_dE_E_TAPS, recoil_proton, subtr_weight);

                // check PSA
                FillPSA(fH_Comp_p_Proton_PSA, recoil_proton, subtr_weight);
                
                // check Fermi momentum
                fH_Comp_p_Recoil_Fermi->Fill((p4Beam.Vect() - p4RecoilProtonCalc.Vect() - p4Gamma->Vect()).Mag(), subtr_weight);
                
                // compare energies of the recoil nucleon
                fH_Comp_p_ENuclKin_ENuclTOF->Fill(p4RecoilProtonCalc.E(), p4RecoilProtonCalcTOF.E(), subtr_weight);
                
                // check beam energy vs. cos(theta) CM
                if (true_cm_ok) fH_Comp_p_EBeam_CosThetaCM->Fill(fTaggerPhotonHits[i], cosThetaCMTrue, subtr_weight);
                
                // check energy vs. theta of the detected gamma
                fH_Comp_p_E_Theta_Gamma->Fill(p4Gamma->E(), thetaGamma, subtr_weight);
  
                // MC efficiency determination 
                if (fCreateComp_p_Eff) 
                {
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_Comp_p_ExFunc_W[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                        if (thetaGamma >= 90 && thetaGamma < 110) 
                           fH_Comp_p_ExFunc_W_90_110[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]); 
                        else if (thetaGamma >= 110 && thetaGamma < 130) 
                           fH_Comp_p_ExFunc_W_110_130[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]); 
                        else if (thetaGamma >= 130 && thetaGamma < 150) 
                           fH_Comp_p_ExFunc_W_130_150[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]); 
                        
                        Double_t xtup[5] = { fTaggerPhotonHits[i], p4GammaMC->E(), p4GammaMC->Theta() * TMath::RadToDeg(),
                                             p4RecoilMC->E() - TOGlobals::kProtonMass, p4RecoilMC->Theta() * TMath::RadToDeg() };
                        fH_Comp_p_Det->Fill(xtup);
                    }

                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_Comp_p_ExFunc_W_TOF[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                     
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated gamma  : %f %f %f %f\n", p4GammaMC->E(), p4GammaMC->Px(), p4GammaMC->Py(), p4GammaMC->Pz());
                        printf("detected gamma   : %f %f %f %f\n", p4Gamma->E(), p4Gamma->Px(), p4Gamma->Py(), p4Gamma->Pz());
                        printf("generated proton : %f %f %f %f\n", p4RecoilMC->E(), p4RecoilMC->Px(), p4RecoilMC->Py(), p4RecoilMC->Pz());
                        printf("detected proton  : %f %f %f %f\n", p4RecoilProtonCalc.E(), p4RecoilProtonCalc.Px(), p4RecoilProtonCalc.Py(), p4RecoilProtonCalc.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_Comp_p_ExFunc_W[GetCTBin(cosThetaCMTrue, fComp_p_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff);
                        if (thetaGamma >= 90 && thetaGamma < 110) 
                           fH_Comp_p_ExFunc_W_90_110[GetCTBin(cosThetaCMTrue, fComp_p_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff); 
                        else if (thetaGamma >= 110 && thetaGamma < 130) 
                           fH_Comp_p_ExFunc_W_110_130[GetCTBin(cosThetaCMTrue, fComp_p_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff); 
                        else if (thetaGamma >= 130 && thetaGamma < 150) 
                           fH_Comp_p_ExFunc_W_130_150[GetCTBin(cosThetaCMTrue, fComp_p_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff); 
                    }

                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_Comp_p_ExFunc_W_TOF[GetCTBin(cosThetaCMTrueTOF, fComp_p_NCTBin)]->Fill(w_true_tof, fTaggerPhotonHits[i], subtr_weight);
                }
            }

        } // tagger loop

    } // end exclusive quasi-free compton scattering g + p -> g + p reconstruction
    label_end_Comp_p:



    ////////////////////////////////////////////////////////////////////////////
    // exclusive quasi-free compton scattering g + n -> g + n reconstruction  //
    ////////////////////////////////////////////////////////////////////////////
  
    if (fNNeutral == 2 && fNCharged == 0)
    {
        Int_t idGamma, idRecoil;

        // check detectors
        if (fCBNCluster == 1 && fTAPSNCluster == 1)
        {
            if (fPartNeutral[0]->GetDetector() == kCBDetector)
            {
                idGamma = 0;
                idRecoil = 1;
            }
            else
            {
                idGamma = 1;
                idRecoil = 0;
            }
        }
        else goto label_end_Comp_n;

        // compton gamma
        TOA2RecParticle gamma(1);
        if (!gamma.Reconstruct(1, fPartNeutral+idGamma)) goto label_end_Comp_n;
        TLorentzVector* p4Gamma = gamma.Get4Vector();
        Double_t thetaGamma = p4Gamma->Theta() * TMath::RadToDeg();
 
        // recoil neutron
        TOA2DetParticle* recoil_neutron = fPartNeutral[idRecoil];
        TLorentzVector p4RecoilNeutron;
        SetP4(p4RecoilNeutron, recoil_neutron->GetEnergy(), TOGlobals::kNeutronMass, 
              recoil_neutron->GetX(), recoil_neutron->GetY(), recoil_neutron->GetZ());
        
        // save event in reduced file
        fSaveEvent = 1;
  
        // check software trigger
        if (!IsTrigger(gamma)) goto label_end_Comp_n;
  
        // cut on clean photons at backward angles
        if (thetaGamma < 90 || thetaGamma > 150) goto label_end_Comp_n;
        
        // remove background by requesting no signals in the other detectors
        // no hit in the PID
        if (fPIDNhits > 0) goto label_end_Comp_n;

        // no hit in the Veto
        if (fVetoNhits > 0) goto label_end_Comp_n;
 
        // get invariant mass for background reduction
        TLorentzVector p4FalseGamma;
        SetP4(p4FalseGamma, recoil_neutron->GetEnergy(), 0, 
              recoil_neutron->GetX(), recoil_neutron->GetY(), recoil_neutron->GetZ());
        Double_t im = (*p4Gamma + p4FalseGamma).M();

        // invariant mass cut
        Bool_t imOk = kTRUE;
        //if (im < 160) imOk = kFALSE;
        
        // check gamma-neutron kinematics
        Double_t phi_diff = TMath::Abs(p4Gamma->Phi() - p4RecoilNeutron.Phi())*TMath::RadToDeg();
        Bool_t copOk = kTRUE;
        if (phi_diff < 170 || phi_diff > 190) copOk = kFALSE;

        // target 4-vector
        TLorentzVector p4Target(0., 0., 0., TOGlobals::kNeutronMass);

        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // skip bad tagger channels
            if (IsBadTaggerChannel(fTaggerPhotonHits[i])) continue;
            
            // beam 4-vector
            TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
  
            // calculate kinetic energy of recoil neutron
            Double_t ekinRecoil = TOPhysics::CalcQFThreeBodyRecoilPartT(fTaggerPhotonEnergy[i], p4Gamma, &p4RecoilNeutron,
                                                                        TOGlobals::kDeuteronMass, TOGlobals::kNeutronMass, TOGlobals::kProtonMass);
            
            // calculate real 4-vector of recoil neutron
            TLorentzVector p4RecoilNeutronCalc;
            SetP4(p4RecoilNeutronCalc, ekinRecoil, TOGlobals::kNeutronMass, p4RecoilNeutron.X(), p4RecoilNeutron.Y(), p4RecoilNeutron.Z());
            
            // calculate kinetic energy of the recoil neutron using TOF
            Double_t ekinRecoilTOF = recoil_neutron->CalculateEkinTOFTagger(TOGlobals::kNeutronMass, fTaggerPhotonTime[i], fIsMC);
            
            // calculate real 4-vector of recoil neutron using TOF
            TLorentzVector p4RecoilNeutronCalcTOF;
            SetP4(p4RecoilNeutronCalcTOF, ekinRecoilTOF, TOGlobals::kNeutronMass, p4RecoilNeutron.X(), p4RecoilNeutron.Y(), p4RecoilNeutron.Z());

            // skip unphysical solutions of the recoil energy calculation
            Bool_t true_cm_ok = kTRUE;
            if (ekinRecoil <= 0 || TMath::IsNaN(ekinRecoil)) true_cm_ok = kFALSE;

            // skip unphysical solutions of the recoil energy calculation using TOF
            Bool_t true_cm_tof_ok = kTRUE;
            if (recoil_neutron->GetDetector() != kTAPSDetector || ekinRecoilTOF <= 0) true_cm_tof_ok = kFALSE;
            
            // calculate the 4-vector of the undetected gamma for the pi0 production hypothesis
            TLorentzVector p4GammaMissing = p4Beam + p4Target - p4RecoilNeutronCalc - *p4Gamma;

            // skip events with possible undetectable gamma in the backward hole
            Bool_t missGammaOk = kTRUE;
            //if (p4GammaMissing.Theta()*TMath::RadToDeg() > 120) missGammaOk = kFALSE;
  
            // fill tagger coincidence time
            if (imOk && copOk && missGammaOk) FillTaggerCoincidence(gamma, fTaggerPhotonTime[i], fH_Comp_n_Rand_Time_CB, 0);
            
            // calculate background subtraction factor
            Double_t subtr_weight = GetBGSubtractionWeight(gamma, fTaggerPhotonTime[i], 
                                                           -5.0, 5.0, -650, -50, 50, 250,
                                                              0,   0,    0,   0,  0,   0);
            
            // skip useless background events (not belonging to the prompt/background windows)
            if (subtr_weight == 0) continue;
            
            ///////////////////////////////

            //TLorentzVector* p4G1 = fPartMC[4]->GetP4();
            //TLorentzVector* p4G2 = fPartMC[5]->GetP4();
            //Double_t thetaDiff1 = TMath::Abs(p4Gamma->Theta() - p4G1->Theta());
            //Double_t thetaDiff2 = TMath::Abs(p4Gamma->Theta() - p4G2->Theta());
            //TLorentzVector* p4GLost;
            //
            //// gamma 1 was detected -> gamma 2 was lost
            //if (thetaDiff1 < thetaDiff2) p4GLost = p4G2;
            //// gamma 2 was detected -> gamma 1 was lost
            //else p4GLost = p4G1;
            //
            ////if (copOk) hTestN->Fill(TMath::RadToDeg()*p4GLost->Theta());
            //Double_t thetaLost = p4GLost->Theta()*TMath::RadToDeg();
            //if (thetaLost > 90 && thetaLost < 150)
            //    hTestN->Fill(TMath::Abs(TMath::RadToDeg()*p4GLost->Vect().Angle(p4Gamma->Vect())));
  
            ///////////////////////////////

 
            // neutron TOF cut
            Double_t tof = recoil_neutron->CalculateTOFTagger(fTaggerPhotonTime[i], fIsMC);
            //if (tof < 3.5) continue;
            
            // calculate center-of-mass energy
            TLorentzVector p4CM = p4Beam + p4Target;
            Double_t w = p4CM.M();
   
            // calculate true center-of-mass energy
            TLorentzVector p4CMTrue = p4RecoilNeutronCalc + *p4Gamma;
            Double_t w_true = p4CMTrue.M();
            
            // calculate true center-of-mass energy using TOF
            TLorentzVector p4CMTrueTOF = p4RecoilNeutronCalcTOF + *p4Gamma;
            Double_t w_true_tof = p4CMTrueTOF.M();
            
            // gamma 4-vector in c.m. frame
            TLorentzVector p4GammaCM(*p4Gamma);
            p4GammaCM.Boost(-p4CM.BoostVector());
  
            // gamma 4-vector in true c.m. frame
            TLorentzVector p4GammaCMTrue(*p4Gamma);
            p4GammaCMTrue.Boost(-p4CMTrue.BoostVector());
            Double_t cosThetaCMTrue = p4GammaCMTrue.CosTheta();
            
            // gamma 4-vector in true c.m. frame using TOF
            TLorentzVector p4GammaCMTrueTOF(*p4Gamma);
            p4GammaCMTrueTOF.Boost(-p4CMTrueTOF.BoostVector());
            Double_t cosThetaCMTrueTOF = p4GammaCMTrueTOF.CosTheta();
            
            // apply phase space cut
            //Double_t xtup[5] = { fTaggerPhotonHits[i], p4Gamma->E(), p4Gamma->Theta() * TMath::RadToDeg(), 
            //                     (p4Beam + p4Target - *p4Gamma).E() - TOGlobals::kNeutronMass,
            //                     (p4Beam + p4Target - *p4Gamma).Theta() * TMath::RadToDeg()
            //                   };
            //Double_t ps_cont = TOHUtils::GetBinContent(fH_Comp_n_PhaseSpace, xtup);
            //if (ps_cont == 0) continue;
            
            // get detection efficiency
            Double_t xtup[5] = { fTaggerPhotonHits[i], p4Gamma->E(), p4Gamma->Theta() * TMath::RadToDeg(),
                                 p4RecoilNeutronCalc.E() - TOGlobals::kNeutronMass, p4RecoilNeutronCalc.Theta() * TMath::RadToDeg() };
            Double_t eff;
            if (!fIsMC) eff = TOHUtils::GetBinContent(fH_Comp_n_DetEff, xtup);
            else eff = 1.;
            if (eff == 0) continue;
  
            // MVA classification
            //fMVA_Var_1_Comp_n = fTaggerPhotonHits[i];
            //fMVA_Var_2_Comp_n = p4Gamma->E();
            //fMVA_Var_3_Comp_n = p4Gamma->Theta() * TMath::RadToDeg();
            //fMVA_Var_4_Comp_n = (p4Beam + p4Target - *p4Gamma).E() - TOGlobals::kNeutronMass;
            //fMVA_Var_5_Comp_n = (p4Beam + p4Target - *p4Gamma).Theta() * TMath::RadToDeg();
            //Double_t mva_value = fMVA_Reader_Comp_n->EvaluateMVA("FisherG");
            //fH_Comp_n_MVA_Value->Fill(mva_value);
            //if (mva_value < 0.4) continue;
 
            // missing energy
            Double_t eGammaCMCalc = (w*w - TOGlobals::kNeutronMass*TOGlobals::kNeutronMass) / (2.*w);
            //Double_t me = eGammaCMCalc - p4GammaCM.E();
            //Double_t me = (p4Beam + p4Target - *p4Gamma).M() - TOGlobals::kNeutronMass;
            //Double_t me = (p4Beam + p4Target - p4RecoilNeutronCalc).M();
            Double_t me = p4GammaMissing.Theta()*TMath::RadToDeg();
            Bool_t meOk = kTRUE;
            //if (me < -100 || me > 100) meOk = kFALSE;
            
            // check energy vs. theta of the missing gamma
            if (imOk && meOk && copOk)
            {
                fH_Comp_n_E_Theta_Gamma_Miss->Fill(p4GammaMissing.E(), p4GammaMissing.Theta()*TMath::RadToDeg(), subtr_weight);
            }
 
            // fill coplanarity
            if (imOk && meOk && missGammaOk)
            {
                fH_Comp_n_PhiDiff_Gamma_Recoil->Fill(phi_diff, subtr_weight);
            }

            // fill invariant mass
            if (meOk && copOk && missGammaOk)
            {
                fH_Comp_n_2g_IM->Fill(im, subtr_weight);    
            }
 
            // fill missing energy
            if (imOk && copOk && missGammaOk)  
            {
                fH_Comp_n_ME_W_Tot->Fill(me, w_true, subtr_weight);
                if (true_cm_ok) fH_Comp_n_ME_W[GetCTBin(cosThetaCMTrue, fComp_n_NCTBin)]->Fill(me, w_true, subtr_weight / eff);
                if (true_cm_tof_ok) fH_Comp_n_ME_W_TOF[GetCTBin(cosThetaCMTrueTOF, fComp_n_NCTBin)]->Fill(me, w_true_tof, subtr_weight);
            }
  
            // fill good events
            if (imOk && meOk && copOk && missGammaOk)
            {
                // check TOF of gamma
                FillTOF(fH_Comp_n_Photon_TOF_CB, 0, fPartNeutral[0], fTaggerPhotonTime[i], fIsMC, subtr_weight);
                
                // check TOF of recoil neutron
                FillTOF(0, fH_Comp_n_Neutron_TOF_TAPS, recoil_neutron, fTaggerPhotonTime[i], fIsMC, subtr_weight);

                // check PSA
                FillPSA(fH_Comp_n_Neutron_PSA, recoil_neutron, subtr_weight);
                
                // check Fermi momentum
                fH_Comp_n_Recoil_Fermi->Fill((p4Beam.Vect() - p4RecoilNeutronCalc.Vect() - p4Gamma->Vect()).Mag(), subtr_weight);
                
                // compare energies of the recoil nucleon
                fH_Comp_n_ENuclKin_ENuclTOF->Fill(p4RecoilNeutronCalc.E(), p4RecoilNeutronCalcTOF.E(), subtr_weight);
                
                // check beam energy vs. cos(theta) CM
                if (true_cm_ok) fH_Comp_n_EBeam_CosThetaCM->Fill(fTaggerPhotonHits[i], cosThetaCMTrue, subtr_weight);
                
                // check energy vs. theta of the detected gamma
                fH_Comp_n_E_Theta_Gamma->Fill(p4Gamma->E(), thetaGamma, subtr_weight);
      
                // MC efficiency determination 
                if (fCreateComp_n_Eff) 
                {
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_Comp_n_ExFunc_W[GetCTBin(cosThetaCMTrue_MC, fComp_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                        if (thetaGamma >= 90 && thetaGamma < 110) 
                           fH_Comp_n_ExFunc_W_90_110[GetCTBin(cosThetaCMTrue_MC, fComp_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]); 
                        else if (thetaGamma >= 110 && thetaGamma < 130) 
                           fH_Comp_n_ExFunc_W_110_130[GetCTBin(cosThetaCMTrue_MC, fComp_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]); 
                        else if (thetaGamma >= 130 && thetaGamma < 150) 
                           fH_Comp_n_ExFunc_W_130_150[GetCTBin(cosThetaCMTrue_MC, fComp_p_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]); 
                        
                        Double_t xtup[5] = { fTaggerPhotonHits[i], p4GammaMC->E(), p4GammaMC->Theta() * TMath::RadToDeg(),
                                             p4RecoilMC->E() - TOGlobals::kNeutronMass, p4RecoilMC->Theta() * TMath::RadToDeg() };
                        fH_Comp_n_Det->Fill(xtup);
                    }

                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_Comp_n_ExFunc_W_TOF[GetCTBin(cosThetaCMTrue_MC, fComp_n_NCTBin)]->Fill(w_true_mc, fTaggerPhotonHits[i]);
                     
                    // debug output to check synchronization
                    if (fEventCounter % 1000000 < 100)
                    {
                        printf("generated gamma   : %f %f %f %f\n", p4GammaMC->E(), p4GammaMC->Px(), p4GammaMC->Py(), p4GammaMC->Pz());
                        printf("detected gamma    : %f %f %f %f\n", p4Gamma->E(), p4Gamma->Px(), p4Gamma->Py(), p4Gamma->Pz());
                        printf("generated neutron : %f %f %f %f\n", p4RecoilMC->E(), p4RecoilMC->Px(), p4RecoilMC->Py(), p4RecoilMC->Pz());
                        printf("detected neutron  : %f %f %f %f\n", p4RecoilNeutronCalc.E(), p4RecoilNeutronCalc.Px(), p4RecoilNeutronCalc.Py(), p4RecoilNeutronCalc.Pz());
                        printf("\n");
                    }
                }
                // normal data analysis
                else
                {
                    // excitation function with true center-of-mass energy
                    if (true_cm_ok)
                    {
                        fH_Comp_n_ExFunc_W[GetCTBin(cosThetaCMTrue, fComp_n_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff);
                        if (thetaGamma >= 90 && thetaGamma < 110) 
                           fH_Comp_n_ExFunc_W_90_110[GetCTBin(cosThetaCMTrue, fComp_n_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff); 
                        else if (thetaGamma >= 110 && thetaGamma < 130) 
                           fH_Comp_n_ExFunc_W_110_130[GetCTBin(cosThetaCMTrue, fComp_n_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff); 
                        else if (thetaGamma >= 130 && thetaGamma < 150) 
                           fH_Comp_n_ExFunc_W_130_150[GetCTBin(cosThetaCMTrue, fComp_n_NCTBin)]->Fill(w_true, fTaggerPhotonHits[i], subtr_weight / eff); 
                    }

                    // excitation function with true center-of-mass energy using TOF
                    if (true_cm_tof_ok) fH_Comp_n_ExFunc_W_TOF[GetCTBin(cosThetaCMTrueTOF, fComp_n_NCTBin)]->Fill(w_true_tof, fTaggerPhotonHits[i], subtr_weight);
                }
            }

        } // tagger loop

    } // end exclusive quasi-free compton scattering g + n -> g + n reconstruction
    label_end_Comp_n:


    //fclose(outText);

    return;
}
    
//______________________________________________________________________________
void TA2MyAnalysis::FillExFunc(Int_t taggElem, TH1* hExFunc, Double_t weight, Double_t eKin, Double_t theta, TH2* hEff = 0)
{
    // Fill the meson with kinetic energy 'eKin' and theta (deg) 'theta' using its 
    // detection efficiency stored in 'hEff' into the excitation function histogram 
    // 'hExFunc' using the tagger channel 'taggElem'. The final fill 
    // weight will be multiplied with 'weight'.
    
    // check if efficiency histo is here
    if (hEff)
    {
        // get detection efficiency for this particle
        Int_t binX = hEff->GetXaxis()->FindBin(eKin);
        Int_t binY = hEff->GetYaxis()->FindBin(theta);
        Double_t eff = hEff->GetBinContent(binX, binY);
        
        // abort if efficiency is 0
        if (eff == 0.) return;

        // fill excitation function and set the error
        hExFunc->Fill(taggElem, weight / eff);
    }
    else
    {
        hExFunc->Fill(taggElem, weight);
    }
}

//______________________________________________________________________________
Int_t TA2MyAnalysis::GetCTBin(Double_t cosTheta, Int_t nCosThetaBins)
{
    // Returns the index of the cos(theta) bin corresponding to the cos(theta)
    // value 'cosTheta' when using a total number of 'nCosThetaBins' cos(theta) 
    // bins. Indexing starts at 0.

    // check range
    if (TMath::IsNaN(cosTheta)) 
    {
        Warning("GetCosThetaBin", "cos(theta) value is NaN!");
        return 0;
    }
    else if (cosTheta < -1 || cosTheta >= 1)
    {
        Error("GetCosThetaBin", "cos(theta) value out of range!");
        return 0;
    }
    else
    {
        return Int_t(nCosThetaBins * (cosTheta + 1.) / 2.);
    }
}

