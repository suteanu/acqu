// SVN Info: $Id: TOA2MCEvent.h 935 2011-06-01 23:38:53Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2MCEvent                                                          //
//                                                                      //
// This class is used to store generated A2 MC events into a TTree.     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2MCEvent
#define OSCAR_TOA2MCEvent

#include "TClass.h"
#include "TClonesArray.h"

#include "TOA2MCParticle.h"


class TOA2MCEvent : public TObject
{

protected:
    Int_t fTaggerElement;                       // tagger element
    Int_t fNParticles;                          // number of particles

    TClonesArray* fParticles;                   //-> array of particles

public:
    TOA2MCEvent();
    virtual ~TOA2MCEvent();
    
    void SetTaggerElement(Int_t element) { fTaggerElement = element; }
    void AddParticle(TLorentzVector* p);

    void Clear(Option_t* option = "");
    
    ClassDef(TOA2MCEvent, 1)  // A2 MC event class
};

#endif

