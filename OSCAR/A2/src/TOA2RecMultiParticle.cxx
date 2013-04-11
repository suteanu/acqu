// SVN Info: $Id: TOA2RecMultiParticle.cxx 665 2010-10-18 01:03:46Z werthm $

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


#include "TOA2RecMultiParticle.h"

ClassImp(TOA2RecMultiParticle)


//______________________________________________________________________________
TOA2RecMultiParticle::TOA2RecMultiParticle(Int_t nParticle)
{
    // Pseudo-non-empty constructor (nParticle is not really used here).
 
    // init members
    fNRecParticles = 0;
    fRecParticles  = 0;
    fChiSquare     = 0.;
}

//______________________________________________________________________________
TOA2RecMultiParticle::~TOA2RecMultiParticle()
{
    // Destructor.
    
    if (fRecParticles) 
    {
        for (Int_t i = 0; i < fNRecParticles; i++) 
        {
            // check if particle really exists before deleting
            // sometimes fNRecParticles is larger than the actually saved
            // particles in the array because the number of particles
            // is not known in the beginning
            if (fRecParticles[i]) delete fRecParticles[i];
        }
        delete [] fRecParticles;
    }
}

//______________________________________________________________________________
TOA2RecParticle* TOA2RecMultiParticle::GetReconstructedParticle(Int_t index) const
{
    // Return the reconstructed particle with index 'index'. Return 0 if there is no
    // particle with this index in the reconstructed particle list.

    // check index
    if (index < 0 || index >= fNRecParticles) 
    {
        Error("GetReconstructedParticle", "Index %d out of bounds (size: %d)", index, fNRecParticles);
        return 0;
    }

    // return particle
    return fRecParticles[index];
}

