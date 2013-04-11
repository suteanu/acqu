// SVN Info: $Id: TOA2RecEtaPrime.h 1023 2011-10-11 15:36:00Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEtaPrime                                                      //
//                                                                      //
// Class for the reconstruction of eta-prime mesons in the A2 setup     //
// using the eta pi0 pi0 decay.                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecEtaPrime
#define OSCAR_TOA2RecEtaPrime

#include "TOGlobals.h"
#include "TOPairCombinations.h"
#include "TOA2RecMultiParticle.h"


class TOA2RecEtaPrime : public TOA2RecParticle
{

private:
    TOA2DetParticle* fNeutronCand;                  // neutron candidate
    TOPairCombinations* fCombi6;                    // 6 element pair combination calculation

    Double_t FindBestEtaPi0Pi0(TOA2DetParticle* list[6], Int_t* p1_g1, Int_t* p1_g2,
                                                         Int_t* p2_g1, Int_t* p2_g2,
                                                         Int_t* p3_g1, Int_t* p3_g2);

public:
    TOA2RecEtaPrime() : TOA2RecParticle(), fNeutronCand(0), fCombi6(0) { }
    TOA2RecEtaPrime(Int_t nParticle);
    virtual ~TOA2RecEtaPrime();
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecEtaPrime, 1)  // Reconstructed eta-prime mesons in the A2 setup using eta pi0 pi0 decay
};

#endif

