// SVN Info: $Id: TOA2MCParticle.cxx 935 2011-06-01 23:38:53Z werthm $

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


#include "TOA2MCParticle.h"

ClassImp(TOA2MCParticle)


//______________________________________________________________________________
TOA2MCParticle::TOA2MCParticle()
    : TObject()
{
    // Default constructor.

    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
    
    // init members
    fPx = 0;
    fPy = 0;
    fPz = 0;
    fE  = 0;
}

//______________________________________________________________________________
TOA2MCParticle::TOA2MCParticle(TLorentzVector* v)
    : TObject()
{
    // Constructor.

    // init members
    fPx = v->Px();
    fPy = v->Py();
    fPz = v->Pz();
    fE  = v->E();
}

