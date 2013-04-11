// SVN Info: $Id: TOMCParticle.cxx 669 2010-10-18 01:05:04Z werthm $

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


#include "TOMCParticle.h"

ClassImp(TOMCParticle)


//______________________________________________________________________________
TOMCParticle::TOMCParticle(TParticlePDG* inPDG, Bool_t initDecay)
    : TObject()
{
    // Constructor using the PDG particle definition 'inPDG'.
    // Set the random number generator seed if 'initDecay' is kTRUE (default).

    fPDG = inPDG;
    fP4 = new TLorentzVector(0., 0., 0., 0.);
    fStable = kTRUE;
    fNDecay = 0;
    fDecay = 0;
    fMassDistr = 0;

    // set machine clock based random seed to the standart
    // ROOT random number generator (used by TGenPhaseSpace)
    if (initDecay) gRandom->SetSeed(0);
}

//______________________________________________________________________________
TOMCParticle::TOMCParticle(const Char_t* inName, Bool_t initDecay)
    : TObject()
{
    // Constructor using the particle name 'inName'.
    // Set the random number generator seed if 'initDecay' is kTRUE (default).

    fPDG = TOGlobals::GetPDG_DB()->GetParticle(inName);
    fP4 = new TLorentzVector(0., 0., 0., 0.);
    fStable = kTRUE;
    fNDecay = 0;
    fDecay = 0;
    fMassDistr = 0;
    
    // set machine clock based random seed to the standart
    // ROOT random number generator (used by TGenPhaseSpace)
    if (initDecay) gRandom->SetSeed(0);
}

//______________________________________________________________________________
TOMCParticle::TOMCParticle(Int_t inID, Bool_t isGeant3_ID, Bool_t initDecay)
    : TObject()
{
    // Constructor using the PDG or the GEANT3 particle ID 'inID'.
    // Set the random number generator seed if 'initDecay' is kTRUE (default).

    if (isGeant3_ID) fPDG = TOGlobals::GetPDG_DB()->GetParticle(TOGlobals::GetPDG_DB()->ConvertGeant3ToPdg(inID));
    else fPDG = TOGlobals::GetPDG_DB()->GetParticle(inID);
    fP4 = new TLorentzVector(0., 0., 0., 0.);
    fStable = kTRUE;
    fNDecay = 0;
    fDecay = 0;
    fMassDistr = 0;
    
    // set machine clock based random seed to the standart
    // ROOT random number generator (used by TGenPhaseSpace)
    if (initDecay) gRandom->SetSeed(0);
}

//______________________________________________________________________________
TOMCParticle::~TOMCParticle()
{
    // Destructor.
 
    if (fP4) delete fP4;
    if (fNDecay) 
    {
        for (Int_t i = 0; i < fNDecay; i++) delete fDecay[i];
        delete [] fDecay;
    }
}

//______________________________________________________________________________
void TOMCParticle::SetGeant3_ID(Int_t inID)
{
    // Set the PDG ID of this particle using the GEANT3 ID 'inID'.
    
    // set ID
    fPDG = TOGlobals::GetPDG_DB()->GetParticle(TOGlobals::GetPDG_DB()->ConvertGeant3ToPdg(inID));
}

//______________________________________________________________________________
Int_t TOMCParticle::GetGeant3_ID() const 
{
    // Return the GEANT3 particle ID of this particle.
    
    // handle self-defined particles
    if (fPDG->PdgCode() == TOGlobals::kDeuteronID) return fPDG->TrackingCode();
    else return TOGlobals::GetPDG_DB()->ConvertPdgToGeant3(fPDG->PdgCode()); 
}

//______________________________________________________________________________
TOMCParticle* TOMCParticle::AddDecayParticle(const Char_t* inPartName)
{
    // Add the particle with the name 'inPartName' to the list of decay
    // particles.
    // A pointer to the added particle is returned.
    
    // translate some shortcut particle names
    Char_t partName[256];
    if (!strcmp(inPartName, "p")) strcpy(partName, "proton");
    else if (!strcmp(inPartName, "n")) strcpy(partName, "neutron");
    else if (!strcmp(inPartName, "g")) strcpy(partName, "gamma");
    else strcpy(partName, inPartName);
    
    // get particle from PDG database
    TParticlePDG* pdg_pa = TOGlobals::GetPDG_DB()->GetParticle(partName);
    
    // check if the particle exists
    if (!pdg_pa) 
    {
        Error("AddDecayParticle", "Particle '%s' was not found in the PDG database!", inPartName);
        return 0;
    }
    
    // create the particle
    TOMCParticle* part = new TOMCParticle(partName);
  
    // mark particle as unstable to trigger the decay calculation
    fStable = kFALSE;
    
    // backup the existing decay products
    TOMCParticle** backup = 0;
    if (fNDecay)
    {
        // copy the decay products
        backup = new TOMCParticle*[fNDecay];
        for (Int_t i = 0; i < fNDecay; i++) backup[i] = fDecay[i];
        
        // delete the old array
        delete [] fDecay;
    }

    // create new decay particle array
    fDecay = new TOMCParticle*[fNDecay + 1];
    
    // copy backuped particles
    for (Int_t i = 0; i < fNDecay; i++) fDecay[i] = backup[i];

    // delete the backup
    delete [] backup;

    // add new particle
    fDecay[fNDecay] = part;
    
    // increment number of decay particles
    fNDecay++;

    // return added particle
    return part;
}

//______________________________________________________________________________
Int_t TOMCParticle::GetTotalNDecayParticles() const
{
    // Returns the total number of particles of this particle's decay chain
    // (recursive method).

    // recursive search through decay chain
    Int_t npart = 0;
    for (Int_t i = 0; i < fNDecay; i++)
    {
        // get decay particle and count it
        TOMCParticle* p = GetDecayParticle(i);
        npart++;

        // count decay particles of this decay particle
        npart += p->GetTotalNDecayParticles();
    }
    
    return npart;
}

//______________________________________________________________________________
Bool_t TOMCParticle::CalculateDecay()
{
    // Calculate a phase-space decay of this particle whole decay chain
    // (recursive method). Return kFALSE if there was an error during the
    // calculation, e.g. insufficient center-of-mass energy for a decay.
    //
    // ATTENTION: All input energies are assumed to be in GeV and all output energies
    //            will be set in GeV!

    // no decay if particle is marked as stable
    if (fStable) return kTRUE;

    // set masses of decay particles
    // choose random mass from distribution if the particle is a Rootino
    // and a mass distribution was specified.
    Double_t masses[fNDecay];
    for (Int_t i = 0; i < fNDecay; i++) 
    {
        // get decay particle
        TOMCParticle* decayPart = GetDecayParticle(i);

        // check for Rootinos
        if (decayPart->GetPDG_ID()) 
        {
            masses[i] = decayPart->GetPDGMassGeV();
        }
        else
        {
            // check for mass distribution, set random mass [GeV]
            if (decayPart->GetMassDistribution()) 
            {
                masses[i] = 0.001 * decayPart->GetMassDistribution()->GetRandom();
            }
            else 
            {
                Error("CalculateDecay", "No mass distribution found for decay Rootino");
                return kFALSE;
            }
        }
    }

    // calculate decay
    TGenPhaseSpace decayGen;
    if (decayGen.SetDecay(*fP4, fNDecay, masses))
    {
        // generate decay
        decayGen.Generate();

        // set 4-vectors of decay products
        for (Int_t i = 0; i < fNDecay; i++) GetDecayParticle(i)->SetP4(decayGen.GetDecay(i));
    }
    else 
    {
        // decay could not be calculated (forbidden by kinematics)
        return kFALSE;
    }

    // calculate decay of each decay product
    for (Int_t i = 0; i < fNDecay; i++) 
    {
        // abort decay chain calculation if there was an error
        if (!GetDecayParticle(i)->CalculateDecay()) return kFALSE;
    }

    // calculation is ok here
    return kTRUE;
}

//______________________________________________________________________________
TOMCParticle* TOMCParticle::GetDecayParticle(Int_t index) const
{
    // Return the decay particle with index 'index'. Return 0 if no such 
    // particle could be found.

    // check index range
    if (index < 0 || index >= fNDecay)
    {
        Error("GetDecayParticle", "Decay particle index %d out of bounds (number of decay particles: %d)", index, fNDecay);
        return 0;
    }
    else
    {
        return fDecay[index];
    }
}

//______________________________________________________________________________
void TOMCParticle::PrintDecayChain(Char_t* outString, Int_t level)
{
    // Print the total decay chain of this particle to 'outString' or
    // to stdout if 'outString' is zero.
    // 'level' is used to keep track of the decay hierarchy 
    // (recursive method).
    
    Char_t dchain[256] = "";
    Char_t name[256];
    Bool_t toStdout = kFALSE;

    // create local string if outString is zero
    // (for stdout output)
    if (!outString) 
    {
        outString = dchain;
        toStdout = kTRUE;
    }

    // add space if needed
    if (level != 0 ) 
    {
        if (outString[strlen(outString)-1] != '[')
            strcat(outString, " ");
    }

    // add this particle, set some shortcuts for longer particle names
    if (!strcmp(GetName(), "gamma")) strcpy(name, "g");
    else if (!strcmp(GetName(), "proton")) strcpy(name, "p");
    else if (!strcmp(GetName(), "neutron")) strcpy(name, "n");
    else strcpy(name, GetName());
    strcat(outString, name);
    
    // leave if there are no further decay products
    if (!fNDecay) return;
    
    // open decay chain 
    strcat(outString, "[");

    // recursive search through decay chain
    for (Int_t i = 0; i < fNDecay; i++)
    {
        // call this method of the sub-decay products
        GetDecayParticle(i)->PrintDecayChain(outString, level+1);
    }
    
    // close decay chain
    strcat(outString, "]");

    // print everything to stdout if initial outString was zero
    if (level == 0 && toStdout) printf("%s\n", outString);
}

