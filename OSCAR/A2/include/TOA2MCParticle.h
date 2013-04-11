// SVN Info: $Id: TOA2MCParticle.h 935 2011-06-01 23:38:53Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2MCParticle                                                       //
//                                                                      //
// This class represents an A2 MC particle.                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2MCParticle
#define OSCAR_TOA2MCParticle

#include "TClass.h"
#include "TObject.h"
#include "TLorentzVector.h"


class TOA2MCParticle : public TObject
{

protected:
    Double_t fPx;                           // x-momentum
    Double_t fPy;                           // y-momentum
    Double_t fPz;                           // z-momentum
    Double_t fE;                            // total energy

public:
    TOA2MCParticle();
    TOA2MCParticle(TLorentzVector* v);
    virtual ~TOA2MCParticle() { }
    
    ClassDef(TOA2MCParticle, 1)  // MC particle class for A2 experiments
};

#endif

