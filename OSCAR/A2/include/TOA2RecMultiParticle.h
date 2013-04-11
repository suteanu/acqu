// SVN Info: $Id: TOA2RecMultiParticle.h 665 2010-10-18 01:03:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecMultiParticle                                                 //
//                                                                      //
// Base class for the reconstruction of multiple correlated particles   //
// in the A2 setup.                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2RecMultiParticle
#define OSCAR_TOA2RecMultiParticle

#include "TOA2RecParticle.h"


class TOA2RecMultiParticle : public TObject
{

protected:
    Int_t fNRecParticles;                               // number of reconstructed particles
    TOA2RecParticle** fRecParticles;                    //[fNRecParticles] pointer to the reconstructed particle list
    Double_t fChiSquare;                                // chi square value of reconstruction

public:
    TOA2RecMultiParticle() : fNRecParticles(0), fRecParticles(0), fChiSquare(0.) { }
    TOA2RecMultiParticle(Int_t nParticle);
    virtual ~TOA2RecMultiParticle();

    virtual Bool_t Reconstruct(Int_t nParticle, TOA2DetParticle** particleList) = 0;
    
    Int_t GetNReconstructedParticles() const { return fNRecParticles; }
    TOA2RecParticle** GetReconstructedParticles() const { return fRecParticles; }
    TOA2RecParticle* GetReconstructedParticle(Int_t index) const;
    Double_t GetChiSquare() const { return fChiSquare; }

    ClassDef(TOA2RecMultiParticle, 1)  // Reconstruction of multiple correlated particles in the A2 setup
};

#endif

