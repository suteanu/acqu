//--Author	Dave H   26 June 2013
//
//				Based on TA2Cristina
//

#include "TA2Pi0Compton.h"

enum { EInput = 1000};
static const Map_t kInputs[] = {
	{"Input:",	EInput},
	{NULL,          -1}
};

ClassImp(TA2Pi0Compton)

//-----------------------------------------------------------------------------
TA2Pi0Compton::TA2Pi0Compton( const char* name, TA2Analysis* analysis )
	:TA2Physics( name, analysis ) 
{
// Initialise Detectors
	fTAGG			= NULL; // Tagger
	fCB			= NULL; // CB (PID, MWPC, NaI)
	fTAPS			= NULL; // TAPS

	fNaI			= NULL; // CB
	fMWPC			= NULL; // MWPC
	fPID			= NULL; // PID

	fBaF2			= NULL;	// BaF2
	fVeto			= NULL; // TAPS Vetos

// Physics Variables
	fBasicVariable 		= 0;

	// Particle Counters
	fNPhotTemp		= 0;
	fNPhoton		= 0;
	fNProton		= 0;
	fNPi0			= 0;
	fNUnknown		= 0;	

	// Particle arrays
	fTaggedPhoton		= NULL;
	fPhotTemp		= NULL;
	fPhoton 		= NULL;	
	fProton			= NULL;
	fPi0			= NULL;
	fUnknown		= NULL;

	fPhotonEnergy		= NULL;
	fPhotonTheta		= NULL;
	fPhotonPhi		= NULL;
	fPhotonTime		= NULL;

	fProtonEnergy		= NULL;
	fProtonTheta		= NULL;
	fProtonPhi		= NULL;
	fProtonTime		= NULL;

	fPi0Energy		= NULL;
	fPi0Theta		= NULL;
	fPi0Phi			= NULL;
	fPi0Time		= NULL;

	fIsPionPhoton		= NULL;
	fNPionPhoton		= 0;

	fNTagg			= 0;
	fTaggerChannel		= NULL;
	fTaggerTime		= NULL;

	fNPrompt 		= 0;
	fNRandom		= 0;
	fNTaggNPhot		= 0;

	fNPromptPi0		= 0;
	fNRandomPi0		= 0;
	fNTaggNPi0		= 0;

	// photon and pi0 physics reconstruction variables
	f2PhotonInvariantMass	= NULL;
	fTaggerPhotonTime	= NULL;
	fTaggerPi0Time		= NULL;

	fTaggerChannelPrompt	= NULL;
	fTaggerChannelRandom	= NULL;
	fMissingMassPrompt	= NULL;
	fMissingMassRandom	= NULL;
	fPhotonThetaPrompt	= NULL;
	fPhotonThetaRandom	= NULL;
	fPhotonPhiPrompt	= NULL;
	fPhotonPhiRandom	= NULL;

	fTaggerChannelPromptPi0	= NULL;
	fTaggerChannelRandomPi0	= NULL;
	fMissingMassPromptPi0	= NULL;
	fMissingMassRandomPi0	= NULL;
	fPi0ThetaPrompt		= NULL;
	fPi0ThetaRandom		= NULL;
	fPi0PhiPrompt		= NULL;
	fPi0PhiRandom		= NULL;


	AddCmdList(kInputs);
}


//-----------------------------------------------------------------------------
TA2Pi0Compton::~TA2Pi0Compton()
{

// Delete Tree Files

// Shut off for now
//	delete fCristinaTree;
//	delete fCristinaFile;

}
	
//-----------------------------------------------------------------------------
void TA2Pi0Compton::SetConfig(Char_t* line, Int_t key)
{
	// Any special command-line input for Crystal Ball apparatus

	switch (key){
		case EInput:
			//  Invariant mass limits
			if( sscanf( line, "%lf", &fInput ) != 1 ){
				PrintError( line, "<OOOOPS...>");
				return;
			}
			break;
		default:
		// default main apparatus SetConfig()
		TA2Physics::SetConfig( line, key );
		break;
	}
}

//---------------------------------------------------------------------------
void TA2Pi0Compton::PostInit()
{

// Introduce Detectors
	printf("\n");

	// Tagger
	fTAGG = (TA2Tagger*)((TA2Analysis*)fParent)->GetChild("TAGG");
	if ( !fTAGG) PrintError("","<No Tagger class found>",EErrFatal);
	else {  printf("Tagger included in analysis\n");
		fTAGGParticles = fTAGG->GetParticles(); }

	// Ladder
	fLADD = (TA2Ladder*)((TA2Analysis*)fParent)->GetGrandChild( "FPD");
	if ( !fLADD) PrintError( "", "<No Ladder class found>", EErrFatal);
//	else printf(" - Focal plane included in analysis\n\n");

	// Central Apparatus
	fCB = (TA2CrystalBall*)((TA2Analysis*)fParent)->GetChild("CB");
	if (!fCB) PrintError( "", "<No Central Apparatus/CB class found>", EErrFatal);
	else {  printf("CB system included in analysis\n");
		fCBParticles  = fCB->GetParticles(); }

	// NaI
//	fNaI = (TA2CalArray*)((TA2Analysis*)fParent)->GetGrandChild("NaI");
//	if (!fNaI) printf(" - NaI NOT included in analysis\n");
//	else printf(" - NaI included in analysis\n");

	// Pid
//	fPID = (TA2PlasticPID*)((TA2Analysis*)fParent)->GetGrandChild("PID");
//	if (!fPID) printf(" - PID NOT included in analysis\n");
//	else printf(" - PID included in analysis\n");

	// Mwpc
//	fMWPC = (TA2CylMwpc*)((TA2Analysis*)fParent)->GetGrandChild("CylMWPC");
//	if (!fMWPC) printf(" - Wire Chambers NOT included in analysis\n\n");
//	else printf(" - Wire Chambers included in analysis\n\n");

	// TAPS
	fTAPS = (TA2Taps*)((TA2Analysis*)fParent)->GetChild("TAPS");
	if ( !fTAPS) printf("TAPS *NOT* included in analysis\n");
	else {  printf("TAPS included in analysis\n");
		fTAPSParticles = fTAPS->GetParticles(); }

	// BaF2
//	fBaF2 = (TA2TAPS_BaF2*)((TA2Analysis*)fParent)->GetGrandChild("BaF2");
//	if (!fBaF2) printf(" - BaF2 NOT included in analysis\n\n");
//	else printf(" - BaF2 included in analysis\n");

	// Vetos
 //	fVeto = (TA2PlasticPID*)((TA2Analysis*)fParent)->GetGrandChild("VetoBaF2");
//	if (!fVeto) printf(" - BaF2 Vetos NOT included in analysis\n\n");
//	else printf(" - BaF2 Vetos included in analysis\n\n");


// Get max # of Particles from detectors, used for defining array sizes

	fCBMaxNParticle 	= fCB->GetMaxParticle();	
	if (fTAPS) 						
	fTAPSMaxNParticle	= fTAPS->GetMaxParticle(); 
	else fTAPSMaxNParticle 	= 0;

	fMaxNParticle		= fCBMaxNParticle + fTAPSMaxNParticle;  

// Create arrays to hold Particles

	fPhotTemp		= new TA2Particle*[fMaxNParticle];
	fPhoton 		= new TA2Particle*[fMaxNParticle];	
	fProton			= new TA2Particle*[fMaxNParticle];

	Int_t squareMax		= fMaxNParticle*fMaxNParticle*2;
	fPi0	 		= new TA2Particle*[squareMax];
	TA2Particle* part	= new TA2Particle[squareMax];
	for ( i = 0; i < squareMax; i++) fPi0[i] = part + i;

	fUnknown		= new TA2Particle*[fMaxNParticle];
	fTaggedPhoton		= new TA2Particle*[352];

	fPhotonEnergy		= new Double_t[fMaxNParticle];
	fPhotonTheta		= new Double_t[fMaxNParticle];
	fPhotonPhi		= new Double_t[fMaxNParticle];
	fPhotonTime		= new Double_t[fMaxNParticle];

	fProtonEnergy		= new Double_t[fMaxNParticle];
	fProtonTheta		= new Double_t[fMaxNParticle];
	fProtonPhi		= new Double_t[fMaxNParticle];
	fProtonTime		= new Double_t[fMaxNParticle];

	fPi0Energy		= new Double_t[fMaxNParticle];
	fPi0Theta		= new Double_t[fMaxNParticle];
	fPi0Phi			= new Double_t[fMaxNParticle];
	fPi0Time		= new Double_t[fMaxNParticle];

	fIsPionPhoton		= new Bool_t[fMaxNParticle];
	f2PhotonInvariantMass	= new Double_t[squareMax];

	fTaggerTime		= new Double_t[352];
	fTaggerPhotonTime	= new Double_t[352*fMaxNParticle];
	fTaggerPi0Time		= new Double_t[352*fMaxNParticle];
	fTaggerChannel		= new Int_t[352*5];

	fTaggerChannelPrompt 	= new Int_t[352*fMaxNParticle*fMaxNParticle];
	fTaggerChannelRandom 	= new Int_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassPrompt	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassRandom	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonThetaPrompt	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonThetaRandom	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonPhiPrompt	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonPhiRandom	= new Double_t[352*fMaxNParticle*fMaxNParticle];

	fTaggerChannelPromptPi0 = new Int_t[352*fMaxNParticle*fMaxNParticle];
	fTaggerChannelRandomPi0 = new Int_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassPromptPi0	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassRandomPi0	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0ThetaPrompt		= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0ThetaRandom		= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0PhiPrompt		= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0PhiRandom		= new Double_t[352*fMaxNParticle*fMaxNParticle];

/*
// Create Tree Files, Define Branches
	fCristinaFile = new TFile("TA2Pi0Compton.root", "RECREATE", "Cristina", 3);
	fCristinaTree = new TTree("TA2Pi0ComptonTree", "Compton Kinematics");
	fCristinaTree->Branch("BasicVariable",	&fBasicVariable,"BasicVariable/I");
	fCristinaTree->Branch("NPhotTemp",	&fNPhotTemp, 	"NPhotTemp/I");
	fCristinaTree->Branch("NPhoton",	&fNPhoton, 	"NPhoton/I");
	fCristinaTree->Branch("NProton",	&fNProton, 	"NProton/I");
	fCristinaTree->Branch("NPi0",		&fNPi0, 	"NPi0/I");
	fCristinaTree->Branch("NUnknown",	&fNUnknown, 	"NUnknown/I");
	fCristinaTree->Branch("NPionPhoton",	&fNPionPhoton, 	"NPionPhoton/I");

	fCristinaTree->Branch("PhotonEnergy",	fPhotonEnergy,	"PhotonEnergy[NPhoton]/D");
	fCristinaTree->Branch("PhotonTheta",	fPhotonTheta, 	"PhotonTheta[NPhoton]/D");
	fCristinaTree->Branch("PhotonPhi",	fPhotonPhi, 	"PhotonPhi[NPhoton]/D");
	fCristinaTree->Branch("PhotonTime",	fPhotonTime, 	"PhotonTime[NPhoton]/D");

	fCristinaTree->Branch("ProtonEnergy",	fProtonEnergy,	"ProtonEnergy[NProton]/D");
	fCristinaTree->Branch("ProtonTheta",	fProtonTheta, 	"ProtonTheta[NProton]/D");
	fCristinaTree->Branch("ProtonPhi",	fProtonPhi, 	"ProtonPhi[NProton]/D");
	fCristinaTree->Branch("ProtonTime",	fProtonTime, 	"ProtonTime[NProton]/D");

	fCristinaTree->Branch("Pi0Energy",	fPi0Energy,	"Pi0Energy[NPi0]/D");
	fCristinaTree->Branch("Pi0Theta",	fPi0Theta, 	"Pi0Theta[NPi0]/D");
	fCristinaTree->Branch("Pi0Phi",		fPi0Phi, 	"Pi0Phi[NPi0]/D");
	fCristinaTree->Branch("Pi0Time",	fPi0Time, 	"Pi0Time[NPi0]/D");

	fCristinaTree->Branch("NTagg",		&fNTagg,	"NTagg/I");
	fCristinaTree->Branch("TaggerChannel",	fTaggerChannel,	"TaggerChannel[NTagg]/I");

	fCristinaTree->Branch("NPrompt",	&fNPrompt, 	"NPrompt/I");
	fCristinaTree->Branch("NRandom",	&fNRandom, 	"NRandom/I");
	fCristinaTree->Branch("NTaggNPhot",	&fNTaggNPhot,	"NTaggNPhot/I");

	fCristinaTree->Branch("NPromptPi0",	&fNPromptPi0, 	"NPromptPi0/I");
	fCristinaTree->Branch("NRandomPi0",	&fNRandomPi0, 	"NRandomPi0/I");
	fCristinaTree->Branch("NTaggNPi0",	&fNTaggNPi0,	"NTaggNPi0/I");

	fCristinaTree->Branch("TaggerTime",  		fTaggerTime,   		"TaggerTime[NTagg]/D");
	fCristinaTree->Branch("TaggerPhotonTime",  	fTaggerPhotonTime,   	"TaggerPhotonTime[NTaggNPhot]/D");
	fCristinaTree->Branch("TaggerPi0Time",	   	fTaggerPi0Time,      	"TaggerPi0Time[NTaggNPi0]/D");

	fCristinaTree->Branch("N2PhotonInvariantMass", 	&fN2PhotonInvariantMass,"N2PhotonInvariantMass/I");
	fCristinaTree->Branch("2PhotonInvariantMass",  	f2PhotonInvariantMass,  "2PhotonInvariantMass[N2PhotonInvariantMass]/D");

	fCristinaTree->Branch("TaggerChannelPrompt",	fTaggerChannelPrompt,	"TaggerChannelPrompt[NPrompt]/I");
	fCristinaTree->Branch("TaggerChannelRandom",	fTaggerChannelRandom,	"TaggerChannelRandom[NRandom]/I");
	fCristinaTree->Branch("MissingMassPrompt",	fMissingMassPrompt,	"MissingMassPrompt[NPrompt]/D");
	fCristinaTree->Branch("MissingMassRandom",	fMissingMassRandom,	"MissingMassRandom[NRandom]/D");
	fCristinaTree->Branch("PhotonThetaPrompt",	fPhotonThetaPrompt, 	"PhotonThetaPrompt[NPrompt]/D");
	fCristinaTree->Branch("PhotonThetaRandom",	fPhotonThetaRandom, 	"PhotonThetaRandom[NRandom]/D");
	fCristinaTree->Branch("PhotonPhiPrompt",	fPhotonPhiPrompt, 	"PhotonPhiPrompt[NPrompt]/D");
	fCristinaTree->Branch("PhotonPhiRandom",	fPhotonPhiRandom, 	"PhotonPhiRandom[NRandom]/D");

	fCristinaTree->Branch("TaggerChannelPromptPi0",	fTaggerChannelPromptPi0,"TaggerChannelPromptPi0[NPromptPi0]/I");
	fCristinaTree->Branch("TaggerChannelRandomPi0",	fTaggerChannelRandomPi0,"TaggerChannelRandomPi0[NRandomPi0]/I");
	fCristinaTree->Branch("MissingMassPromptPi0",	fMissingMassPromptPi0,	"MissingMassPromptPi0[NPromptPi0]/D");
	fCristinaTree->Branch("MissingMassRandomPi0",	fMissingMassRandomPi0,	"MissingMassRandomPi0[NRandomPi0]/D");
	fCristinaTree->Branch("Pi0ThetaPrompt",		fPi0ThetaPrompt, 	"Pi0ThetaPrompt[NPromptPi0]/D");
	fCristinaTree->Branch("Pi0ThetaRandom",		fPi0ThetaRandom, 	"Pi0ThetaRandom[NRandomPi0]/D");
	fCristinaTree->Branch("Pi0PhiPrompt",		fPi0PhiPrompt, 		"Pi0PhiPrompt[NPromptPi0]/D");
	fCristinaTree->Branch("Pi0PhiRandom",		fPi0PhiRandom, 		"Pi0PhiRandom[NRandomPi0]/D");

	gROOT->cd();
*/
	
	// Default physics initialisation
	TA2Physics::PostInit();
}

//-----------------------------------------------------------------------------
void TA2Pi0Compton::LoadVariable( )
{

// Input name - variable pointer associations for any subsequent cut/histogram setup

	TA2Physics::LoadVariable();
	TA2DataManager::LoadVariable("BasicVariable", 		&fBasicVariable, 		EISingleX);

	TA2DataManager::LoadVariable("NPhoton", 		&fNPhoton,			EISingleX);
	TA2DataManager::LoadVariable("PhotonTheta", 		fPhotonTheta,			EDMultiX);
	TA2DataManager::LoadVariable("PhotonPhi", 		fPhotonPhi,			EDMultiX);
	TA2DataManager::LoadVariable("PhotonEnergy", 		fPhotonEnergy,			EDMultiX);
	TA2DataManager::LoadVariable("PhotonTime", 		fPhotonTime,			EDMultiX);

	TA2DataManager::LoadVariable("NProton", 		&fNProton,			EISingleX);
	TA2DataManager::LoadVariable("ProtonTheta", 		fProtonTheta,			EDMultiX);
	TA2DataManager::LoadVariable("ProtonPhi", 		fProtonPhi,			EDMultiX);
	TA2DataManager::LoadVariable("ProtonEnergy", 		fProtonEnergy,			EDMultiX);
	TA2DataManager::LoadVariable("ProtonTime", 		fProtonTime,			EDMultiX);

	TA2DataManager::LoadVariable("NPi0", 			&fNPi0,				EISingleX);
	TA2DataManager::LoadVariable("Pi0Theta", 		fPi0Theta,			EDMultiX);
	TA2DataManager::LoadVariable("Pi0Phi", 			fPi0Phi,			EDMultiX);
	TA2DataManager::LoadVariable("Pi0Energy", 		fPi0Energy,			EDMultiX);
	TA2DataManager::LoadVariable("Pi0Time", 		fPi0Time,			EDMultiX);

	TA2DataManager::LoadVariable("2PhotonInvariantMass", 	f2PhotonInvariantMass,		EDMultiX);

	TA2DataManager::LoadVariable("TaggerChannel",		fTaggerChannel,			EIMultiX);

	TA2DataManager::LoadVariable("TaggerTime",		fTaggerTime,			EDMultiX);
	TA2DataManager::LoadVariable("TaggerPhotonTime",	fTaggerPhotonTime,		EDMultiX);
	TA2DataManager::LoadVariable("TaggerPi0Time",		fTaggerPi0Time,			EDMultiX);

	TA2DataManager::LoadVariable("TaggerChannelPrompt",	fTaggerChannelPrompt,		EIMultiX);
	TA2DataManager::LoadVariable("TaggerChannelRandom",	fTaggerChannelRandom,		EIMultiX);
	TA2DataManager::LoadVariable("MissingMassPrompt",	fMissingMassPrompt,		EDMultiX);
	TA2DataManager::LoadVariable("MissingMassRandom",	fMissingMassRandom,		EDMultiX);
	TA2DataManager::LoadVariable("PhotonThetaPrompt",	fPhotonThetaPrompt, 		EDMultiX);
	TA2DataManager::LoadVariable("PhotonThetaRandom",	fPhotonThetaRandom, 		EDMultiX);
	TA2DataManager::LoadVariable("PhotonPhiPrompt",		fPhotonPhiPrompt, 		EDMultiX);
	TA2DataManager::LoadVariable("PhotonPhiRandom",		fPhotonPhiRandom, 		EDMultiX);

	TA2DataManager::LoadVariable("TaggerChannelPromptPi0",	fTaggerChannelPromptPi0,	EIMultiX);
	TA2DataManager::LoadVariable("TaggerChannelRandomPi0",	fTaggerChannelRandomPi0,	EIMultiX);
	TA2DataManager::LoadVariable("MissingMassPromptPi0",	fMissingMassPromptPi0,		EDMultiX);
	TA2DataManager::LoadVariable("MissingMassRandomPi0",	fMissingMassRandomPi0,		EDMultiX);
	TA2DataManager::LoadVariable("Pi0ThetaPrompt",		fPi0ThetaPrompt, 		EDMultiX);
	TA2DataManager::LoadVariable("Pi0ThetaRandom",		fPi0ThetaRandom, 		EDMultiX);
	TA2DataManager::LoadVariable("Pi0PhiPrompt",		fPi0PhiPrompt, 			EDMultiX);
	TA2DataManager::LoadVariable("Pi0PhiRandom",		fPi0PhiRandom, 			EDMultiX);


	return;
}

//-----------------------------------------------------------------------------
void TA2Pi0Compton::Reconstruct() 
{
// Get # of Particles from detectors

	fTAGGNParticle 		= fTAGG->GetNparticle();	
	fCBNParticle 		= fCB->GetNparticle();
	if (fTAPS) 					
	fTAPSNParticle		= fTAPS->GetNparticle(); 
	else fTAPSNParticle 	= 0;

// Sort according to Particle type
	
	fNPhotTemp		= 0;
	fNPhoton		= 0;
	fNProton		= 0;
	fNPi0			= 0;
	fNUnknown		= 0;	

	// CentAPP
	for ( i = 0; i < fCBNParticle; i++ ) {

		switch( (fCBParticles+i)->GetParticleID() ) { // Get PDG code

		case kGamma:                               	// Identified as a Photon
		fPhotTemp[fNPhotTemp] 	= fCBParticles+i;       // Add to Photon Array
		fNPhotTemp++;					// Increase Photon counter
		break;

		case kProton:                               	// Identified as a Proton
		fProton[fNProton]	= fCBParticles+i;       // Add to Proton Array
		fNProton++;					// Increase Proton counter
		break;

		default:
		fUnknown[fNUnknown]   	= fCBParticles+i;    	// include in "Unknown" list
		fNUnknown++; 					// Increase "Unknown" counter

		}						// End switch/case function
	}							// End # of Particle Loop

	// TAPS
	if(fTAPS) {
	for ( i = 0; i < fTAPSNParticle; i++ ) {

		switch( (fTAPSParticles+i)->GetParticleID() ) { // Get PDG code

		case kGamma:                               	// Identified as a Photon
	        fPhotTemp[fNPhotTemp] 	= fTAPSParticles+i;     // Add to Photon Array
	        fNPhotTemp++;					// Increase Photon counter
	        break;

		case kProton:                               	// Identified as a Proton
	        fProton[fNProton] 	= fTAPSParticles+i;     // Add to Proton Array
	        fNProton++;					// Increase Proton counter
	        break;

		default:
	        fUnknown[fNUnknown] 	= fTAPSParticles+i;    	// include in "Unknown" list
	        fNUnknown++; 					// Increase "Unknown" counter

	        }						// End switch/case function
	}							// End # of Particle Loop
	}							// End If(fTAPS)

// Reconstruct Pions from detected photons

	for (i = 0; i < fMaxNParticle; i++) { fIsPionPhoton[i] = kFALSE; }

	TLorentzVector p4;
	Double_t time;
	fN2PhotonInvariantMass = 0;

	for ( i = 0; i< fNPhotTemp; i++) {
		TA2Particle photon1 	= *fPhotTemp[i];

		for ( j = (i+1); j< fNPhotTemp; j++) {
			TA2Particle photon2 	= *fPhotTemp[j];

			p4 = photon1.GetP4() + photon2.GetP4();	

			f2PhotonInvariantMass[fN2PhotonInvariantMass] = p4.M();
			fN2PhotonInvariantMass++;

			if (p4.M() > 120 && p4.M() < 150) {

				time = (photon1.GetTime() + photon2.GetTime())/2;

				(*fPi0[fNPi0]).SetP4(p4);
				(*fPi0[fNPi0]).SetTime(time);

				TA2Particle pi0 	= *fPi0[fNPi0];
				
				fPi0Energy[fNPi0]	= pi0.GetT();
				fPi0Theta[fNPi0]	= pi0.GetThetaDg();
				fPi0Phi[fNPi0]		= pi0.GetPhiDg();
				fPi0Time[fNPi0]		= pi0.GetTime();

				fNPi0++;
				fIsPionPhoton[i] = kTRUE;
				fIsPionPhoton[j] = kTRUE;
			}
		}
	}


// Fill Proton and Photon properties (remove pion photons from list) 

	fNPionPhoton = 0;

	for ( i = 0; i< fNPhotTemp; i++) {
	    if (fIsPionPhoton[i] == kFALSE) {
		TA2Particle photon 	= *fPhotTemp[i];
		fPhoton[fNPhoton]	= fPhotTemp[i];
		fPhotonEnergy[fNPhoton]	= photon.GetT();
		fPhotonTheta[fNPhoton]	= photon.GetThetaDg();
		fPhotonPhi[fNPhoton]	= photon.GetPhiDg();
		fPhotonTime[fNPhoton]	= photon.GetTime();
		fNPhoton++;
	    }
	    else if (fIsPionPhoton[i] == kTRUE) fNPionPhoton++;		
	}

	for ( i = 0; i< fNProton; i++) {
		TA2Particle proton 	= *fProton[i];
		fProtonEnergy[i]	= proton.GetT();
		fProtonTheta[i]		= proton.GetThetaDg();
		fProtonPhi[i]		= proton.GetPhiDg();
		fProtonTime[i]		= proton.GetTime();
	}

// Calculate Missing Mass for all photons

	fNTagg		= fTAGGNParticle;

	fNTaggNPhot	= 0;
	fNPrompt	= 0;
	fNRandom	= 0;

	fPhotTimePL = -190;
	fPhotTimePR = -165;
	fPhotTimeRL1 = -240;
	fPhotTimeRR1 = -200;
	fPhotTimeRL2 = -140;
	fPhotTimeRR2 = -100;

	for (i = 0; i < fNTagg; i++) {
		fTaggerChannel[i] 	 = (fLADD->GetHits())[i];
		fTaggedPhoton[i] 	 = fTAGGParticles+i;
		TA2Particle taggerphoton = *fTaggedPhoton[i];
		fTaggerTime[i]	 	 = taggerphoton.GetTime();
	} 


	for (i = 0; i < fNPhoton; i++)  {

		for (j = 0; j < fNTagg; j++) {

//			fTaggedPhoton[j] 		= fTAGGParticles+j;
			TA2Particle taggerphoton 	= *fTaggedPhoton[j];
			fTaggerPhotonTime[fNTaggNPhot] 	= taggerphoton.GetTime() - fPhotonTime[i];
			TA2Particle photon   		= *fPhoton[i];

			TLorentzVector p4incident , p4missing;
			p4incident = fP4target[0] + taggerphoton.GetP4();
			p4missing  = p4incident   - photon.GetP4();

			if ( (fTaggerPhotonTime[fNTaggNPhot] >= fPhotTimePL && fTaggerPhotonTime[fNTaggNPhot] <= fPhotTimePR) ||
			  	(gAR->GetProcessType() == EMCProcess) ) {

				fTaggerChannelPrompt[fNPrompt]  = fTaggerChannel[j];
				fMissingMassPrompt[fNPrompt]	= p4missing.M();
				fPhotonThetaPrompt[fNPrompt]	= fPhotonTheta[i];
				fPhotonPhiPrompt[fNPrompt]	= fPhotonPhi[i];

				fNPrompt++;
			}

			if ( (fTaggerPhotonTime[fNTaggNPhot] >= fPhotTimeRL1 && fTaggerPhotonTime[fNTaggNPhot] <= fPhotTimeRR1) ||
			     (fTaggerPhotonTime[fNTaggNPhot] >= fPhotTimeRL2 && fTaggerPhotonTime[fNTaggNPhot] <= fPhotTimeRR2) ) {

				fTaggerChannelRandom[fNRandom]  = fTaggerChannel[j];
				fMissingMassRandom[fNRandom]	= p4missing.M();
				fPhotonThetaRandom[fNRandom]	= fPhotonTheta[i];
				fPhotonPhiRandom[fNRandom]	= fPhotonPhi[i];

				fNRandom++;
			}
		fNTaggNPhot++;
		}
	}

// Calculate Missing Mass for all pi0

	fNTaggNPi0	= 0;
	fNPromptPi0	= 0;
	fNRandomPi0	= 0;

	fPi0TimePL = -185;
	fPi0TimePR = -165;
	fPi0TimeRL1 = -240;
	fPi0TimeRR1 = -200;
	fPi0TimeRL2 = -140;
	fPi0TimeRR2 = -100;

	for (i = 0; i < fNPi0; i++)  {

		for (j = 0; j < fNTagg; j++) {

//			fTaggedPhoton[j] 		= fTAGGParticles+j;
			TA2Particle taggerphoton 	= *fTaggedPhoton[j];
			fTaggerPi0Time[fNTaggNPi0] 	= taggerphoton.GetTime() - fPi0Time[i];
			TA2Particle pi0   	 	= *fPi0[i];

//			std::cout << taggerphoton.GetE() << std::endl;

			TLorentzVector p4incident , p4missing, p4;
 			p4	   = pi0.GetP4();
			p4incident = fP4target[0] + taggerphoton.GetP4();
			p4missing  = p4incident   - pi0.GetP4();

			if ( (fTaggerPi0Time[fNTaggNPi0] >= fPi0TimePL && fTaggerPi0Time[fNTaggNPi0] <= fPi0TimePR) ||
			  	(gAR->GetProcessType() == EMCProcess) ) {

				fTaggerChannelPromptPi0[fNPromptPi0] 	= fTaggerChannel[j];
				fMissingMassPromptPi0[fNPromptPi0]   	= p4missing.M();
				fPi0ThetaPrompt[fNPromptPi0]		= fPi0Theta[i];
				fPi0PhiPrompt[fNPromptPi0]		= fPi0Phi[i];

				fNPromptPi0++;
			}

			if ( (fTaggerPi0Time[fNTaggNPi0] >= fPi0TimeRL1 && fTaggerPi0Time[fNTaggNPi0] <= fPi0TimeRR1) ||
			     (fTaggerPi0Time[fNTaggNPi0] >= fPi0TimeRL2 && fTaggerPi0Time[fNTaggNPi0] <= fPi0TimeRR2) ) {

				fTaggerChannelRandomPi0[fNRandomPi0] 	= fTaggerChannel[j];
				fMissingMassRandomPi0[fNRandomPi0]   	= p4missing.M();
				fPi0ThetaRandom[fNRandomPi0]		= fPi0Theta[i];
				fPi0PhiRandom[fNRandomPi0]		= fPi0Phi[i];

				fNRandomPi0++;
			}

			fNTaggNPi0++;
		}
	}


// Apply BufferEnd to the end of all arrays
	fPhotonEnergy[fNPhoton]			= EBufferEnd;
	fPhotonTheta[fNPhoton]			= EBufferEnd;
	fPhotonPhi[fNPhoton]			= EBufferEnd;
	fPhotonTime[fNPhoton]			= EBufferEnd;

	fProtonEnergy[fNProton]			= EBufferEnd;
	fProtonTheta[fNProton]			= EBufferEnd;
	fProtonPhi[fNProton]			= EBufferEnd;
	fProtonTime[fNProton]			= EBufferEnd;

	fPi0Energy[fNPi0]			= EBufferEnd;
	fPi0Theta[fNPi0]			= EBufferEnd;
	fPi0Phi[fNPi0]				= EBufferEnd;
	fPi0Time[fNPi0]				= EBufferEnd;

	f2PhotonInvariantMass[fN2PhotonInvariantMass] = EBufferEnd;

	fTaggerTime[fNTagg]			= EBufferEnd;
	fTaggerPhotonTime[fNTaggNPhot]		= EBufferEnd;
	fTaggerPi0Time[fNTaggNPi0]		= EBufferEnd;
	fTaggerChannel[fNTagg]			= EBufferEnd;

	fTaggerChannelPrompt[fNPrompt]		= EBufferEnd;
	fTaggerChannelRandom[fNRandom]		= EBufferEnd;
	fMissingMassPrompt[fNPrompt]		= EBufferEnd;
	fMissingMassRandom[fNRandom]		= EBufferEnd;
	fPhotonThetaPrompt[fNPrompt]		= EBufferEnd;
	fPhotonThetaRandom[fNRandom]		= EBufferEnd;
	fPhotonPhiPrompt[fNPrompt]		= EBufferEnd;
	fPhotonPhiRandom[fNRandom]		= EBufferEnd;

	fTaggerChannelPromptPi0[fNPromptPi0]	= EBufferEnd;
	fTaggerChannelRandomPi0[fNRandomPi0]	= EBufferEnd;
	fMissingMassPromptPi0[fNPromptPi0]	= EBufferEnd;
	fMissingMassRandomPi0[fNRandomPi0]	= EBufferEnd;
	fPi0ThetaPrompt[fNPromptPi0]		= EBufferEnd;
	fPi0ThetaRandom[fNRandomPi0]		= EBufferEnd;
	fPi0PhiPrompt[fNPromptPi0]		= EBufferEnd;
	fPi0PhiRandom[fNRandomPi0]		= EBufferEnd;

/*
// Fill Tree File
	fBasicVariable = 4; 
	fCristinaTree->Fill();
*/

}

