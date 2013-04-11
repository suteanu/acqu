// SVN Info: $Id: TOA2Event.cxx 1341 2012-09-12 14:23:39Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2Event                                                            //
//                                                                      //
// This class is used to store presorted events into a TTree.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOA2Event.h"

ClassImp(TOA2Event)


//______________________________________________________________________________
TOA2Event::TOA2Event()
    : TObject()
{
    // Constructor.

    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
    
    // init members
    fTrigger.Reset();
    fNTaggerHits = 0;
    fNParticles = 0;
    fNMCParticles = 0;   
    fRecChiSquare = 0;

    // create arrays
    fTaggerHits = new TClonesArray("TOA2TaggerHit", 60);
    fParticles = new TClonesArray("TOA2BaseDetParticle", 10);
    fMCParticles = new TClonesArray("TOA2MCParticle", 10);
}

//______________________________________________________________________________
TOA2Event::~TOA2Event()
{
    // Destructor.

    Clear();
    delete fTaggerHits;
    delete fParticles;
    delete fMCParticles;
}

//______________________________________________________________________________
void TOA2Event::SetTrigger(TOA2Trigger* trig)
{
    // Set the trigger 'trig'.
    
    fTrigger = *trig;
}

//______________________________________________________________________________
void TOA2Event::AddTaggerHit(Int_t element, Double_t time, Double_t weight)
{
    // Add a tagger hit to the list of tagger hits.

    TClonesArray& taggHit = *fTaggerHits;
    new (taggHit[fNTaggerHits++]) TOA2TaggerHit(element, time, weight);
}

//______________________________________________________________________________
void TOA2Event::AddParticle(TOA2BaseDetParticle* p)
{
    // Add a particle to the list of particles.

    TClonesArray& list = *fParticles;
    new (list[fNParticles++]) TOA2BaseDetParticle(*p);
}

//______________________________________________________________________________
void TOA2Event::AddMCParticle(TLorentzVector* p)
{
    // Add the 4-vector 'p' as an MC particle.
   
    TClonesArray& list = *fMCParticles;
    new (list[fNMCParticles++]) TOA2MCParticle(p);
}

//______________________________________________________________________________
void TOA2Event::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.
    
    fTrigger.Reset();
    fNTaggerHits = 0;
    fTaggerHits->Clear();
    fNParticles = 0;
    fParticles->Clear(); 
    fNMCParticles = 0;
    fMCParticles->Clear();
    fRecChiSquare = 0;
}

