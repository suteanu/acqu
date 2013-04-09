// SVN Info: $Id: TA2MyRateEstimation.h 455 2009-06-21 19:27:15Z werthm $

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


#ifndef __TA2MyRateEstimation_h__
#define __TA2MyRateEstimation_h__

#include "TOA2RecEta2g.h"
#include "TOA2RecEta6g.h"

#include "TA2MyPhysics.h"


class TA2MyRateEstimation : public TA2MyPhysics
{

private:
    TH1F* fH_eta2g_IM;
    TH1F* fH_eta6g_raw_IM;
    TH1F* fH_eta6g_IM;
    TH1F* fH_eta6g_pi0_IM;

public:
    TA2MyRateEstimation(const char* name, TA2Analysis* analysis);
    virtual ~TA2MyRateEstimation();

    void SetConfig(Char_t* line, Int_t key);
    void PostInit();
    void ReconstructPhysics();
    
    ClassDef(TA2MyRateEstimation, 1)
};

#endif

