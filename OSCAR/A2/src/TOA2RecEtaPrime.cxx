// SVN Info: $Id: TOA2RecEtaPrime.cxx 1023 2011-10-11 15:36:00Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEtaPrime                                                      //
//                                                                      //
// Class for the reconstruction of eta-prime mesons in the A2 setup     //
// using the eta pi0 pi0 decay.                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOA2RecEtaPrime.h"

ClassImp(TOA2RecEtaPrime)


//______________________________________________________________________________
TOA2RecEtaPrime::TOA2RecEtaPrime(Int_t nParticle)
    : TOA2RecParticle(nParticle)
{
    // Constructor.
    
    // init members
    fNeutronCand = 0;
    fCombi6 = new TOPairCombinations(6);
}

//______________________________________________________________________________
TOA2RecEtaPrime::~TOA2RecEtaPrime() 
{
    // Destructor.

    if (fNeutronCand) delete fNeutronCand; 
    if (fCombi6) delete fCombi6;
}

//______________________________________________________________________________
Bool_t TOA2RecEtaPrime::Reconstruct(Int_t nParticle, TOA2DetParticle** particleList)
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
    // find best eta-prime in a subset of 6 particles
    else 
    {
        Int_t id1, id2, id3, id4, id5, id6;
        TOA2DetParticle* bestGammas[6] = {0, 0, 0, 0, 0, 0};
        
        // check number of particles
        if (nParticle == 6)
        {
            // find the best eta pi0 pi0
            fChiSquare = FindBestEtaPi0Pi0(particleList, &id1, &id2, &id3, &id4, &id5, &id6);

            // leave if no solution was selected
            if (fChiSquare == 1e30) return kFALSE;

            // save the gammas
            bestGammas[0] = particleList[id1];
            bestGammas[1] = particleList[id2];
            bestGammas[2] = particleList[id3];
            bestGammas[3] = particleList[id4];
            bestGammas[4] = particleList[id5];
            bestGammas[5] = particleList[id6];
        
        } // if: 6 particles found 
        else if (nParticle == 7)
        {
            // find the best combination to form eta pi0 pi0
            Double_t bestChi = 1e30;
        
            // subset of 6 particles
            TOA2DetParticle* subset[6];

            // loop over all 7 subsets of 6 particles
            for (Int_t i = 0; i < 7; i++)
            {
                // fill 6 elements into array
                Int_t k = 0;
                for (Int_t j = 0; j < 7; j++) 
                {
                    if (j != i) subset[k++] = particleList[j];
                }

                // find the best combinations
                Double_t chi = FindBestEtaPi0Pi0(subset, &id1, &id2, &id3, &id4, &id5, &id6);
                
                // continue when no solution was selected
                if (chi == 1e30) continue;

                // save the best solution
                if (chi < bestChi)
                {
                    // save currently best combination
                    bestGammas[0] = subset[id1];
                    bestGammas[1] = subset[id2];
                    bestGammas[2] = subset[id3];
                    bestGammas[3] = subset[id4];
                    bestGammas[4] = subset[id5];
                    bestGammas[5] = subset[id6];
                    bestChi = chi;
                }
            }

            // save best chi square value
            fChiSquare = bestChi;
            
            // leave if no solution was selected
            if (fChiSquare == 1e30) return kFALSE;

            //
            // find neutron by comparing pointers of the particle input
            // and the detected gammas list
            //
            // loop over all particles
            for (Int_t i = 0; i < 7; i++)
            {
                Bool_t inGammaList = kFALSE;

                // loop over the gammas
                for (Int_t j = 0; j < 6; j++)
                {
                    // check if particle element is in gamma list
                    if (particleList[i] == bestGammas[j]) 
                    {
                        inGammaList = kTRUE;
                        break;
                    }
                }

                // check if missing element in gamma list was found
                if (!inGammaList)
                {
                    fNeutronCand = new TOA2DetParticle(*particleList[i]);   
                    break;
                }
            }
        } // if: 7 particles found
        else 
        {
            Error("Reconstruct", "No reconstruction implemented for %d particles!", nParticle);
            return kFALSE;
        }

        // create detected particle list
        fNDetectedProducts = 6;
        fDetectedProducts = new TOA2DetParticle*[6];
        for (Int_t i = 0; i < 6; i++)
        {
            fDetectedProducts[i] = new TOA2DetParticle(*bestGammas[i]);
            fDetectedProducts[i]->SetPDG_ID(kGamma);
        }

        // calculate the 4-vector
        Calculate4Vector();

        return kTRUE;
    }
}

//______________________________________________________________________________
Double_t TOA2RecEtaPrime::FindBestEtaPi0Pi0(TOA2DetParticle* list[6], Int_t* p1_g1, Int_t* p1_g2,
                                                                      Int_t* p2_g1, Int_t* p2_g2,
                                                                      Int_t* p3_g1, Int_t* p3_g2)

{
    // Find the best solution of creating eta pi0 pi0 out of 6 gammas.
    // The indices of the particles that make up the best eta pi0 pi0
    // will be stored in p1_g1 to p3_g2.

    Double_t bestChi = 1e30;
    Double_t chi;
    TLorentzVector p4Gamma[6];
    TLorentzVector p4Meson[3];
    Double_t IM[3];
    UInt_t** c = fCombi6->GetCombinations();
    
    // calculate photon 4-vectors
    for (Int_t i = 0; i < 6; i++) list[i]->Calculate4Vector(p4Gamma+i, 0.);
 
    // loop over all combinations
    for (Int_t i = 0; i < 15; i++)
    {
        // constitute the mesons
        p4Meson[0] = p4Gamma[c[i][0]] + p4Gamma[c[i][1]];
        p4Meson[1] = p4Gamma[c[i][2]] + p4Gamma[c[i][3]];
        p4Meson[2] = p4Gamma[c[i][4]] + p4Gamma[c[i][5]];
        
        // calculate invariant mass
        IM[0] = p4Meson[0].M();
        IM[1] = p4Meson[1].M();
        IM[2] = p4Meson[2].M();

        // 
        // calculate chi for eta pi0 pi0
        //

        chi = (TOGlobals::kEtaMass - IM[0]) * (TOGlobals::kEtaMass - IM[0]) +
              (TOGlobals::kPi0Mass - IM[1]) * (TOGlobals::kPi0Mass - IM[1]) +
              (TOGlobals::kPi0Mass - IM[2]) * (TOGlobals::kPi0Mass - IM[2]);
        
        // if better solution found
        if (chi < bestChi)
        {
            *p1_g1 = c[i][0];
            *p1_g2 = c[i][1];
            *p2_g1 = c[i][2];
            *p2_g2 = c[i][3];
            *p3_g1 = c[i][4];
            *p3_g2 = c[i][5];
            bestChi = chi;
        }

        // 
        // calculate chi for pi0 eta pi0
        //

        chi = (TOGlobals::kPi0Mass - IM[0]) * (TOGlobals::kPi0Mass - IM[0]) +
              (TOGlobals::kEtaMass - IM[1]) * (TOGlobals::kEtaMass - IM[1]) +
              (TOGlobals::kPi0Mass - IM[2]) * (TOGlobals::kPi0Mass - IM[2]);
        
        // if better solution found
        if (chi < bestChi)
        {
            *p1_g1 = c[i][2];
            *p1_g2 = c[i][3];
            *p2_g1 = c[i][0];
            *p2_g2 = c[i][1];
            *p3_g1 = c[i][4];
            *p3_g2 = c[i][5];
            bestChi = chi;
        }

        // 
        // calculate chi for pi0 pi0 eta
        //

        chi = (TOGlobals::kPi0Mass - IM[0]) * (TOGlobals::kPi0Mass - IM[0]) +
              (TOGlobals::kPi0Mass - IM[1]) * (TOGlobals::kPi0Mass - IM[1]) +
              (TOGlobals::kEtaMass - IM[2]) * (TOGlobals::kEtaMass - IM[2]);
        
        // if better solution found
        if (chi < bestChi)
        {
            *p1_g1 = c[i][4];
            *p1_g2 = c[i][5];
            *p2_g1 = c[i][2];
            *p2_g2 = c[i][3];
            *p3_g1 = c[i][0];
            *p3_g2 = c[i][1];
            bestChi = chi;
        }
    }

    return bestChi;
}

