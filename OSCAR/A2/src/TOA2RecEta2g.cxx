// SVN Info: $Id: TOA2RecEta2g.cxx 1346 2012-09-12 14:33:28Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEta2g                                                         //
//                                                                      //
// Class for the reconstruction of eta mesons in the A2 setup using the //
// 2 gamma decay.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOA2RecEta2g.h"

ClassImp(TOA2RecEta2g)


//______________________________________________________________________________
TOA2RecEta2g::TOA2RecEta2g(Int_t nParticle, Bool_t useExtChi2)
    : TOA2RecParticle(nParticle)
{
    // Constructor.

    // init members
    fNeutronCand = 0;
    fUseExtChi2 = useExtChi2;
}

//______________________________________________________________________________
Bool_t TOA2RecEta2g::Reconstruct(Int_t nParticle, TOA2DetParticle** particleList)
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
        // find the best combination to form an eta
        Int_t id_1, id_2;
        FindBestEta(nParticle, particleList, &id_1, &id_2);
        
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
void TOA2RecEta2g::FindBestEta(Int_t n, TOA2DetParticle** list, Int_t* id_1, Int_t* id_2)
{
    // Find the best eta by applying a chi square search for the best
    // 2g combination in the particle list 'list' containing 'n' particles.
    // The indices of the two particles belonging to the best combination
    // will be stored in id_1 and id_2, respectively.
    
    // check number of particles
    if (n > 20)
    {
        Error("FindBestEta", "Number of particles cannot exceed 20!");
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
            // photon energy sum cut
            if (p4[i].E() + p4[j].E() < 500) continue;

            // calculate invariant mass and its error
            Double_t im = (p4[i] + p4[j]).M();
            Double_t im_err = im * list[i]->CalcChi2IMError(list[j]);
        
            // take into account eta-neutron phi angle difference in case of a
            // reconstruction out of 3 particles
            Double_t chi;
            if (n == 3 && fUseExtChi2)
            {   
                // calculate the phi angle difference between the current eta and the neutron candidate
                Double_t phi_diff = TMath::Abs((p4[i] + p4[j]).Phi() - p4[3-i-j].Phi())*TMath::RadToDeg();
                
                // coplanarity cut
                if (phi_diff < 50) continue;

                // calculate chi square
                chi = ((TOGlobals::kEtaMass - im) / im_err) * ((TOGlobals::kEtaMass - im) / im_err);
            }
            else
            {
                // calculate chi square
                chi = ((TOGlobals::kEtaMass - im) / im_err) * ((TOGlobals::kEtaMass - im) / im_err);
            }

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

