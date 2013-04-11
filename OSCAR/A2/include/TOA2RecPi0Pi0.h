// SVN Info: $Id: TOA2RecPi0Pi0.h 665 2010-10-18 01:03:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Markus Oberle, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecPi0Pi0                                                        //
//                                                                      //
// Class for the reconstruction of two pi0 mesons in the A2 setup using //
// their 2 gamma decays.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecPi0Pi0
#define OSCAR_TOA2RecPi0Pi0

#include "TOA2RecMultiParticle.h"
#include "TOPairCombinations.h"


class TOA2RecPi0Pi0 : public TOA2RecMultiParticle
{

private:
    TOA2DetParticle* fNeutronCand;                      // neutron candidate

    Double_t FindBestPi0Pi0(TOA2DetParticle** list, 
                            Int_t* id_1_1, Int_t* id_1_2, Int_t* id_2_1, Int_t* id_2_2);

public:
    TOA2RecPi0Pi0() : TOA2RecMultiParticle(), fNeutronCand(0) { }
    TOA2RecPi0Pi0(Int_t nDetParticle);
    virtual ~TOA2RecPi0Pi0() { if (fNeutronCand) delete fNeutronCand; }
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecPi0Pi0, 1)  // Reconstructed two pi0 mesons in the A2 setup using their 2g decay
};

#endif

