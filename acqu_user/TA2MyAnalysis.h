// SVN Info: $Id: TA2MyAnalysis.h 747 2011-01-14 17:41:57Z werthm $

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


#ifndef __TA2MyAnalysis_h__
#define __TA2MyAnalysis_h__

#include "TOA2RecEta2g.h"
#include "TOA2RecEta6g.h"
#include "TOPhysics.h"
#include "TOLoader.h"
#include "TOHUtils.h"

#include "TMVA/Reader.h"
#include "TGraphErrors.h"
#include "THnSparse.h"

#include "TA2MyPhysics.h"


enum {
    EMA_CREATE_ETA_GRID_EFF = 30000,
    EMA_CREATE_ETA_P_EFF,
    EMA_CREATE_ETA_N_EFF,
    EMA_CREATE_COMP_P_EFF,
    EMA_CREATE_COMP_N_EFF,
    EMA_N_CT_BINS,
};


static const Map_t myAnalysisConfigKeys[] = {
    // General keys
    {"Create-Eta-GridEff:" ,        EMA_CREATE_ETA_GRID_EFF  },           // key for the eta grid efficiency determination
    {"Create-Eta-p-Eff:" ,          EMA_CREATE_ETA_P_EFF  },              // key for the eta proton efficiency determination
    {"Create-Eta-n-Eff:" ,          EMA_CREATE_ETA_N_EFF  },              // key for the eta neutron efficiency determination
    {"Create-Comp-p-Eff:" ,         EMA_CREATE_COMP_P_EFF  },             // key for the compton proton efficiency determination
    {"Create-Comp-n-Eff:" ,         EMA_CREATE_COMP_N_EFF  },             // key for the compton neutron efficiency determination
    {"N-CosTheta-Bins:" ,           EMA_N_CT_BINS  },                     // key for the number of cos(theta) bins
    // Termination
    {NULL        , -1           }
};


class TA2MyAnalysis : public TA2MyPhysics
{

private:
    // fully inclusive eta -> 2g
    TH2F* fH_2g_fulinc_Photon_TOF_TAPS;
    TH2F* fH_2g_fulinc_Photon_TOF_CB;
    
    TH2F* fH_2g_fulinc_Photon_PSA;
    
    TH2F* fH_2g_fulinc_Eta_E_Theta_Gen;
    TH2F* fH_2g_fulinc_Eta_E_Theta;
  
    TH1F* fH_2g_fulinc_Coinc_Time_CB;
    TH1F* fH_2g_fulinc_Coinc_Time_CB_TAPS;
    TH1F* fH_2g_fulinc_Coinc_Time_TAPS;
    TH1F* fH_2g_fulinc_Rand_Time_CB;
    TH1F* fH_2g_fulinc_Rand_Time_TAPS;
   
    TH2F* fH_2g_fulinc_IM_Tot;
    TH2F* fH_2g_fulinc_IM_E;
    TH2F* fH_2g_fulinc_MM_Tot;

    Int_t f2g_fulinc_NCTBin;
    TH2F** fH_2g_fulinc_IM;
    TH2F** fH_2g_fulinc_E;
    TH1F** fH_2g_fulinc_ExFunc;
  
    
    // inclusive quasi-free g + X -> [eta->2g] + X
    TH2F* fH_2g_qfinc_Photon_TOF_TAPS;
    TH2F* fH_2g_qfinc_Photon_TOF_CB;
    
    TH2F* fH_2g_qfinc_Photon_PSA;
    
    TH1F* fH_2g_qfinc_Coinc_Time_CB;
    TH1F* fH_2g_qfinc_Coinc_Time_CB_TAPS;
    TH1F* fH_2g_qfinc_Coinc_Time_TAPS;
    TH1F* fH_2g_qfinc_Rand_Time_CB;
    TH1F* fH_2g_qfinc_Rand_Time_TAPS;
 
    TH2F* fH_2g_qfinc_IM_Tot;
    TH2F* fH_2g_qfinc_MM_Tot;
    
    Int_t f2g_qfinc_NCTBin;
    TH2F** fH_2g_qfinc_IM;
    TH1F** fH_2g_qfinc_ExFunc;
    TH1F** fH_2g_qfinc_ExFunc_Gen;

    THnSparseF* fH_2g_qfinc_Det;


    // fully exclusive quasi-free g + p -> [eta->2g] + p
    TH2F* fH_2g_fulexc_p_Photon_TOF_TAPS;
    TH2F* fH_2g_fulexc_p_Proton_TOF_TAPS;
    
    TH2F* fH_2g_fulexc_p_Photon_TOF_CB;
    TH2F* fH_2g_fulexc_p_Proton_TOF_CB;
     
    TH2F* fH_2g_fulexc_p_Photon_PSA;
    TH2F* fH_2g_fulexc_p_Proton_PSA;
    
    TH2F* fH_2g_fulexc_p_Proton_dE_E_CB;
    TH2F* fH_2g_fulexc_p_Proton_dE_E_TAPS;

    TH1F* fH_2g_fulexc_p_PhiDiff_Eta_Recoil;
    TH1F* fH_2g_fulexc_p_Recoil_Fermi;
    
    TH1F* fH_2g_fulexc_p_Coinc_Time_CB;
    TH1F* fH_2g_fulexc_p_Coinc_Time_CB_TAPS;
    TH1F* fH_2g_fulexc_p_Coinc_Time_TAPS;
    TH1F* fH_2g_fulexc_p_Rand_Time_CB;
    TH1F* fH_2g_fulexc_p_Rand_Time_TAPS;
 
    TH2F* fH_2g_fulexc_p_IM_Tot;
    TH2F* fH_2g_fulexc_p_MM_Tot;
    
    Int_t f2g_fulexc_p_NCTBin;
    TH2F** fH_2g_fulexc_p_IM;
    TH1F** fH_2g_fulexc_p_ExFunc;
    TH1F** fH_2g_fulexc_p_ExFunc_Gen;
    TH2F** fH_2g_fulexc_p_IM_W;
    TH2F** fH_2g_fulexc_p_ExFunc_W;
    TH2F** fH_2g_fulexc_p_ExFunc_W_Gen;
    TH2F** fH_2g_fulexc_p_IM_W_TOF;
    TH2F** fH_2g_fulexc_p_ExFunc_W_TOF;
    TH2F** fH_2g_fulexc_p_ExFunc_W_TOF_Gen;
    
    THnSparseF* fH_2g_fulexc_p_Det;


    // fully exclusive quasi-free g + n -> [eta->2g] + n
    TH2F* fH_2g_fulexc_n_Photon_TOF_TAPS;
    TH2F* fH_2g_fulexc_n_Neutron_TOF_TAPS;
    
    TH2F* fH_2g_fulexc_n_Photon_TOF_CB;
    TH2F* fH_2g_fulexc_n_Neutron_TOF_CB;
     
    TH2F* fH_2g_fulexc_n_Photon_PSA;
    TH2F* fH_2g_fulexc_n_Neutron_PSA;
    
    TH1F* fH_2g_fulexc_n_PhiDiff_Eta_Recoil;
    TH1F* fH_2g_fulexc_n_Recoil_Fermi;
    
    TH1F* fH_2g_fulexc_n_Coinc_Time_CB;
    TH1F* fH_2g_fulexc_n_Coinc_Time_CB_TAPS;
    TH1F* fH_2g_fulexc_n_Coinc_Time_TAPS;
    TH1F* fH_2g_fulexc_n_Rand_Time_CB;
    TH1F* fH_2g_fulexc_n_Rand_Time_TAPS;
 
    TH2F* fH_2g_fulexc_n_IM_Tot;
    TH2F* fH_2g_fulexc_n_MM_Tot;
    
    Int_t f2g_fulexc_n_NCTBin;
    TH2F** fH_2g_fulexc_n_IM;
    TH1F** fH_2g_fulexc_n_ExFunc;
    TH1F** fH_2g_fulexc_n_ExFunc_Gen;
    TH2F** fH_2g_fulexc_n_IM_W;
    TH2F** fH_2g_fulexc_n_ExFunc_W;
    TH2F** fH_2g_fulexc_n_ExFunc_W_Gen;
    TH2F** fH_2g_fulexc_n_IM_W_TOF;
    TH2F** fH_2g_fulexc_n_ExFunc_W_TOF;
    TH2F** fH_2g_fulexc_n_ExFunc_W_TOF_Gen;

    THnSparseF* fH_2g_fulexc_n_Det;

    
    // fully inclusive eta -> 6g
    TH2F* fH_6g_fulinc_Photon_TOF_TAPS;
    TH2F* fH_6g_fulinc_Photon_TOF_CB;
  
    TH2F* fH_6g_fulinc_Photon_PSA;
    
    TH2F* fH_6g_fulinc_Eta_E_Theta_Gen;
    TH2F* fH_6g_fulinc_Eta_E_Theta;
    
    TH1F* fH_6g_fulinc_Coinc_Time_CB;
    TH1F* fH_6g_fulinc_Coinc_Time_CB_TAPS;
    TH1F* fH_6g_fulinc_Coinc_Time_TAPS;
    TH1F* fH_6g_fulinc_Rand_Time_CB;
    TH1F* fH_6g_fulinc_Rand_Time_TAPS;
      
    TH2F* fH_6g_fulinc_IM_Tot;
    TH2F* fH_6g_fulinc_IM_E;
    TH2F* fH_6g_fulinc_MM_Tot;
    
    Int_t f6g_fulinc_NCTBin;
    TH2F** fH_6g_fulinc_IM;
    TH2F** fH_6g_fulinc_E;
    TH1F** fH_6g_fulinc_ExFunc;
    
    TH1F* fH_6g_fulinc_IM_Debug[10];
    
    
    // inclusive quasi-free g + X -> [eta->6g] + X
    TH2F* fH_6g_qfinc_Photon_TOF_TAPS;
    TH2F* fH_6g_qfinc_Photon_TOF_CB;
    
    TH2F* fH_6g_qfinc_Photon_PSA;
    
    TH1F* fH_6g_qfinc_Coinc_Time_CB;
    TH1F* fH_6g_qfinc_Coinc_Time_CB_TAPS;
    TH1F* fH_6g_qfinc_Coinc_Time_TAPS;
    TH1F* fH_6g_qfinc_Rand_Time_CB;
    TH1F* fH_6g_qfinc_Rand_Time_TAPS;
 
    TH2F* fH_6g_qfinc_IM_Tot;
    TH2F* fH_6g_qfinc_MM_Tot;
    
    Int_t f6g_qfinc_NCTBin;
    TH2F** fH_6g_qfinc_IM;
    TH1F** fH_6g_qfinc_ExFunc;
    TH1F** fH_6g_qfinc_ExFunc_Gen;

    TH1F* fH_6g_qfinc_IM_Debug[10];
 
   
    // fully exclusive quasi-free g + p -> [eta->6g] + p
    TH2F* fH_6g_fulexc_p_Photon_TOF_TAPS;
    TH2F* fH_6g_fulexc_p_Proton_TOF_TAPS;
    
    TH2F* fH_6g_fulexc_p_Photon_TOF_CB;
    TH2F* fH_6g_fulexc_p_Proton_TOF_CB;
     
    TH2F* fH_6g_fulexc_p_Photon_PSA;
    TH2F* fH_6g_fulexc_p_Proton_PSA;
    
    TH2F* fH_6g_fulexc_p_Proton_dE_E_CB;
    TH2F* fH_6g_fulexc_p_Proton_dE_E_TAPS;
    
    TH1F* fH_6g_fulexc_p_PhiDiff_Eta_Recoil;
    TH1F* fH_6g_fulexc_p_Recoil_Fermi;
    
    TH1F* fH_6g_fulexc_p_Coinc_Time_CB;
    TH1F* fH_6g_fulexc_p_Coinc_Time_CB_TAPS;
    TH1F* fH_6g_fulexc_p_Coinc_Time_TAPS;
    TH1F* fH_6g_fulexc_p_Rand_Time_CB;
    TH1F* fH_6g_fulexc_p_Rand_Time_TAPS;
   
    TH2F* fH_6g_fulexc_p_IM_Tot;
    TH2F* fH_6g_fulexc_p_MM_Tot;
    
    Int_t f6g_fulexc_p_NCTBin;
    TH2F** fH_6g_fulexc_p_IM;
    TH1F** fH_6g_fulexc_p_ExFunc;
    TH1F** fH_6g_fulexc_p_ExFunc_Gen;
    TH2F** fH_6g_fulexc_p_IM_W;
    TH2F** fH_6g_fulexc_p_ExFunc_W;
    TH2F** fH_6g_fulexc_p_ExFunc_W_Gen;
    
    TH1F* fH_6g_fulexc_p_IM_Debug[10];
   

    // fully exclusive quasi-free g + n -> [eta->6g] + n
    TH2F* fH_6g_fulexc_n_Photon_TOF_TAPS;
    TH2F* fH_6g_fulexc_n_Neutron_TOF_TAPS;
    
    TH2F* fH_6g_fulexc_n_Photon_TOF_CB;
    TH2F* fH_6g_fulexc_n_Neutron_TOF_CB;
     
    TH2F* fH_6g_fulexc_n_Photon_PSA;
    TH2F* fH_6g_fulexc_n_Neutron_PSA;
    
    TH1F* fH_6g_fulexc_n_PhiDiff_Eta_Recoil;
    TH1F* fH_6g_fulexc_n_Recoil_Fermi;
    
    TH1F* fH_6g_fulexc_n_Coinc_Time_CB;
    TH1F* fH_6g_fulexc_n_Coinc_Time_CB_TAPS;
    TH1F* fH_6g_fulexc_n_Coinc_Time_TAPS;
    TH1F* fH_6g_fulexc_n_Rand_Time_CB;
    TH1F* fH_6g_fulexc_n_Rand_Time_TAPS;
   
    TH2F* fH_6g_fulexc_n_IM_Tot;
    TH2F* fH_6g_fulexc_n_MM_Tot;
    
    Int_t f6g_fulexc_n_NCTBin;
    TH2F** fH_6g_fulexc_n_IM;
    TH1F** fH_6g_fulexc_n_ExFunc;
    TH1F** fH_6g_fulexc_n_ExFunc_Gen;
    TH2F** fH_6g_fulexc_n_IM_W;
    TH2F** fH_6g_fulexc_n_ExFunc_W;
    TH2F** fH_6g_fulexc_n_ExFunc_W_Gen;
    
    TH1F* fH_6g_fulexc_n_IM_Debug[10];
    

    // exclusive quasi-free compton scattering g + p -> g + p reconstruction
    TH2F* fH_Comp_p_Proton_TOF_TAPS;
    TH2F* fH_Comp_p_Photon_TOF_CB;
     
    TH2F* fH_Comp_p_Proton_PSA;
    
    TH2F* fH_Comp_p_Proton_dE_E_TAPS;

    TH1F* fH_Comp_p_2g_IM;
    TH1F* fH_Comp_p_PhiDiff_Gamma_Recoil;
    TH1F* fH_Comp_p_Recoil_Fermi;
    TH2F* fH_Comp_p_ENuclKin_ENuclTOF;
    TH2F* fH_Comp_p_EBeam_CosThetaCM;
    TH2F* fH_Comp_p_E_Theta_Gamma;
    TH2F* fH_Comp_p_E_Theta_Gamma_Miss;

    TH1F* fH_Comp_p_Rand_Time_CB;
 
    TH2F* fH_Comp_p_ME_W_Tot;
    
    Int_t fComp_p_NCTBin;
    TH2F** fH_Comp_p_ME_W;
    TH2F** fH_Comp_p_ExFunc_W;
    TH2F** fH_Comp_p_ExFunc_W_90_110;
    TH2F** fH_Comp_p_ExFunc_W_110_130;
    TH2F** fH_Comp_p_ExFunc_W_130_150;
    TH2F** fH_Comp_p_ExFunc_W_Gen;
    TH2F** fH_Comp_p_ME_W_TOF;
    TH2F** fH_Comp_p_ExFunc_W_TOF;
    
    THnSparseF* fH_Comp_p_Gen;
    THnSparseF* fH_Comp_p_Det;

    TH1F* fH_Comp_p_MVA_Value;
    TMVA::Reader* fMVA_Reader_Comp_p;
    Float_t fMVA_Var_1_Comp_p;
    Float_t fMVA_Var_2_Comp_p;
    Float_t fMVA_Var_3_Comp_p;
    Float_t fMVA_Var_4_Comp_p;
    Float_t fMVA_Var_5_Comp_p;


    // exclusive quasi-free compton scattering g + n -> g + n reconstruction
    TH2F* fH_Comp_n_Neutron_TOF_TAPS;
    TH2F* fH_Comp_n_Photon_TOF_CB;
     
    TH2F* fH_Comp_n_Neutron_PSA;
    
    TH1F* fH_Comp_n_2g_IM;
    TH1F* fH_Comp_n_PhiDiff_Gamma_Recoil;
    TH1F* fH_Comp_n_Recoil_Fermi;
    TH2F* fH_Comp_n_ENuclKin_ENuclTOF;
    TH2F* fH_Comp_n_EBeam_CosThetaCM;
    TH2F* fH_Comp_n_E_Theta_Gamma;
    TH2F* fH_Comp_n_E_Theta_Gamma_Miss;
    
    TH1F* fH_Comp_n_Rand_Time_CB;
 
    TH2F* fH_Comp_n_ME_W_Tot;
    
    Int_t fComp_n_NCTBin;
    TH2F** fH_Comp_n_ME_W;
    TH2F** fH_Comp_n_ExFunc_W;
    TH2F** fH_Comp_n_ExFunc_W_90_110;
    TH2F** fH_Comp_n_ExFunc_W_110_130;
    TH2F** fH_Comp_n_ExFunc_W_130_150;
    TH2F** fH_Comp_n_ExFunc_W_Gen;
    TH2F** fH_Comp_n_ME_W_TOF;
    TH2F** fH_Comp_n_ExFunc_W_TOF;
    
    THnSparseF* fH_Comp_n_Gen;
    THnSparseF* fH_Comp_n_Det;
    
    TH1F* fH_Comp_n_MVA_Value;
    TMVA::Reader* fMVA_Reader_Comp_n;
    Float_t fMVA_Var_1_Comp_n;
    Float_t fMVA_Var_2_Comp_n;
    Float_t fMVA_Var_3_Comp_n;
    Float_t fMVA_Var_4_Comp_n;
    Float_t fMVA_Var_5_Comp_n;


    // other class members
    Bool_t fCreateEtaGridEff;
    Bool_t fCreateEta_p_Eff;
    Bool_t fCreateEta_n_Eff;
    Bool_t fCreateComp_p_Eff;
    Bool_t fCreateComp_n_Eff;
    
    TH2* fH_2g_fulinc_DetEff;   
    TH2* fH_6g_fulinc_DetEff;   
    
    THnSparse* fH_2g_qfinc_PhaseSpace;   
    THnSparse* fH_2g_fulexc_p_PhaseSpace;   
    THnSparse* fH_2g_fulexc_n_PhaseSpace;   
    THnSparse* fH_Comp_p_DetEff;   
    THnSparse* fH_Comp_n_DetEff;   
     
    TGraphErrors* fG_2g_qfinc_MM_Min;
    TGraphErrors* fG_2g_qfinc_MM_Max;
    TGraph* fG_2g_fulexc_p_MM_Min;
    TGraph* fG_2g_fulexc_p_MM_Max;
    TGraph* fG_2g_fulexc_n_MM_Min;
    TGraph* fG_2g_fulexc_n_MM_Max;
    
    TGraphErrors* fG_6g_qfinc_MM_Min;
    TGraphErrors* fG_6g_qfinc_MM_Max;
    TGraphErrors* fG_6g_fulexc_p_MM_Min;
    TGraphErrors* fG_6g_fulexc_p_MM_Max;
    TGraphErrors* fG_6g_fulexc_n_MM_Min;
    TGraphErrors* fG_6g_fulexc_n_MM_Max;
    
    TH1* hTestP;
    TH1* hTestN;

    void FillExFunc(Int_t taggElem, TH1* hExFunc, Double_t weight, Double_t eKin, Double_t theta, TH2* hEff);
    Int_t GetCTBin(Double_t cosTheta, Int_t nCosThetaBins);
    
public:
    TA2MyAnalysis(const char* name, TA2Analysis* analysis);
    virtual ~TA2MyAnalysis();

    void SetConfig(Char_t* line, Int_t key);
    void PostInit();
    void ReconstructPhysics();
    
    ClassDef(TA2MyAnalysis, 1)
};

#endif

