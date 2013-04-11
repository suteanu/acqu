// SVN Info: $Id: TOA2RecEta6g.cxx 1346 2012-09-12 14:33:28Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2RecEta6g                                                         //
//                                                                      //
// Class for the reconstruction of eta mesons in the A2 setup using the //
// 6 gamma decay.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOA2RecEta6g.h"

ClassImp(TOA2RecEta6g)


//______________________________________________________________________________
TOA2RecEta6g::TOA2RecEta6g(Int_t nParticle)
    : TOA2RecParticle(nParticle)
{
    // Constructor.
    
    // init members
    fNeutronCand = 0;
    fCombi6 = new TOPairCombinations(6);
    fCombi8 = new TOPairCombinations(8);
}

//______________________________________________________________________________
TOA2RecEta6g::~TOA2RecEta6g() 
{
    // Destructor.

    if (fNeutronCand) delete fNeutronCand; 
    if (fCombi6) delete fCombi6;
    if (fCombi8) delete fCombi8;
}

//______________________________________________________________________________
Bool_t TOA2RecEta6g::Reconstruct(Int_t nParticle, TOA2DetParticle** particleList)
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
    // find best eta in a subset of 6 particles
    else 
    {
        Int_t id1, id2, id3, id4, id5, id6;
        TOA2DetParticle* bestGammas[6] = {0, 0, 0, 0, 0, 0};
        
        // check number of particles
        if (nParticle == 6)
        {
            // find the best 3 pi0
            fChiSquare = FindBest3Pi0(particleList, &id1, &id2, &id3, &id4, &id5, &id6);

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
            // find the best combination to form 3 pi0
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
                Double_t chi = FindBest3Pi0(subset, &id1, &id2, &id3, &id4, &id5, &id6);
                
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
        else if (nParticle == 8)
        {
            // find the best eta
            fChiSquare = FindBest34Pi0(particleList, &id1, &id2, &id3, &id4, &id5, &id6);

            // leave if no solution was selected
            if (fChiSquare == 1e30) return kFALSE;

            // save the gammas
            bestGammas[0] = particleList[id1];
            bestGammas[1] = particleList[id2];
            bestGammas[2] = particleList[id3];
            bestGammas[3] = particleList[id4];
            bestGammas[4] = particleList[id5];
            bestGammas[5] = particleList[id6];
  
        } // if: 8 particles found
        else if (nParticle == 9)
        {
            // find the best eta
            Double_t bestChi = 1e30;
        
            // subset of 8 particles
            TOA2DetParticle* subset[8];

            // loop over all 9 subsets of 8 particles
            for (Int_t i = 0; i < 9; i++)
            {
                // fill 8 elements into array
                Int_t k = 0;
                for (Int_t j = 0; j < 9; j++) 
                {
                    if (j != i) subset[k++] = particleList[j];
                }

                // find the best combinations
                Double_t chi = FindBest34Pi0(subset, &id1, &id2, &id3, &id4, &id5, &id6);
                
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

        } // if: 9 particles found
        else if (nParticle == 10)
        {
            // find the best eta
            Double_t bestChi = 1e30;
        
            // subset of 8 particles
            TOA2DetParticle* subset[8];

            // loop over all subsets of 8 particles
            for (Int_t i = 0; i < 10; i++)
            {
                for (Int_t ii = i+1; ii < 10; ii++)
                {
                    // fill 8 elements into array
                    Int_t k = 0;
                    for (Int_t j = 0; j < 10; j++) 
                    {
                        if (j != i && j != ii) subset[k++] = particleList[j];
                    }

                    // find the best combinations
                    Double_t chi = FindBest34Pi0(subset, &id1, &id2, &id3, &id4, &id5, &id6);
                    
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
            }

            // save best chi square value
            fChiSquare = bestChi;
            
            // leave if no solution was selected
            if (fChiSquare == 1e30) return kFALSE;

        } // if: 10 particles found
        else if (nParticle == 11)
        {
            // find the best eta
            Double_t bestChi = 1e30;
        
            // subset of 8 particles
            TOA2DetParticle* subset[8];

            // loop over all subsets of 8 particles
            for (Int_t i = 0; i < 11; i++)
            {
                for (Int_t ii = i+1; ii < 11; ii++)
                {
                    for (Int_t iii = ii+1; iii < 11; iii++)
                    {
                        // fill 8 elements into array
                        Int_t k = 0;
                        for (Int_t j = 0; j < 11; j++) 
                        {
                            if (j != i && j != ii && j != iii) subset[k++] = particleList[j];
                        }

                        // find the best combinations
                        Double_t chi = FindBest34Pi0(subset, &id1, &id2, &id3, &id4, &id5, &id6);
                        
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
                }
            }

            // save best chi square value
            fChiSquare = bestChi;
            
            // leave if no solution was selected
            if (fChiSquare == 1e30) return kFALSE;

        } // if: 11 particles found
        else if (nParticle == 12)
        {
            // find the best eta
            Double_t bestChi = 1e30;
        
            // subset of 8 particles
            TOA2DetParticle* subset[8];

            // loop over all subsets of 8 particles
            for (Int_t i = 0; i < 12; i++)
            {
                for (Int_t ii = i+1; ii < 12; ii++)
                {
                    for (Int_t iii = ii+1; iii < 12; iii++)
                    {
                        for (Int_t iiii = iii+1; iiii < 12; iiii++)
                        {
                            // fill 8 elements into array
                            Int_t k = 0;
                            for (Int_t j = 0; j < 12; j++) 
                            {
                                if (j != i && j != ii && j != iii && j != iiii) subset[k++] = particleList[j];
                            }

                            // find the best combinations
                            Double_t chi = FindBest34Pi0(subset, &id1, &id2, &id3, &id4, &id5, &id6);
                            
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
                    }
                }
            }

            // save best chi square value
            fChiSquare = bestChi;
            
            // leave if no solution was selected
            if (fChiSquare == 1e30) return kFALSE;

        } // if: 12 particles found
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
Double_t TOA2RecEta6g::FindBest3Pi0(TOA2DetParticle* list[6], Int_t* p1_g1, Int_t* p1_g2,
                                                              Int_t* p2_g1, Int_t* p2_g2,
                                                              Int_t* p3_g1, Int_t* p3_g2)

{
    // Find the best solution of creating 3 pi0 out of 6 gammas.
    // The indices of the particles that make up the best 3 pi0
    // will be stored in p1_g1 to p3_g2.

    Double_t bestChi = 1e30;
    Double_t chi;
    TLorentzVector p4Gamma[6];
    TLorentzVector p4Pi0[3];
    Double_t IM[3];
    Double_t IM_err[3];
    UInt_t** c = fCombi6->GetCombinations();
    
    // calculate photon 4-vectors
    for (Int_t i = 0; i < 6; i++) list[i]->Calculate4Vector(p4Gamma+i, 0.);
 
    // loop over all combinations
    for (Int_t i = 0; i < 15; i++)
    {
        // constitute the pions
        p4Pi0[0] = p4Gamma[c[i][0]] + p4Gamma[c[i][1]];
        p4Pi0[1] = p4Gamma[c[i][2]] + p4Gamma[c[i][3]];
        p4Pi0[2] = p4Gamma[c[i][4]] + p4Gamma[c[i][5]];
        
        // calculate invariant mass
        IM[0] = p4Pi0[0].M();
        IM[1] = p4Pi0[1].M();
        IM[2] = p4Pi0[2].M();
        
        // calculate invariant mass error
        IM_err[0] = IM[0] * list[c[i][0]]->CalcChi2IMError(list[c[i][1]]);
        IM_err[1] = IM[1] * list[c[i][2]]->CalcChi2IMError(list[c[i][3]]);
        IM_err[2] = IM[2] * list[c[i][4]]->CalcChi2IMError(list[c[i][5]]);

        // calculate chi
        chi = ((TOGlobals::kPi0Mass - IM[0]) / IM_err[0]) * ((TOGlobals::kPi0Mass - IM[0]) / IM_err[0]) +
              ((TOGlobals::kPi0Mass - IM[1]) / IM_err[1]) * ((TOGlobals::kPi0Mass - IM[1]) / IM_err[1]) +
              ((TOGlobals::kPi0Mass - IM[2]) / IM_err[2]) * ((TOGlobals::kPi0Mass - IM[2]) / IM_err[2]);
        
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
    }

    return bestChi;
}

//______________________________________________________________________________
Double_t TOA2RecEta6g::FindBest34Pi0(TOA2DetParticle* list[8], Int_t* p1_g1, Int_t* p1_g2,
                                                               Int_t* p2_g1, Int_t* p2_g2,
                                                               Int_t* p3_g1, Int_t* p3_g2)
{
    // Find the best solution of creating 3 or 4 pi0 out of 8 gammas.
    // The indices of the particles that make up the best eta
    // will be stored in p1_g1 to p3_g2.

    Double_t bestChi = 1e30;
    TLorentzVector p4Gamma[8];
    TLorentzVector p4Combi[4];
    TLorentzVector* p4Pi0[4];
    Double_t IM[4];
    Int_t photon0[4];
    Int_t photon1[4];
    UInt_t** c = fCombi8->GetCombinations();
    
    // calculate photon 4-vectors
    for (Int_t i = 0; i < 8; i++) list[i]->Calculate4Vector(p4Gamma+i, 0.);
 
    //
    // loop over all combinations and find the best 3 Pi0 and 4 Pi0 solutions
    //
    
    for (Int_t i = 0; i < 105; i++)
    {
        // constitute the pions
        p4Combi[0] = p4Gamma[c[i][0]] + p4Gamma[c[i][1]];
        p4Combi[1] = p4Gamma[c[i][2]] + p4Gamma[c[i][3]];
        p4Combi[2] = p4Gamma[c[i][4]] + p4Gamma[c[i][5]];
        p4Combi[3] = p4Gamma[c[i][6]] + p4Gamma[c[i][7]];
        
        // calculate invariant mass
        IM[0] = p4Combi[0].M();
        IM[1] = p4Combi[1].M();
        IM[2] = p4Combi[2].M();
        IM[3] = p4Combi[3].M();

        // reset pion counter
        Int_t nPi0 = 0;
        
        // check if first combination is a pion
        if (IM[0] > 110 && IM[0] < 160) 
        {
            p4Pi0[nPi0] = p4Combi;
            photon0[nPi0] = c[i][0];
            photon1[nPi0] = c[i][1];
            nPi0++;
        }
        
        // check if second combination is a pion
        if (IM[1] > 110 && IM[1] < 160) 
        {
            p4Pi0[nPi0] = p4Combi + 1;
            photon0[nPi0] = c[i][2];
            photon1[nPi0] = c[i][3];
            nPi0++;
        }

        // check if third combination is a pion
        if (IM[2] > 110 && IM[2] < 160) 
        {
            p4Pi0[nPi0] = p4Combi + 2;
            photon0[nPi0] = c[i][4];
            photon1[nPi0] = c[i][5];
            nPi0++;
        }

        // check if fourth combination is a pion
        if (IM[3] > 110 && IM[3] < 160) 
        {
            p4Pi0[nPi0] = p4Combi + 3;
            photon0[nPi0] = c[i][6];
            photon1[nPi0] = c[i][7];
            nPi0++;
        }
        
        // continue if there were less than 3 pions found
        if (nPi0 < 3) continue;

        // check for best eta combination for 3 pi0
        if (nPi0 == 3)
        {
            TLorentzVector p4Eta = *p4Pi0[0] + *p4Pi0[1] + *p4Pi0[2];
            Double_t chi = (TOGlobals::kEtaMass - p4Eta.M()) * (TOGlobals::kEtaMass - p4Eta.M());
            
            // update best solution
            if (chi < bestChi)
            {
                *p1_g1 = photon0[0];
                *p1_g2 = photon1[0];
                *p2_g1 = photon0[1];
                *p2_g2 = photon1[1];
                *p3_g1 = photon0[2];
                *p3_g2 = photon1[2];
                bestChi = chi;
            }
        }
        
        // check for best eta combination for 4 pi0
        if (nPi0 == 4)
        {
            Double_t chi[4];
            TLorentzVector p4Eta_0 = *p4Pi0[0] + *p4Pi0[1] + *p4Pi0[2];
            chi[0] = (TOGlobals::kEtaMass - p4Eta_0.M()) * (TOGlobals::kEtaMass - p4Eta_0.M());
            
            TLorentzVector p4Eta_1 = *p4Pi0[0] + *p4Pi0[1] + *p4Pi0[3];
            chi[1] = (TOGlobals::kEtaMass - p4Eta_1.M()) * (TOGlobals::kEtaMass - p4Eta_1.M());
            
            TLorentzVector p4Eta_2 = *p4Pi0[0] + *p4Pi0[2] + *p4Pi0[3];
            chi[2] = (TOGlobals::kEtaMass - p4Eta_2.M()) * (TOGlobals::kEtaMass - p4Eta_2.M());
            
            TLorentzVector p4Eta_3 = *p4Pi0[1] + *p4Pi0[2] + *p4Pi0[3];
            chi[3] = (TOGlobals::kEtaMass - p4Eta_3.M()) * (TOGlobals::kEtaMass - p4Eta_3.M());
    
            // search the best solution
            Int_t bestSol = TMath::LocMin(4, chi);

            // update best solution
            if (chi[bestSol] < bestChi)
            {
                // solution 1
                if (bestSol == 0)
                {
                    *p1_g1 = photon0[0];
                    *p1_g2 = photon1[0];
                    *p2_g1 = photon0[1];
                    *p2_g2 = photon1[1];
                    *p3_g1 = photon0[2];
                    *p3_g2 = photon1[2];
                }
                // solution 2
                else if (bestSol == 1)
                {
                    *p1_g1 = photon0[0];
                    *p1_g2 = photon1[0];
                    *p2_g1 = photon0[1];
                    *p2_g2 = photon1[1];
                    *p3_g1 = photon0[3];
                    *p3_g2 = photon1[3];
                }
                // solution 3
                else if (bestSol == 2)
                {
                    *p1_g1 = photon0[0];
                    *p1_g2 = photon1[0];
                    *p2_g1 = photon0[2];
                    *p2_g2 = photon1[2];
                    *p3_g1 = photon0[3];
                    *p3_g2 = photon1[3];
                }
                // solution 4
                else if (bestSol == 3)
                {
                    *p1_g1 = photon0[1];
                    *p1_g2 = photon1[1];
                    *p2_g1 = photon0[2];
                    *p2_g2 = photon1[2];
                    *p3_g1 = photon0[3];
                    *p3_g2 = photon1[3];
                }

                bestChi = chi[bestSol];
            }
        }
    }
    
    return bestChi;
}

