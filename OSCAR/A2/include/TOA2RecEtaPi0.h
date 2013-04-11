// SVN Info: $Id: TOA2RecEtaPi0.h 665 2010-10-18 01:03:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Markus Oberle, Alex Kaeser, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEtaPi0                                                        //
//                                                                      //
// Class for the reconstruction of eta-pi0 pairs in the A2 setup using  //
// their 2 gamma decays.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecEtaPi0
#define OSCAR_TOA2RecEtaPi0

#include "TOA2RecMultiParticle.h"
#include "TOPairCombinations.h"


class TOA2RecEtaPi0 : public TOA2RecMultiParticle
{

private:
    TOA2DetParticle* fNeutronCand;                      // neutron candidate

    Double_t FindBestEtaPi0(TOA2DetParticle** list, 
                            Int_t* id_1_1, Int_t* id_1_2, Int_t* id_2_1, Int_t* id_2_2);

public:
    TOA2RecEtaPi0() : TOA2RecMultiParticle(), fNeutronCand(0) { }
    TOA2RecEtaPi0(Int_t nDetParticle);
    virtual ~TOA2RecEtaPi0() { if (fNeutronCand) delete fNeutronCand; }
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecEtaPi0, 1)  // Reconstructed eta-pi0 pairs in the A2 setup using their 2g decay
};

#endif

