// SVN Info: $Id: TOA2MCEvent.cxx 935 2011-06-01 23:38:53Z werthm $

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


#include "TOA2MCEvent.h"

ClassImp(TOA2MCEvent)


//______________________________________________________________________________
TOA2MCEvent::TOA2MCEvent()
    : TObject()
{
    // Constructor.

    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
    
    // init members
    fTaggerElement = -1;
    
    // create arrays
    fParticles = new TClonesArray("TOA2MCParticle", 6);
}

//______________________________________________________________________________
TOA2MCEvent::~TOA2MCEvent()
{
    // Destructor.

    Clear();
    delete fParticles;
}

//______________________________________________________________________________
void TOA2MCEvent::AddParticle(TLorentzVector* p)
{
    // Add the 4-vector 'p' as a particle.
   
    TClonesArray& list = *fParticles;
    new (list[fNParticles++]) TOA2MCParticle(p);
}

//______________________________________________________________________________
void TOA2MCEvent::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.
    
    fTaggerElement = -1;
    fNParticles = 0;
    fParticles->Clear();
}

