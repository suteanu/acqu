// SVN Info: $Id: TOA2RecK0Lambda.h 933 2011-06-01 13:51:21Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecK0Lambda                                                      //
//                                                                      //
// Class for the reconstruction of K0 Lambda in the A2 setup using the  //
// 6 gamma decay.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecK0Lambda
#define OSCAR_TOA2RecK0Lambda

#include "TOGlobals.h"
#include "TOPairCombinations.h"
#include "TOA2RecParticle.h"


class TOA2RecK0Lambda : public TOA2RecParticle
{

private:
    TOA2DetParticle* fNeutronCand;                  // neutron candidate
    TOPairCombinations* fCombi6;                    // 6 element pair combination calculation

    Double_t FindBest3Pi0(TOA2DetParticle* list[6], Int_t* p1_g1, Int_t* p1_g2,
                                                    Int_t* p2_g1, Int_t* p2_g2,
                                                    Int_t* p3_g1, Int_t* p3_g2);

public:
    TOA2RecK0Lambda() : TOA2RecParticle(), fNeutronCand(0), fCombi6(0) { }
    TOA2RecK0Lambda(Int_t nParticle);
    virtual ~TOA2RecK0Lambda();
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecK0Lambda, 1)  // Reconstructed K0 Lambda in the A2 setup 
};

#endif

