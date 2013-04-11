// SVN Info: $Id: TOA2RecPi02g.h 665 2010-10-18 01:03:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Manuel Dieterle, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecPi02g                                                         //
//                                                                      //
// Class for the reconstruction of pi0 mesons in the A2 setup using the //
// 2 gamma decay.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecPi02g
#define OSCAR_TOA2RecPi02g

#include "TOGlobals.h"
#include "TOA2RecParticle.h"


class TOA2RecPi02g : public TOA2RecParticle
{

private:
    TOA2DetParticle* fNeutronCand;                  // neutron candidate

    void FindBestPi0(Int_t n, TOA2DetParticle** list, Int_t* id_1, Int_t* id_2);

public:
    TOA2RecPi02g() : TOA2RecParticle(), fNeutronCand(0) { }
    TOA2RecPi02g(Int_t nParticle);
    virtual ~TOA2RecPi02g() { if (fNeutronCand) delete fNeutronCand; }
 
    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList);

    TOA2DetParticle* GetNeutronCandidate() const { return fNeutronCand; }

    ClassDef(TOA2RecPi02g, 1)  // Reconstructed pi0 mesons in the A2 setup using 2g decay
};

#endif

