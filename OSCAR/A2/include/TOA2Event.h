// SVN Info: $Id: TOA2Event.h 1341 2012-09-12 14:23:39Z werthm $

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


#ifndef OSCAR_TOA2Event
#define OSCAR_TOA2Event

#include "TClass.h"
#include "TClonesArray.h"

#include "TOA2Trigger.h"
#include "TOA2TaggerHit.h"
#include "TOA2BaseDetParticle.h"
#include "TOA2MCParticle.h"


class TOA2Event : public TObject
{

protected:
    TOA2Trigger fTrigger;                       // trigger information
    Int_t fNTaggerHits;                         // number of tagger hits
    Int_t fNParticles;                          // number of detected particles
    Int_t fNMCParticles;                        // number of MC particles
    Double_t fRecChiSquare;                     // reconstruction chi square value

    TClonesArray* fTaggerHits;                  //-> array of tagger hits 
    TClonesArray* fParticles;                   //-> array of detected particles
    TClonesArray* fMCParticles;                 //-> array of MC particles

public:
    TOA2Event();
    virtual ~TOA2Event();
    
    void SetTrigger(TOA2Trigger* trig);
    void AddTaggerHit(Int_t element, Double_t time, Double_t weight);
    void AddParticle(TOA2BaseDetParticle* p);
    void AddMCParticle(TLorentzVector* p);
    void SetRecChiSquare(Double_t chi2) { fRecChiSquare = chi2; }

    void Clear(Option_t* option = "");
    
    ClassDef(TOA2Event, 1)  // A2 event class
};

#endif

