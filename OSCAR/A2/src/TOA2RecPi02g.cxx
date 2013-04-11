// SVN Info: $Id: TOA2RecPi02g.cxx 1346 2012-09-12 14:33:28Z werthm $

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


#include "TOA2RecPi02g.h"

ClassImp(TOA2RecPi02g)


//______________________________________________________________________________
TOA2RecPi02g::TOA2RecPi02g(Int_t nParticle)
    : TOA2RecParticle(nParticle)
{
    // Constructor.

    // init members
    fNeutronCand = 0;
}

//______________________________________________________________________________
Bool_t TOA2RecPi02g::Reconstruct(Int_t nParticle, TOA2DetParticle** particleList)
{
    // Reconstruction method.

    // check the number of particles
    if (!nParticle)
    {
        fNDetectedProducts = 0;
        fDetectedProducts = 0;
        Error("Reconstruct", "Number of particles cannot be zero!");
        return kFALSE;
    }
    // copy the particles directly in case of 2 hits
    else if (nParticle == 2)
    {
        fNDetectedProducts = 2;
        fDetectedProducts = new TOA2DetParticle*[2];
        fDetectedProducts[0] = new TOA2DetParticle(*particleList[0]);
        fDetectedProducts[1] = new TOA2DetParticle(*particleList[1]);
        fDetectedProducts[0]->SetPDG_ID(kGamma);
        fDetectedProducts[1]->SetPDG_ID(kGamma);
        
        // calculate the 4-vector
        Calculate4Vector();

        return kTRUE;
    }
    else
    {
        // find the best combination to form a pi0
        Int_t id_1, id_2;
        FindBestPi0(nParticle, particleList, &id_1, &id_2);
        
        // leave if no solution was selected
        if (fChiSquare == 1e30) return kFALSE;
 
        // create detected particle list
        fNDetectedProducts = 2;
        fDetectedProducts = new TOA2DetParticle*[2];
        fDetectedProducts[0] = new TOA2DetParticle(*particleList[id_1]);
        fDetectedProducts[1] = new TOA2DetParticle(*particleList[id_2]);
        fDetectedProducts[0]->SetPDG_ID(kGamma);
        fDetectedProducts[1]->SetPDG_ID(kGamma);
       
        // save neutron candidate in 3 particle case
        if (nParticle == 3) fNeutronCand = new TOA2DetParticle(*particleList[3-id_1-id_2]);

        // calculate the 4-vector
        Calculate4Vector();

        return kTRUE;
    }
}

//______________________________________________________________________________
void TOA2RecPi02g::FindBestPi0(Int_t n, TOA2DetParticle** list, Int_t* id_1, Int_t* id_2)
{
    // Find the best pi0 by applying a chi square search for the best
    // 2g combination in the particle list 'list' containing 'n' particles.
    // The indices of the two particles belonging to the best combination
    // will be stored in id_1 and id_2, respectively.
    
    // check number of particles
    if (n > 20)
    {
        Error("FindBestPi0", "Number of particles cannot exceed 20!");
        return;
    }

    // calculate the 4-vectors of all particles assuming
    // they are photons
    TLorentzVector p4[20];
    for (Int_t i = 0; i < n; i++) list[i]->Calculate4Vector(p4+i, 0.);
    
    Double_t bestChi = 1e30;
    // loop over first particle
    for (Int_t i = 0; i < n; i++)
    {
        // loop over second particle
        for (Int_t j = i+1; j < n; j++)
        {
            // calculate invariant mass and its error
            Double_t im = (p4[i] + p4[j]).M();
            Double_t im_err = im * list[i]->CalcChi2IMError(list[j]);
            
            // calculate chi square
            Double_t chi = ((TOGlobals::kPi0Mass - im) / im_err) * ((TOGlobals::kPi0Mass - im) / im_err);
            
            // chi square is smaller
            if (chi < bestChi)
            {
                *id_1 = i;
                *id_2 = j;
                bestChi = chi;
            } 
        } // second particle loop
    } // first particle loop

    // save best chi square value
    fChiSquare = bestChi;
}

