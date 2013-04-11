// SVN Info: $Id: TOA2RecEtaPi0.cxx 916 2011-05-24 16:25:38Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Markus Oberle, Alex Kaeser, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEtaPi0                                                        //
//                                                                      //
// Class for the reconstruction of eta-pi0 pairs in the A2 setup using  //
// their 2 gamma decays.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOA2RecEtaPi0.h"

ClassImp(TOA2RecEtaPi0)


//______________________________________________________________________________
TOA2RecEtaPi0::TOA2RecEtaPi0(Int_t nDetParticle)
    : TOA2RecMultiParticle(nDetParticle)
{
    // Constructor.

    // init members
    fNeutronCand = 0;
}

//______________________________________________________________________________
Bool_t TOA2RecEtaPi0::Reconstruct(Int_t nParticle, TOA2DetParticle** particleList)
{
    // Reconstruction method.

    // check the number of particles
    if (nParticle != 4 && nParticle != 5)
    {
        fNRecParticles = 0;
        fRecParticles  = 0;
        if (!nParticle) Error("Reconstruct", "Number of particles cannot be zero!");
        else Error("Reconstruct", "Number of particles must be 4 or 5!");
        return kFALSE;
    }
    else
    {
        Int_t id_1_1, id_1_2, id_2_1, id_2_2;
        TOA2DetParticle* bestGammas[4];

        // check number of particles
        if (nParticle == 4)
        {
            // find the best two pi0
            fChiSquare = FindBestEtaPi0(particleList, &id_1_1, &id_1_2, &id_2_1, &id_2_2);
            
            // save the gammas
            bestGammas[0] = particleList[id_1_1];
            bestGammas[1] = particleList[id_1_2];
            bestGammas[2] = particleList[id_2_1];
            bestGammas[3] = particleList[id_2_2];
        }
        else if (nParticle == 5)
        {
            // find the best combination to form two pi0		// eta and Pi0
            Double_t bestChi = 1e30;
            Double_t chi;
        
            // subset of 4 particles
            TOA2DetParticle* subset[4];

            // loop over all 5 subsets of 4 particles
            for (Int_t i = 0; i < 5; i++)
            {
                // fill 4 elements into array
                Int_t k = 0;
                for (Int_t j = 0; j < 5; j++) 
                {
                    if (j != i) subset[k++] = particleList[j];
                }

                // find the best combinations
                if ((chi = FindBestEtaPi0(subset, &id_1_1, &id_1_2, &id_2_1, &id_2_2)) < bestChi)
                {
                    // save currently best combination
                    bestGammas[0] = subset[id_1_1];
                    bestGammas[1] = subset[id_1_2];
                    bestGammas[2] = subset[id_2_1];
                    bestGammas[3] = subset[id_2_2];
                    bestChi = chi;
                }
            }

            // save best chi square value
            fChiSquare = bestChi;

            //
            // find neutron by comparing pointers of the particle input
            // and the detected gammas list
            //
            // loop over all particles
            for (Int_t i = 0; i < 5; i++)
            {
                Bool_t inGammaList = kFALSE;

                // loop over the gammas
                for (Int_t j = 0; j < 4; j++)
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
        
        } // if: 5 particles found
 
        // create reconstructed particle list
        fNRecParticles = 2;
        fRecParticles = new TOA2RecParticle*[2];
        
        // create first pi0
        fRecParticles[0] = new TOA2RecParticle(2);
        fRecParticles[0]->Reconstruct(2, bestGammas);

        // create second pi0
        fRecParticles[1] = new TOA2RecParticle(2);
        fRecParticles[1]->Reconstruct(2, bestGammas + 2);
        
        return kTRUE;

    } // if: 4 or 5 particles found
}

//______________________________________________________________________________
Double_t TOA2RecEtaPi0::FindBestEtaPi0(TOA2DetParticle** list,
                                       Int_t* id_1_1, Int_t* id_1_2, Int_t* id_2_1, Int_t* id_2_2)
{
    // Find the best eta-pi0 pairs by applying a chi square search for the best
    // 2g combinations in the particle list 'list' containing 'n' particles.
    // The indices of the two particles belonging to the Eta will be stored
    // in id_1_1 and id_1_2, the indices of the two particles belonging to the 
    // Pi0 will be stored in id_2_1 and id_2_2.
    
    Double_t bestChi = 1e30;
    
    Double_t chi=0;
    Double_t chi2=0;
    TLorentzVector p4Gamma[4];
    TLorentzVector p4Pi0[2];
    Double_t IM[2];
    TOPairCombinations combi(4);
    UInt_t** c = combi.GetCombinations();
    
    // calculate photon 4-vectors
    for (Int_t i = 0; i < 4; i++) list[i]->Calculate4Vector(p4Gamma+i, 0.);
 
    // loop over all combinations
    for (Int_t i = 0; i < 3; i++)
    {
        // constitute the Eta and Pi0
        p4Pi0[0] = p4Gamma[c[i][0]] + p4Gamma[c[i][1]];			// should be the Eta 
        p4Pi0[1] = p4Gamma[c[i][2]] + p4Gamma[c[i][3]];			// should be the Pi0
        
        // calculate invariant mass
        IM[0] = p4Pi0[0].M();						//should be Eta-Mass        
        IM[1] = p4Pi0[1].M();

        // calculate chi
        chi = (TOGlobals::kEtaMass - IM[0]) * (TOGlobals::kEtaMass - IM[0]) / (31.1 * 31.1) +
              (TOGlobals::kPi0Mass - IM[1]) * (TOGlobals::kPi0Mass - IM[1]) / (12.8 * 12.8);

	chi2 = (TOGlobals::kPi0Mass - IM[0]) * (TOGlobals::kPi0Mass - IM[0]) / (12.8 * 12.8) +
               (TOGlobals::kEtaMass - IM[1]) * (TOGlobals::kEtaMass - IM[1]) / (31.1 * 31.1);

        // if better solution found
        if (chi < bestChi)
        {
            *id_1_1 = c[i][0];
            *id_1_2 = c[i][1];
            *id_2_1 = c[i][2];
            *id_2_2 = c[i][3];
            bestChi = chi;
        }

	if (chi2 < bestChi)
        {
            *id_1_1 = c[i][2];
            *id_1_2 = c[i][3];
            *id_2_1 = c[i][0];
            *id_2_2 = c[i][1];
            bestChi = chi2;
        }
    }

    return bestChi;
}

