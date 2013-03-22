// SVN Info: $Id: TA2MyTAPS.h 689 2010-11-18 16:21:43Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyTAPS                                                            //
//                                                                      //
// My customized TAPS detector class.                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef __TA2MyTAPS_h__
#define __TA2MyTAPS_h__

#include "TA2Analysis.h"   
#include "TA2Apparatus.h"   
#include "TA2MyTAPS_BaF2PWO.h"
#include "TA2PlasticVETO.h"
#include "TA2GenericDetector.h"

#include <iostream>


using namespace std;


enum {
      EChild_Det_BaF2PWO,
      EChild_Det_Veto,
};


static const Map_t kValidTAPSDetectors[] = {
    {"TA2MyTAPS_BaF2PWO",    EChild_Det_BaF2PWO  },
    {"TA2PlasticVETO",       EChild_Det_Veto     },  
    {NULL,                    -1                }
};


enum {
    ETAPSFactor = 1000
};


static const Map_t kTAPSKeys[] = {
    {"TAPS-Factor:",    ETAPSFactor },
    {NULL,                 -1       }
};


class TA2MyTAPS : public TA2Apparatus 
{

protected:
    TA2MyTAPS_BaF2PWO* fBaF2PWO;                    // combined BaF2/PWO array
    TA2PlasticVETO* fVETO;                          // charged-particle ID  
    
    Double_t fTapsFudge;                            // TAPS fudge factor
  
public:
    TA2MyTAPS(const char*, TA2System*);    
    virtual ~TA2MyTAPS();                  
    virtual void PostInit();           
    virtual TA2DataManager* CreateChild(const char*, Int_t);
    virtual void LoadVariable();       
    virtual void Cleanup();           
    virtual void SetConfig(Char_t*, Int_t);  
    virtual void Reconstruct();               
  
    ClassDef(TA2MyTAPS, 1)
};

//______________________________________________________________________________
inline void TA2MyTAPS::Cleanup( )
{
    // Clear any arrays holding variables
    TA2DataManager::Cleanup();

    /*
    if( !fNparticle ) return;
    for (Int_t i = 0; i < fNparticle; i++)
    {
        fPDG_ID[i] = 0;
    }
    fP4tot.SetXYZT(0., 0., 0., 0.);
    fNparticle = 0;
    */
}

#endif

