// SVN Info: $Id: TOMCParticle.h 669 2010-10-18 01:05:04Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOMCParticle                                                         //
//                                                                      //
// Class representing a particle in the MC simulation.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOMCParticle
#define OSCAR_TOMCParticle

#include "TParticlePDG.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TGenPhaseSpace.h"
#include "TF1.h"

#include "TOGlobals.h"


class TOMCParticle : public TObject
{

private:
    TParticlePDG* fPDG;                         // PDG particle properties
    TLorentzVector* fP4;                        // 4-vector
    Bool_t fStable;                             // stable flag for decay calculation
    Int_t fNDecay;                              // number of decay particles
    TOMCParticle** fDecay;                      //[fNDecay] list of decay particles (this class is owner)
    TF1* fMassDistr;                            // particle mass distribution (e.g. for resonances) [MeV]

public:
    TOMCParticle() : fPDG(0), fP4(0), fStable(kTRUE), fNDecay(0), fDecay(0), fMassDistr(0) { }
    TOMCParticle(TParticlePDG* inPDG, Bool_t initDecay = kTRUE);
    TOMCParticle(const Char_t* inName, Bool_t initDecay = kTRUE);
    TOMCParticle(Int_t inID, Bool_t isGeant3_ID, Bool_t initDecay = kTRUE);
    virtual ~TOMCParticle();

    Bool_t CalculateDecay();
    
    TOMCParticle* AddDecayParticle(const Char_t* inPartName);
    void SetGeant3_ID(Int_t inID);
    void SetP4(TLorentzVector* inP4) { if (fP4) *fP4 = *inP4; }
    void SetP4(Double_t inPx, Double_t inPy, Double_t inPz, Double_t inE) { if (fP4) fP4->SetPxPyPzE(inPx, inPy, inPz, inE); }
    void SetMassDistribution(TF1* inDistr) { fMassDistr = inDistr; }

    Int_t GetPDG_ID() const { return fPDG->PdgCode(); }
    Int_t GetGeant3_ID() const;
    const Char_t* GetName() const { return fPDG->GetName(); }
    Double_t GetPDGMassGeV() const { return fPDG->Mass(); }
    TLorentzVector* GetP4() const { return fP4; }
    Int_t GetTotalNDecayParticles() const;
    Int_t GetNDecayParticles() const { return fNDecay; }
    TOMCParticle* GetDecayParticle(Int_t index) const;
    TF1* GetMassDistribution() const { return fMassDistr; }

    void PrintDecayChain(Char_t* outString = 0, Int_t level = 0);

    ClassDef(TOMCParticle, 0)  // MC particle class
};

#endif

