// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2007-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyCaLib                                                           //
//                                                                      //
// Class used for the CaLib based calibration.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef __TA2MyCaLib_h__
#define __TA2MyCaLib_h__

#include "TOA2RecPi02g.h"

#include "TA2MyPhysics.h"


enum {
    ECALIB_TARGET_POS = 30000,
    ECALIB_CB_ENERGY,
    ECALIB_CB_QUAD,
    ECALIB_CB_TIME,
    ECALIB_CB_WALK,
    ECALIB_TAPS_ENERGY,
    ECALIB_TAPS_TIME,
    ECALIB_TAPS_QUAD,
    ECALIB_TAPS_PSA,
    ECALIB_TAPS_LED,
    ECALIB_PID_PHI,
    ECALIB_PID_ENERGY,
    ECALIB_PID_TIME,
    ECALIB_VETO_CORR,
    ECALIB_VETO_ENERGY,
    ECALIB_VETO_TIME,
    ECALIB_TAGGER_TIME,
};


static const Map_t myCaLibConfigKeys[] = {
    // Calibration configuration keys
    {"CaLib-Target-Position:"   ,   ECALIB_TARGET_POS    },        // target position
    {"CaLib-CB-Energy:"         ,   ECALIB_CB_ENERGY     },        // CB energy calibration
    {"CaLib-CB-Quad:"           ,   ECALIB_CB_QUAD       },        // CB quadratic energy correction
    {"CaLib-CB-Time:"           ,   ECALIB_CB_TIME       },        // CB time calibration
    {"CaLib-CB-Walk:"           ,   ECALIB_CB_WALK       },        // CB time walk calibration
    {"CaLib-TAPS-Energy:"       ,   ECALIB_TAPS_ENERGY   },        // TAPS energy calibration
    {"CaLib-TAPS-Time:"         ,   ECALIB_TAPS_TIME     },        // TAPS time calibration
    {"CaLib-TAPS-Quad:"         ,   ECALIB_TAPS_QUAD     },        // TAPS quadratic energy correction
    {"CaLib-TAPS-PSA:"          ,   ECALIB_TAPS_PSA      },        // TAPS pulse shape calibration
    {"CaLib-TAPS-LED:"          ,   ECALIB_TAPS_LED      },        // TAPS LED calibration
    {"CaLib-PID-Phi:"           ,   ECALIB_PID_PHI       },        // PID Phi calibration
    {"CaLib-PID-Energy:"        ,   ECALIB_PID_ENERGY    },        // PID energy calibration
    {"CaLib-PID-Time:"          ,   ECALIB_PID_TIME      },        // PID time calibration
    {"CaLib-Veto-Corr:"         ,   ECALIB_VETO_CORR     },        // veto-crystal element correlation
    {"CaLib-Veto-Energy:"       ,   ECALIB_VETO_ENERGY   },        // Veto energy calibration
    {"CaLib-Veto-Time:"         ,   ECALIB_VETO_TIME     },        // Veto time calibration
    {"CaLib-Tagger-Time:"       ,   ECALIB_TAGGER_TIME   },        // tagger time

    // Termination
    {NULL        , -1           }
};


class TA2MyCaLib : public TA2MyPhysics
{

private:
    UInt_t fNCalib;                                         // number of enabled calibrations
    Int_t fNelemTAGG;                                       // number of Tagger elements
    Int_t fNelemCB;                                         // number of CB elements
    Int_t fNelemTAPS;                                       // number of TAPS elements
    Int_t fNelemPID;                                        // number of PID elements
    Int_t fNelemVeto;                                       // number of Veto elements

    // ---------------------------------- target position ---------------------------------- 
    Int_t fCalib_Target_Position;                           // target position calibration toggle
    TH2* fHCalib_CB_IM_TargPos;                             // CB invariant mass vs target position

    // ------------------------------------- CB energy ------------------------------------- 
    Int_t fCalib_CB_Energy;                                 // CB energy calibration toggle
    TH2* fHCalib_CB_IM;                                     // CB invariant mass vs element
    TH2* fHCalib_CB_IM_Neut;                                // CB invariant mass (from neutral hits) vs element

    // -------------------------- CB quadratic energy correction --------------------------- 
    Int_t fCalib_CB_Quad;                                   // CB quadratic energy correction toggle
    Double_t fCalib_CB_Quad_Pi0_Min;                        // lower bound for the pi0 invariant mass cut
    Double_t fCalib_CB_Quad_Pi0_Max;                        // upper bound for the pi0 invariant mass cut
    Double_t fCalib_CB_Quad_Eta_Min;                        // lower bound for the eta invariant mass cut
    Double_t fCalib_CB_Quad_Eta_Max;                        // upper bound for the eta invariant mass cut
    TH2* fHCalib_CB_Quad_IM;                                // CB invariant mass vs element
    TH2* fHCalib_CB_Quad_Pi0_Mean_E;                        // mean energy of pi0 in CB
    TH2* fHCalib_CB_Quad_Eta_Mean_E;                        // mean energy of eta in CB

    // -------------------------------------- CB time -------------------------------------- 
    Int_t fCalib_CB_Time;                                   // CB time calibration toggle
    TH2* fHCalib_CB_Time;                                   // CB time vs element
    TH2* fHCalib_CB_Time_Neut;                              // CB time (from neutral hits) vs element
    TH2* fHCalib_CB_Time_Ind;                               // CB time (from neutral hits) vs element
    
    // ----------------------------------- CB time walk ------------------------------------ 
    Int_t fCalib_CB_Walk;                                   // CB time walk calibration toggle
    Double_t fCalib_CB_Walk_Pi0_Min;                        // lower bound of the pi0 invariant mass cut 
    Double_t fCalib_CB_Walk_Pi0_Max;                        // upper bound of the pi0 invariant mass cut 
    Double_t fCalib_CB_Walk_Prompt_Min;                     // lower bound for prompt tagger hits 
    Double_t fCalib_CB_Walk_Prompt_Max;                     // upper bound for prompt tagger hits 
    Double_t fCalib_CB_Walk_BG1_Min;                        // lower bound for tagger hits in background window 1
    Double_t fCalib_CB_Walk_BG1_Max;                        // upper bound for tagger hits in background window 1
    Double_t fCalib_CB_Walk_BG2_Min;                        // lower bound for tagger hits in background window 2
    Double_t fCalib_CB_Walk_BG2_Max;                        // upper bound for tagger hits in background window 2
    Double_t fCalib_CB_Walk_MM_Min;                         // lower bound of the missing mass cut 
    Double_t fCalib_CB_Walk_MM_Max;                         // upper bound of the missing mass cut 
    TH1* fHCalib_CB_Walk_IM;                                // 2 photon invariant mass for pi0 identification
    TH2* fHCalib_CB_Walk_MM;                                // pi0 missing mass for g + p -> p + pi0 identification
    TH1* fHCalib_CB_Walk_Rand_Time_CB;                      // CB-tagger time difference for random subtraction
    TH2** fHCalib_CB_Walk_E_T;                              // time vs energy for all CB elements
    
    // ------------------------------------ TAPS energy ------------------------------------ 
    Int_t fCalib_TAPS_Energy;                               // TAPS energy calibration toggle
    TH2* fHCalib_TAPS_IM;                                   // CB-TAPS invariant mass vs element
    TH2* fHCalib_TAPS_IM_Neut;                              // CB-TAPS invariant mass (from neutral hits) vs element
    TH2* fHCalib_TAPS_IM_Neut_2Cl;                          // CB-TAPS invariant mass (from exactly 2 neutral hits) vs element
    
    // ------------------------------------- TAPS time ------------------------------------- 
    Int_t fCalib_TAPS_Time;                                 // TAPS time calibration toggle
    TH2* fHCalib_TAPS_Time;                                 // TAPS time vs element
    TH2* fHCalib_TAPS_Time_Neut;                            // TAPS time (from neutral hits) vs element
    TH2* fHCalib_TAPS_Time_Ind;                             // TAPS time (from neutral hits) vs element
    
    // ------------------------- TAPS quadratic energy correction -------------------------- 
    Int_t fCalib_TAPS_Quad;                                 // TAPS quadratic energy correction toggle
    Double_t fCalib_TAPS_Quad_Pi0_Min;                      // lower bound for the pi0 invariant mass cut
    Double_t fCalib_TAPS_Quad_Pi0_Max;                      // upper bound for the pi0 invariant mass cut
    Double_t fCalib_TAPS_Quad_Eta_Min;                      // lower bound for the eta invariant mass cut
    Double_t fCalib_TAPS_Quad_Eta_Max;                      // upper bound for the eta invariant mass cut
    TH2* fHCalib_TAPS_Quad_IM;                              // TAPS invariant mass vs element
    TH2* fHCalib_TAPS_Quad_Pi0_Mean_E;                      // mean energy of pi0 in TAPS 
    TH2* fHCalib_TAPS_Quad_Eta_Mean_E;                      // mean energy of eta in TAPS 
    
    // ------------------------------- TAPS PSA calibration -------------------------------- 
    Int_t fCalib_TAPS_PSA;                                  // TAPS PSA calibration toggle
    TH2** fHCalib_TAPS_PSAR_PSAA;                           // LG, SG radius vs angle

    // ------------------------------- TAPS LED calibration -------------------------------- 
    Int_t fCalib_TAPS_LED;                                  // TAPS LED calibration toggle
    TH2* fHCalib_TAPS_LG;                                   // LG raw hit pattern
    TH2* fHCalib_TAPS_LG_CFD;                               // LG raw hit pattern with CFD cut
    TH2* fHCalib_TAPS_LG_NOCFD;                             // LG raw hit pattern with no-CFD cut
    TH2* fHCalib_TAPS_LG_LED1;                              // LG raw hit pattern with LED1 cut
    TH2* fHCalib_TAPS_LG_LED2;                              // LG raw hit pattern with LED2 cut

    // -------------------------------------- PID Phi -------------------------------------- 
    Int_t fCalib_PID_Phi;                                   // PID Phi calibration toggle
    TH2* fHCalib_PID_CBPhi_ID;                              // CB cluster Phi vs PID element index
    TH2* fHCalib_PID_CBPhi_ID_1Cryst;                       // CB cluster (1 crystal) Phi vs PID element index
    
    // ------------------------------------- PID energy ------------------------------------ 
    Int_t fCalib_PID_Energy;                                // PID energy calibration toggle
    TH3** fHCalib_PID_dE_E;                                 // Cluster theta vs CB-PID dE vs E plots

    // ------------------------------------- PID time -------------------------------------- 
    Int_t fCalib_PID_Time;                                  // PID time calibration toggle
    TH2* fHCalib_PID_Time;                                  // PID time vs element
    TH2* fHCalib_PID_Time_Ind;                              // PID time vs element
  
    // ----------------------------- veto-crystal correlation  ----------------------------- 
    Int_t fCalib_Veto_Corr;                                 // veto-crystal correlation toggle
    TH2* fHCalib_Veto_Corr;                                 // veto hit vs TAPS crystal hit
    
    // ------------------------------------ Veto energy ------------------------------------ 
    Int_t fCalib_Veto_Energy;                               // Veto energy calibration toggle
    TH2** fHCalib_Veto_dE_E;                                // TAPS-Veto dE vs E plots

    // ------------------------------------ Veto time ------------------------------------ 
    Int_t fCalib_Veto_Time;                                 // Veto time calibration toggle
    TH2* fHCalib_Veto_Time;                                 // Veto time vs element
    TH2* fHCalib_Veto_Time_Ind;                             // Veto time vs element
     
    // ------------------------------------ tagger time ------------------------------------ 
    Int_t fCalib_Tagger_Time;                               // tagger time toggle
    TH2* fHCalib_Tagger_Time;                               // tagger + TAPS time vs tagger element for all TAPS element
    TH2* fHCalib_Tagger_Time_Neut;                          // tagger + TAPS time vs tagger element for all TAPS element (neutral hits)
    TH2* fHCalib_Tagger_Time_Ind;                           // tagger time vs tagger element
  
public:
    TA2MyCaLib(const char* name, TA2Analysis* analysis);
    virtual ~TA2MyCaLib();

    void SetConfig(Char_t* line, Int_t key);
    void PostInit();
    void ReconstructPhysics();
    
    ClassDef(TA2MyCaLib, 1)
};

#endif

