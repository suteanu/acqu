// SVN Info: $Id: TA2MyClusterCalib.h 495 2009-08-26 12:47:01Z werthm $

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


#ifndef __TA2MyClusterCalib_h__
#define __TA2MyClusterCalib_h__

#include "TA2MyPhysics.h"


enum {
    EMCC_PHOTON_CALIB = 40000,
};


static const Map_t myClusterCalibConfigKeys[] = {
    // General keys
    {"Photon-Calib:" ,              EMCC_PHOTON_CALIB  },         // key for the photon cluster calibration
    // Termination
    {NULL        , -1           }
};


class TA2MyClusterCalib : public TA2MyPhysics
{

private:
    TH1F* fH_Photon_Energy_MC;
    
    TH1F* fH_Photon_CB_Diff_Px;
    TH1F* fH_Photon_CB_Diff_Py;
    TH1F* fH_Photon_CB_Diff_Pz;
    TH1F* fH_Photon_CB_Diff_E;
    TH1F* fH_Photon_CB_Diff_Theta;
    TH1F* fH_Photon_CB_Diff_Phi;
    TH1F* fH_Photon_CB_CorrFac_Tot;
    TH1F* fH_Photon_CB_CorrFac[180];
    TH1F* fH_Photon_CB_CorrTheta[180];
    
    TH1F* fH_Photon_TAPS_Diff_Px;
    TH1F* fH_Photon_TAPS_Diff_Py;
    TH1F* fH_Photon_TAPS_Diff_Pz;
    TH1F* fH_Photon_TAPS_Diff_E;
    TH1F* fH_Photon_TAPS_Diff_Theta;
    TH1F* fH_Photon_TAPS_Diff_Phi;
    TH1F* fH_Photon_TAPS_CorrFac_Tot;
    TH1F* fH_Photon_TAPS_CorrFac[180];
    TH1F* fH_Photon_TAPS_CorrTheta[180];
     
    TH2F* fH_Calib_CB_2g_IM;
    TH2F* fH_Calib_CB_2g_IM2;
    TH2F* fH_Calib_TAPS_1g_IM;
    TH2F* fH_Calib_TAPS_1g_IM2;

    TH2F* fH_Calib_TAPS_2g_Time;

    TH1F* fH_CB_2g_IM;
    TH1F* fH_CB_Pion_Avr_E;
    TH1F* fH_CB_Eta_Avr_E;
    
    TH1F* fH_TAPS_1g_IM;
    TH1F* fH_TAPS_2g_IM;
    TH1F* fH_TAPS_Pion_Avr_E;
    TH1F* fH_TAPS_Eta_Avr_E;

    Bool_t fIsPhotonCalib;

    Float_t fPhotonMC_Px;
    Float_t fPhotonMC_Py;
    Float_t fPhotonMC_Pz;
    Float_t fPhotonMC_Pt;
    Float_t fPhotonMC_En;

public:
    TA2MyClusterCalib(const char* name, TA2Analysis* analysis);
    virtual ~TA2MyClusterCalib();

    void SetConfig(Char_t* line, Int_t key);
    void PostInit();
    void ReconstructPhysics();
    
    ClassDef(TA2MyClusterCalib, 1)
};

#endif

