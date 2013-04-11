// SVN Info: $Id: TOA2RecEta6g.h 665 2010-10-18 01:03:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEta6g                                                         //
//                                                                      //
// Class for the reconstruction of eta mesons in the A2 setup using the //
// 6 gamma decay.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecEta6g
#define OSCAR_TOA2RecEta6g

#include "TOGlobals.h"
#include "TOPairCombinations.h"
#include "TOA2RecMultiParticle.h"


class TOA2RecEta6g : public TOA2RecParticle
{

private:
    TOA2DetParticle* fNeutronCand;                  // neutron candidate
    TOPairCombinations* fCombi6;                    // 6 element pair combination calculation
    TOPairCombinations* fCombi8;                    // 8 element pair combination calculation

    Double_t FindBest3Pi0(TOA2DetParticle* list[6], Int_t* p1_g1, Int_t* p1_g2,
                                                    Int_t* p2_g1, Int_t* p2_g2,
                                                    Int_t* p3_g1, Int_t* p3_g2);
    Double_t FindBest34Pi0(TOA2DetParticle* list[8], Int_t* p1_g1, Int_t* p1_g2,
                                                     Int_t* p2_g1, Int_t* p2_g2,
                                                     Int_t* p3_g1, Int_t* p3_g2);

public:
    TOA2RecEta6g() : TOA2RecParticle(), fNeutronCand(0), fCombi6(0), fCombi8(0) { }
    TOA2RecEta6g(Int_t nParticle);
    virtual ~TOA2RecEta6g();
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecEta6g, 1)  // Reconstructed eta mesons in the A2 setup using 6g decay
};

#endif

