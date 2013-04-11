// SVN Info: $Id: TOA2RecEta2g.h 925 2011-05-29 03:25:14Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEta2g                                                         //
//                                                                      //
// Class for the reconstruction of eta mesons in the A2 setup using the //
// 2 gamma decay.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecEta2g
#define OSCAR_TOA2RecEta2g

#include "TOGlobals.h"
#include "TOA2RecParticle.h"


class TOA2RecEta2g : public TOA2RecParticle
{

private:
    TOA2DetParticle* fNeutronCand;                  //-> neutron candidate
    Bool_t fUseExtChi2;                             // flag for using the extended chi square ansatz

    void FindBestEta(Int_t n, TOA2DetParticle** list, Int_t* id_1, Int_t* id_2);

public:
    TOA2RecEta2g() : TOA2RecParticle(), fNeutronCand(0), fUseExtChi2(kFALSE) { }
    TOA2RecEta2g(Int_t nParticle, Bool_t useExtChi2 = kFALSE);
    virtual ~TOA2RecEta2g() { if (fNeutronCand) delete fNeutronCand; }
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecEta2g, 1)  // Reconstructed eta mesons in the A2 setup using 2g decay
};

#endif

