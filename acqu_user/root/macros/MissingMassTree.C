#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TH2.h"

void MissingMass2008() {

	Char_t* filename = Form("/work0/cristina/TA2Pi0Compton2008.root");
	TFile *file1 = new TFile(filename);
	TTree *tree1 = (TTree*)file1->Get("Pi0ComptonTree");

	gStyle->SetOptStat(1111);

	Int_t NPrompt, NRandom;
	Int_t NPromptPi0, NRandomPi0;

	Int_t TaggerChannelPrompt[1000], TaggerChannelRandom[1000];
	Int_t TaggerChannelPromptPi0[1000], TaggerChannelRandomPi0[1000];

	Double_t MissingMassPrompt[1000], MissingMassRandom[1000];
	Double_t MissingMassPromptPi0[1000], MissingMassRandomPi0[1000];

	tree1->SetBranchAddress("NPrompt",		&NPrompt);
	tree1->SetBranchAddress("NRandom",		&NRandom);

	tree1->SetBranchAddress("NPromptPi0",		&NPromptPi0);
	tree1->SetBranchAddress("NRandomPi0",		&NRandomPi0);

	tree1->SetBranchAddress("TaggerChannelPrompt",	&TaggerChannelPrompt);
	tree1->SetBranchAddress("TaggerChannelRandom",	&TaggerChannelRandom);

	tree1->SetBranchAddress("TaggerChannelPromptPi0",&TaggerChannelPromptPi0);
	tree1->SetBranchAddress("TaggerChannelRandomPi0",&TaggerChannelRandomPi0);

	tree1->SetBranchAddress("MissingMassPrompt",	&MissingMassPrompt);
	tree1->SetBranchAddress("MissingMassRandom",	&MissingMassRandom);

	tree1->SetBranchAddress("MissingMassPromptPi0",	&MissingMassPromptPi0);
	tree1->SetBranchAddress("MissingMassRandomPi0",	&MissingMassRandomPi0);

	TH1D *a0 = new TH1D("Missing-Mass-Prompt", "Missing-Mass-Prompt", 400,800,1200);
	TH1D *a1 = new TH1D("Missing-Mass-Random", "Missing-Mass-Random", 400,800,1200);
	TH1D *a2 = new TH1D("Missing-Mass-Compton","Missing-Mass-Compton",400,800,1200);

	TH1D *b0 = new TH1D("Missing-Mass-Pi0-Prompt","Missing-Mass-Pi0-Prompt", 400,800,1200);
	TH1D *b1 = new TH1D("Missing-Mass-Pi0-Random","Missing-Mass-Pi0-Random", 400,800,1200);
	TH1D *b2 = new TH1D("Missing-Mass-Pi0",	      "Missing-Mass-Pi0",	 400,800,1200);

	Int_t nentries = (Int_t)tree1->GetEntries();
	for (Int_t i=0;i<nentries;i++) {

		tree1->GetEntry(i);
		
		for (Int_t a = 0; a < NPrompt; a++){
			if (TaggerChannelPrompt[a] >= 240 && TaggerChannelPrompt[a] <= 260){
				a0->Fill(MissingMassPrompt[a]);
			}
		}	

		for (Int_t a = 0; a < NRandom; a++){
			if (TaggerChannelRandom[a] >= 240 && TaggerChannelRandom[a] <= 260){
				a1->Fill(MissingMassRandom[a]);
			}
		}	

		for (Int_t a = 0; a < NPromptPi0; a++){
			if (TaggerChannelPromptPi0[a] >= 240 && TaggerChannelPromptPi0[a] <= 260){
				b0->Fill(MissingMassPromptPi0[a]);
			}
		}	

		for (Int_t a = 0; a < NRandomPi0; a++){
			if (TaggerChannelRandomPi0[a] >= 240 && TaggerChannelRandomPi0[a] <= 260){
				b1->Fill(MissingMassRandomPi0[a]);
			}	
		}
	}
	
	canvas1 = new TCanvas("canvas1","Missing-Mass-Compton");
	a2->Add(a0,1);
//	a2->Add(a1,-0.3125); //Data2012
	a2->Add(a1,-0.1071); //Data2008
	a2->Draw();

	canvas2 = new TCanvas("canvas2","Missing-Mass-Pi0");
	b2->Add(b0,1);
//	b2->Add(b1,-0.25); // Data2012
	b2->Add(b1,-0.0714); // Data2008
	b2->Draw();

}


void MissingMass2012() {

	Char_t* filename = Form("/work0/cristina/TA2Pi0Compton2012-373.root");
	TFile *file1 = new TFile(filename);
	TTree *tree1 = (TTree*)file1->Get("Pi0ComptonTree");

	gStyle->SetOptStat(1111);

	Int_t NPrompt, NRandom;
	Int_t NPromptPi0, NRandomPi0;

	Int_t TaggerChannelPrompt[1000], TaggerChannelRandom[1000];
	Int_t TaggerChannelPromptPi0[1000], TaggerChannelRandomPi0[1000];

	Double_t MissingMassPrompt[1000], MissingMassRandom[1000];
	Double_t MissingMassPromptPi0[1000], MissingMassRandomPi0[1000];

	tree1->SetBranchAddress("NPrompt",		&NPrompt);
	tree1->SetBranchAddress("NRandom",		&NRandom);

	tree1->SetBranchAddress("NPromptPi0",		&NPromptPi0);
	tree1->SetBranchAddress("NRandomPi0",		&NRandomPi0);

	tree1->SetBranchAddress("TaggerChannelPrompt",	&TaggerChannelPrompt);
	tree1->SetBranchAddress("TaggerChannelRandom",	&TaggerChannelRandom);

	tree1->SetBranchAddress("TaggerChannelPromptPi0",&TaggerChannelPromptPi0);
	tree1->SetBranchAddress("TaggerChannelRandomPi0",&TaggerChannelRandomPi0);

	tree1->SetBranchAddress("MissingMassPrompt",	&MissingMassPrompt);
	tree1->SetBranchAddress("MissingMassRandom",	&MissingMassRandom);

	tree1->SetBranchAddress("MissingMassPromptPi0",	&MissingMassPromptPi0);
	tree1->SetBranchAddress("MissingMassRandomPi0",	&MissingMassRandomPi0);

	TH1D *a0 = new TH1D("Missing-Mass-Prompt", "Missing-Mass-Prompt", 400,800,1200);
	TH1D *a1 = new TH1D("Missing-Mass-Random", "Missing-Mass-Random", 400,800,1200);
	TH1D *a2 = new TH1D("Missing-Mass-Compton","Missing-Mass-Compton",400,800,1200);

	TH1D *b0 = new TH1D("Missing-Mass-Pi0-Prompt","Missing-Mass-Pi0-Prompt", 400,800,1200);
	TH1D *b1 = new TH1D("Missing-Mass-Pi0-Random","Missing-Mass-Pi0-Random", 400,800,1200);
	TH1D *b2 = new TH1D("Missing-Mass-Pi0",	      "Missing-Mass-Pi0",	 400,800,1200);

	Int_t nentries = (Int_t)tree1->GetEntries();
	for (Int_t i=0;i<nentries;i++) {

		tree1->GetEntry(i);
		
		for (Int_t a = 0; a < NPrompt; a++){
			if (TaggerChannelPrompt[a] >= 240 && TaggerChannelPrompt[a] <= 260){
				a0->Fill(MissingMassPrompt[a]);
			}
		}	

		for (Int_t a = 0; a < NRandom; a++){
			if (TaggerChannelRandom[a] >= 240 && TaggerChannelRandom[a] <= 260){
				a1->Fill(MissingMassRandom[a]);
			}
		}	

		for (Int_t a = 0; a < NPromptPi0; a++){
			if (TaggerChannelPromptPi0[a] >= 240 && TaggerChannelPromptPi0[a] <= 260){
				b0->Fill(MissingMassPromptPi0[a]);
			}
		}	

		for (Int_t a = 0; a < NRandomPi0; a++){
			if (TaggerChannelRandomPi0[a] >= 240 && TaggerChannelRandomPi0[a] <= 260){
				b1->Fill(MissingMassRandomPi0[a]);
			}	
		}
	}
	
	canvas1 = new TCanvas("canvas1","Missing-Mass-Compton");
	a2->Add(a0,1);
	a2->Add(a1,-0.025); //Data2012
	a2->Draw();

	canvas2 = new TCanvas("canvas2","Missing-Mass-Pi0");
	b2->Add(b0,1);
	b2->Add(b1,-0.025); // Data2012
	b2->Draw();

}

void MissingMassComparePi0() {

	Char_t* filename = Form("/work0/cristina/TA2Pi0Compton2008.root");
	TFile *file1 = new TFile(filename);
	TTree *tree1 = (TTree*)file1->Get("Pi0ComptonTree");

	gStyle->SetOptStat(1111);

	Int_t NPromptPi0, NRandomPi0;

	Int_t TaggerChannelPromptPi0[1000], TaggerChannelRandomPi0[1000];

	Double_t MissingMassPromptPi0[1000], MissingMassRandomPi0[1000];

	tree1->SetBranchAddress("NPromptPi0",		&NPromptPi0);
	tree1->SetBranchAddress("NRandomPi0",		&NRandomPi0);

	tree1->SetBranchAddress("TaggerChannelPromptPi0",&TaggerChannelPromptPi0);
	tree1->SetBranchAddress("TaggerChannelRandomPi0",&TaggerChannelRandomPi0);

	tree1->SetBranchAddress("MissingMassPromptPi0",	&MissingMassPromptPi0);
	tree1->SetBranchAddress("MissingMassRandomPi0",	&MissingMassRandomPi0);

	TH1D *a0 = new TH1D("Missing-Mass-Pi0-Prompt-2008","Missing-Mass-Pi0-Prompt-2008", 400,800,1200);
	TH1D *a1 = new TH1D("Missing-Mass-Pi0-Random-2008","Missing-Mass-Pi0-Random-2008", 400,800,1200);
	TH1D *a2 = new TH1D("Missing-Mass-Pi0-2008",	   "Missing-Mass-Pi0-2008",	   400,800,1200);

	TH1D *b0 = new TH1D("Missing-Mass-Pi0-Prompt-2012","Missing-Mass-Pi0-Prompt-2012", 400,800,1200);
	TH1D *b1 = new TH1D("Missing-Mass-Pi0-Random-2012","Missing-Mass-Pi0-Random-2012", 400,800,1200);
	TH1D *b2 = new TH1D("Missing-Mass-Pi0-2012",	      "Missing-Mass-Pi0-2012",	 400,800,1200);

	Int_t nentries = (Int_t)tree1->GetEntries();
	for (Int_t i=0;i<nentries;i++) {

		tree1->GetEntry(i);
		
		for (Int_t a = 0; a < NPromptPi0; a++){
			if (TaggerChannelPromptPi0[a] >= 240 && TaggerChannelPromptPi0[a] <= 260){
				a0->Fill(MissingMassPromptPi0[a]);
			}
		}	

		for (Int_t a = 0; a < NRandomPi0; a++){
			if (TaggerChannelRandomPi0[a] >= 240 && TaggerChannelRandomPi0[a] <= 260){
				a1->Fill(MissingMassRandomPi0[a]);
			}	
		}
	}
	
	Char_t* filename = Form("/work0/cristina/TA2Pi0Compton2012-373.root");
	TFile *file2 = new TFile(filename);
	TTree *tree2 = (TTree*)file2->Get("Pi0ComptonTree");

	tree2->SetBranchAddress("NPromptPi0",		&NPromptPi0);
	tree2->SetBranchAddress("NRandomPi0",		&NRandomPi0);

	tree2->SetBranchAddress("TaggerChannelPromptPi0",&TaggerChannelPromptPi0);
	tree2->SetBranchAddress("TaggerChannelRandomPi0",&TaggerChannelRandomPi0);

	tree2->SetBranchAddress("MissingMassPromptPi0",	&MissingMassPromptPi0);
	tree2->SetBranchAddress("MissingMassRandomPi0",	&MissingMassRandomPi0);

	Int_t nentries = (Int_t)tree2->GetEntries();
	for (Int_t i=0;i<nentries;i++) {

		tree2->GetEntry(i);
		
		for (Int_t a = 0; a < NPromptPi0; a++){
			if (TaggerChannelPromptPi0[a] >= 240 && TaggerChannelPromptPi0[a] <= 260){
				b0->Fill(MissingMassPromptPi0[a]);
			}
		}	

		for (Int_t a = 0; a < NRandomPi0; a++){
			if (TaggerChannelRandomPi0[a] >= 240 && TaggerChannelRandomPi0[a] <= 260){
				b1->Fill(MissingMassRandomPi0[a]);
			}	
		}
	}

	canvas1 = new TCanvas("canvas1","Missing-Mass-Compton");
	a2->Add(a0,1);
	a2->Add(a1,-0.025); //Data2012
	a2->Draw();

	canvas2 = new TCanvas("canvas2","Missing-Mass-Pi0");
	b2->Add(b0,1);
	b2->Add(b1,-0.025); // Data2012
	b2->Draw();

}


