// SVN Info: $Id: TA2MyCalibration.h 455 2009-06-21 19:27:15Z werthm $

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


#ifndef __TA2MyCalibration_h__
#define __TA2MyCalibration_h__

#include "TA2MyPhysics.h"


enum {
    EMC_Calib_CB_2GAMMA = 30000,
    EMC_Calib_TAPS_2GAMMA,
    EMC_Calib_TAPS_PSA,
    EMC_Calib_TAPS_LED,
    EMC_Calib_TIME,
    EMC_Calib_PID,
    EMC_Calib_CB_WALK,
    EMC_Calib_TAGGER_TIME,
    EMC_Calib_TAPS_DEBUG_OUTPUT,
    EMC_Calib_CLUSTER_DEBUG,
    EMC_Calib_VETO
};


static const Map_t myCalibrationConfigKeys[] = {
    // Calibration configuration keys
    {"CB-TwoGamma-Calib:"   ,   EMC_Calib_CB_2GAMMA         },        // CB 2 gamma energy calibration
    {"TAPS-TwoGamma-Calib:" ,   EMC_Calib_TAPS_2GAMMA       },        // TAPS 2 gamma energy calibration
    {"TAPS-PSA-Calib:"      ,   EMC_Calib_TAPS_PSA          },        // TAPS PSA calibration
    {"TAPS-LED-Calib:"      ,   EMC_Calib_TAPS_LED          },        // TAPS LED calibration
    {"Time-Calib:"          ,   EMC_Calib_TIME              },        // time calibration
    {"PID-Calib:"           ,   EMC_Calib_PID               },        // PID calibration
    {"CB-Walk:"             ,   EMC_Calib_CB_WALK           },        // CB walk correction
    {"Tagger-Time-Calib:"   ,   EMC_Calib_TAGGER_TIME       },        // Tagger time calibration
    {"TAPS-Debug-Output:"   ,   EMC_Calib_TAPS_DEBUG_OUTPUT },        // TAPS debug output
    {"Cluster-Debug:"       ,   EMC_Calib_CLUSTER_DEBUG     },        // cluster debugging
    {"Veto-Calib:"          ,   EMC_Calib_VETO              },        // Veto calibration

    // Termination
    {NULL        , -1           }
};


class TA2MyCalibration : public TA2MyPhysics
{

private:
    UInt_t fNCalib;                                         // number of enabled calibrations
    Int_t fCB2gCalib;                                       // CB 2 gamma energy calibration
    Int_t fTAPS2gCalib;                                     // TAPS 2 gamma energy calibration
    Int_t fTAPS_PSACalib;                                   // TAPS PSA calibration
    Int_t fTAPS_LEDCalib;                                   // TAPS LED calibration
    Int_t fTimeCalib;                                       // time calibration
    Int_t fPIDCalib;                                        // PID calibration
    Int_t fCBWalk;                                          // CB walk correction
    Int_t fTaggerTimeCalib;                                 // Tagger time calibration
    Int_t fTAPSDebugOutput;                                 // TAPS debug output
    Int_t fClusterDebug;                                    // cluster debugging
    Int_t fVetoCalib;                                       // Veto calibration

    UInt_t  fNTotClusters;                                  // number of total clusters
    TLorentzVector* fCBP4;                                  // 4-vector array of particle detected in CB
    TLorentzVector* fTAPSP4;                                // 4-vector array of particle detected in TAPS

    TH1F* fH_Calib_BeamHel;
    TH2F* fH_Calib_CB_PID;
    TH1F* fH_Calib_Total_E;
    TH1F* fH_Calib_IM_2g;
    TH1F* fH_Calib_IM_2g_2CB;
    TH1F* fH_Calib_IM_2g_1TAPS;
    TH1F* fH_Calib_IM_2g_2TAPS;
    TH1F* fH_Calib_IM_2g_cut;
    TH2F* fH_Calib_IM_2g_IM_Em;
    TH1F* fH_Calib_Charged_Theta;
    TH1F* fH_Calib_NtotalClusters;
    TH2F* fH_Calib_TAPS_Energy;
    TH2F* fH_Calib_CB_Energy;
    TH2F* fH_Calib_CB_ClSize_v_Energy;
    TH1F* fH_Calib_Tagger_Nhits;
    TH1F* fH_Calib_Tagger_Hits;
    TH1F* fH_Calib_Tagger_Energy;
    TH1F* fH_Calib_Tagger_Time;
    TH1F* fH_Calib_HelTest;         
    TH2F* fH_Calib_TAPS_VetoCorrel;         
    TH1F* fH_Calib_CB_2g_pi0_Energy;
    TH1F* fH_Calib_CB_2g_pi0_Angle;
    TH1F* fH_Calib_CB_2g_eta_Energy;
    TH1F* fH_Calib_CB_2g_eta_Angle;

    TH2F* fH_Calib_TAPS_SingleClusterPos;
    TH2F* fH_Calib_TAPS_ClusterPos;
    TH2F* fH_Calib_TAPS_ClPhi_v_ClTheta;
    TH2F* fH_Calib_CB_ClPhi_v_ClTheta;
    TH2F* fH_Calib_TAPS_TOF;               
    TH2F* fH_Calib_TAPS_Walk[384];               

    TH1F* fH_Calib_CFD;
    TH1F* fH_Calib_LED1;
    TH1F* fH_Calib_LED2;
    TH1F* fH_Calib_TAPS_LG[384];        
    TH1F* fH_Calib_TAPS_LG_CFD[384];    
    TH1F* fH_Calib_TAPS_LG_NOCFD[384];  
    TH1F* fH_Calib_TAPS_LG_LED1[384];   
    TH1F* fH_Calib_TAPS_LG_LED2[384];   

    TH1F* fH_Calib_CB_2g_IMS;     
    TH2F* fH_Calib_CB_2g_IM;      
    TH2F* fH_Calib_CB_2g_IM2;     
    TH1F* fH_Calib_CB_2g_IMS_cut; 
    TH2F* fH_Calib_CB_2g_IM_cut;  
    TH2F* fH_Calib_CB_2g_IM2_cut; 
    TH2F* fH_Calib_CB_2g_Time;    
    TH2F* fH_Calib_CB_2g_Time_cut;

    TH1F* fH_Calib_TAPS_1g_IMS;    
    TH2F* fH_Calib_TAPS_1g_IM;     
    TH2F* fH_Calib_TAPS_1g_IM2;    
    TH1F* fH_Calib_TAPS_2g_IMS_noV;
    TH1F* fH_Calib_TAPS_2g_IMS;    
    TH2F* fH_Calib_TAPS_2g_IM;     
    TH2F* fH_Calib_TAPS_2g_Time;   

    TH2F* fH_Calib_Time_Tagger;
    TH2F* fH_Calib_Time_TAPS_vs_Tagger_ClnoV;
    TH2F* fH_Calib_Time_Tagger_vs_TAPS_ClnoV;
    TH2F* fH_Calib_Time_TAPS_vs_Tagger_Cl;
    TH2F* fH_Calib_Time_Tagger_vs_TAPS_Cl;
    TH2F* fH_Calib_Time_TAPS_vs_Tagger;
    TH2F* fH_Calib_Time_Tagger_vs_TAPS;     

    TH2F* fH_Calib_TAPS_PSAR_vs_PSAA_Veto[384];
    TH2F* fH_Calib_TAPS_PSAR_vs_PSAA[384];

    TH2F* fH_Calib_PID_ID_v_CBAz;       
    TH1F* fH_Calib_PID_CB_Phi_Diff;       
    TH2F* fH_Calib_PID_Energy;      
    TH2F* fH_Calib_PID_Time;        
    TH2F* fH_Calib_ClSize_v_dE;        
    TH2F* fH_Calib_PID_dE_v_Th[24];      

    TH2F* fH_Calib_CBWalk_Et[720];
    TH2F* fH_Calib_CBWalk_EtS;

    TH1F* fH_Calib_PbGlass_Time;    
    TH1F* fH_Calib_Tagger_m_PbGlass_Sum;
    TH1F* fH_Calib_Tagger_p_PbGlass_Sum;
    TH2F* fH_Calib_Tagger_m_PbGlass;
    TH2F* fH_Calib_Tagger_p_PbGlass;
    
    TH2F* fH_TAPS_Veto_dE_E_tot;
    TH2F* fH_TAPS_Veto_dE_E[384];


    Bool_t IsInOuterTAPSRing(Int_t inElement);
    Bool_t IsInInnerTAPSRing(Int_t inElement);

public:
    TA2MyCalibration(const char* name, TA2Analysis* analysis);
    virtual ~TA2MyCalibration();

    void SetConfig(Char_t* line, Int_t key);
    void PostInit();
    void LoadVariable()
    { 
        TA2MyPhysics::LoadVariable();
        TA2DataManager::LoadVariable("fNTotClusters",    &fNTotClusters,   EISingleX);
    }
    void ReconstructPhysics();
    
    ClassDef(TA2MyCalibration, 1)
};

#endif

