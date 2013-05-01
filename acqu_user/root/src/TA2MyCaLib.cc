// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2007-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyCaLib                                                           //
//                                                                      //
// Class used for the CaLib based calibration.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyCaLib.h"

ClassImp(TA2MyCaLib)


//______________________________________________________________________________
TA2MyCaLib::TA2MyCaLib(const char* name, TA2Analysis* analysis) 
    : TA2MyPhysics(name, analysis)
{
    // Constructor.

    // command-line recognition for SetConfig()
    AddCmdList(myCaLibConfigKeys);

    // init variables
    fNCalib    = 0;
    fNelemTAGG = 0;
    fNelemCB   = 0;
    fNelemTAPS = 0;
    fNelemPID  = 0;
    fNelemVeto = 0;

    // target position
    fCalib_Target_Position = 0;

    // CB energy calibration
    fCalib_CB_Energy = 0;    
    
    // CB quadratic energy correction
    fCalib_CB_Quad = 0;    
    fCalib_CB_Quad_Pi0_Min = 0;    
    fCalib_CB_Quad_Pi0_Max = 0;    
    fCalib_CB_Quad_Eta_Min = 0;    
    fCalib_CB_Quad_Eta_Max = 0;    
    
    // CB time calibration
    fCalib_CB_Time = 0;    
    
    // CB time walk calibration
    fCalib_CB_Walk            = 0;   
    fCalib_CB_Walk_Pi0_Min    = 0;
    fCalib_CB_Walk_Pi0_Max    = 0;
    fCalib_CB_Walk_Prompt_Min = 0;   
    fCalib_CB_Walk_Prompt_Max = 0;
    fCalib_CB_Walk_BG1_Min    = 0;
    fCalib_CB_Walk_BG1_Max    = 0;
    fCalib_CB_Walk_BG2_Min    = 0;
    fCalib_CB_Walk_BG2_Max    = 0;
    fCalib_CB_Walk_MM_Min     = 0;
    fCalib_CB_Walk_MM_Max     = 0;

    // TAPS energy calibration
    fCalib_TAPS_Energy = 0;
    
    // TAPS time calibration
    fCalib_TAPS_Time = 0;    
    
    // TAPS quadratic energy correction
    fCalib_TAPS_Quad = 0;    
    fCalib_TAPS_Quad_Pi0_Min = 0;    
    fCalib_TAPS_Quad_Pi0_Max = 0;    
    fCalib_TAPS_Quad_Eta_Min = 0;    
    fCalib_TAPS_Quad_Eta_Max = 0;    
    
    // TAPS PSA calibration
    fCalib_TAPS_PSA = 0;

    // TAPS LED calibration
    fCalib_TAPS_LED = 0;

    // PID Phi calibration
    fCalib_PID_Phi = 0;
    
    // PID energy calibration
    fCalib_PID_Energy = 0;

    // veto-crystal correlation
    fCalib_Veto_Corr = 0;
    
    // Veto energy calibration
    fCalib_Veto_Energy = 0;

    // tagger time
    fCalib_Tagger_Time = 0;
}

//______________________________________________________________________________
TA2MyCaLib::~TA2MyCaLib()
{
    // Destructor.
    
}

//______________________________________________________________________________
void TA2MyCaLib::SetConfig(Char_t* line, Int_t key)
{
    // Read in analysis configuration parameters and configure the class. 
 
    Bool_t error = kFALSE;
    
    switch (key)
    {
        case ECALIB_TARGET_POS:
            // Enable target position calibration
            if (sscanf(line, "%d", &fCalib_Target_Position) != 1) error = kTRUE;
            if (fCalib_Target_Position) fNCalib++;
            break;
        case ECALIB_CB_ENERGY:
            // Enable CB energy calibration
            if (sscanf(line, "%d", &fCalib_CB_Energy) != 1) error = kTRUE;
            if (fCalib_CB_Energy) fNCalib++;
            break;
        case ECALIB_CB_QUAD:
            // Enable CB quadratic energy correction
            if (sscanf(line, "%d%lf%lf%lf%lf", &fCalib_CB_Quad, 
                       &fCalib_CB_Quad_Pi0_Min, &fCalib_CB_Quad_Pi0_Max,
                       &fCalib_CB_Quad_Eta_Min, &fCalib_CB_Quad_Eta_Max) != 6) error = kTRUE;
            if (fCalib_CB_Quad) fNCalib++;
            break;
        case ECALIB_CB_TIME:
            // Enable CB time calibration
            if (sscanf(line, "%d", &fCalib_CB_Time) != 1) error = kTRUE;
            if (fCalib_CB_Time) fNCalib++;
            break;
        case ECALIB_CB_WALK:
            // Enable CB time walk calibration
            if (sscanf(line, "%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", 
                       &fCalib_CB_Walk,
                       &fCalib_CB_Walk_Pi0_Min,    &fCalib_CB_Walk_Pi0_Max,
                       &fCalib_CB_Walk_Prompt_Min, &fCalib_CB_Walk_Prompt_Max,
                       &fCalib_CB_Walk_BG1_Min,    &fCalib_CB_Walk_BG1_Max,
                       &fCalib_CB_Walk_BG2_Min,    &fCalib_CB_Walk_BG2_Max,
                       &fCalib_CB_Walk_MM_Min,     &fCalib_CB_Walk_MM_Max) != 11) error = kTRUE;
            if (fCalib_CB_Walk) fNCalib++;
            break;
        case ECALIB_TAPS_ENERGY:
            // Enable TAPS energy calibration
            if (sscanf(line, "%d", &fCalib_TAPS_Energy) != 1) error = kTRUE;
            if (fCalib_TAPS_Energy) fNCalib++;
            break;
        case ECALIB_TAPS_TIME:
            // Enable TAPS time calibration
            if (sscanf(line, "%d", &fCalib_TAPS_Time) != 1) error = kTRUE;
            if (fCalib_TAPS_Time) fNCalib++;
            break;
        case ECALIB_TAPS_QUAD:
            // Enable TAPS quadratic energy correction
            if (sscanf(line, "%d%lf%lf%lf%lf", &fCalib_TAPS_Quad, 
                       &fCalib_TAPS_Quad_Pi0_Min, &fCalib_TAPS_Quad_Pi0_Max,
                       &fCalib_TAPS_Quad_Eta_Min, &fCalib_TAPS_Quad_Eta_Max) != 6) error = kTRUE;
            if (fCalib_TAPS_Quad) fNCalib++;
            break;
        case ECALIB_TAPS_PSA:
            // Enable TAPS PSA calibration
            if (sscanf(line, "%d", &fCalib_TAPS_PSA) != 1) error = kTRUE;
            if (fCalib_TAPS_PSA) fNCalib++;
            break;
        case ECALIB_TAPS_LED:
            // Enable TAPS LED calibration
            if (sscanf(line, "%d", &fCalib_TAPS_LED) != 1) error = kTRUE;
            if (fCalib_TAPS_LED) fNCalib++;
            break;
        case ECALIB_PID_PHI:
            // Enable PID Phi calibration
            if (sscanf(line, "%d", &fCalib_PID_Phi) != 1) error = kTRUE;
            if (fCalib_PID_Phi) fNCalib++;
            break;
        case ECALIB_PID_ENERGY:
            // Enable PID energy calibration
            if (sscanf(line, "%d", &fCalib_PID_Energy) != 1) error = kTRUE;
            if (fCalib_PID_Energy) fNCalib++;
            break;
        case ECALIB_PID_TIME:
            // Enable PID time calibration
            if (sscanf(line, "%d", &fCalib_PID_Time) != 1) error = kTRUE;
            if (fCalib_PID_Time) fNCalib++;
            break;
        case ECALIB_VETO_CORR:
            // Enable veto-crystal correlation
            if (sscanf(line, "%d", &fCalib_Veto_Corr) != 1) error = kTRUE;
            if (fCalib_Veto_Corr) fNCalib++;
            break;
        case ECALIB_VETO_ENERGY:
            // Enable Veto energy calibration
            if (sscanf(line, "%d", &fCalib_Veto_Energy) != 1) error = kTRUE;
            if (fCalib_Veto_Energy) fNCalib++;
            break;
	case ECALIB_VETO_TIME:
            // Enable Veto time calibration
            if (sscanf(line, "%d", &fCalib_Veto_Time) != 1) error = kTRUE;
            if (fCalib_Veto_Time) fNCalib++;
            break;
        case ECALIB_TAGGER_TIME:
            // Enable tagger time calibration
            if (sscanf(line, "%d", &fCalib_Tagger_Time) != 1) error = kTRUE;
            if (fCalib_Tagger_Time) fNCalib++;
            break;
        default:
            // default parent class SetConfig()
            TA2MyPhysics::SetConfig(line, key);
            break;
    }

    if (error) PrintError(line);
}

//______________________________________________________________________________ 
void TA2MyCaLib::PostInit()
{
    // Do some further initialisation after all setup parameters have been read in.
    
    Char_t name[256];
    Char_t title[256];
    
    // call PostInit of parent class
    TA2MyPhysics::PostInit();
    
    // get number of detector elements
    fNelemTAGG = fLadder->GetNelement();
    fNelemCB   = fNaI->GetNelement();
    fNelemTAPS = fBaF2PWO->GetNelement();
    fNelemPID  = fPID->GetNelement();
    fNelemVeto = fVeto->GetNelement();
 
    // show information about the enabled calibration histogram creation
    printf("\n\n  ****** ENABLED CALIBRATION HISTOGRAMS ******\n\n");
    printf("   %d calibration(s) enabled.\n\n", fNCalib);
    if (fCalib_Target_Position) printf("   - target position\n");
    if (fCalib_CB_Energy)       printf("   - CB energy\n");
    if (fCalib_CB_Quad)         
    {
        printf("   - CB quadratic energy\n");
        printf("     -> pi0 invariant mass cut    : %8.2f to %8.2f MeV\n", fCalib_CB_Quad_Pi0_Min, fCalib_CB_Quad_Pi0_Max);
        printf("     -> eta invariant mass cut    : %8.2f to %8.2f MeV\n", fCalib_CB_Quad_Eta_Min, fCalib_CB_Quad_Eta_Max);
    }
    if (fCalib_CB_Time)     printf("   - CB time\n");
    if (fCalib_CB_Walk)         
    {
        printf("   - CB time walk\n");
        printf("     -> pi0 invariant mass cut    : %8.2f to %8.2f MeV\n", fCalib_CB_Walk_Pi0_Min, fCalib_CB_Walk_Pi0_Max);
        printf("     -> missing mass cut          : %8.2f to %8.2f MeV\n", fCalib_CB_Walk_MM_Min, fCalib_CB_Walk_MM_Max);
        printf("     -> Random subtraction prompt : %8.2f to %8.2f ns\n", fCalib_CB_Walk_Prompt_Min, fCalib_CB_Walk_Prompt_Max);
        printf("     -> Random subtraction BG 1   : %8.2f to %8.2f ns\n", fCalib_CB_Walk_BG1_Min, fCalib_CB_Walk_BG1_Max);
        printf("     -> Random subtraction BG 2   : %8.2f to %8.2f ns\n", fCalib_CB_Walk_BG2_Min, fCalib_CB_Walk_BG2_Max);
    }
    if (fCalib_TAPS_Energy) printf("   - TAPS energy\n");
    if (fCalib_TAPS_Time)   printf("   - TAPS time\n");
    if (fCalib_TAPS_Quad)         
    {
        printf("   - TAPS quadratic energy\n");
        printf("     -> pi0 invariant mass cut    : %8.2f to %8.2f MeV\n", fCalib_TAPS_Quad_Pi0_Min, fCalib_TAPS_Quad_Pi0_Max);
        printf("     -> eta invariant mass cut    : %8.2f to %8.2f MeV\n", fCalib_TAPS_Quad_Eta_Min, fCalib_TAPS_Quad_Eta_Max);
    }
    if (fCalib_TAPS_PSA)    printf("   - TAPS PSA\n");
    if (fCalib_TAPS_LED)    printf("   - TAPS LED\n");
    if (fCalib_PID_Phi)     printf("   - PID Phi\n");
    if (fCalib_PID_Energy)  printf("   - PID energy\n");
    if (fCalib_PID_Time)    printf("   - PID time\n");
    if (fCalib_Veto_Corr)   printf("   - veto-crystal correlation\n");
    if (fCalib_Veto_Energy) printf("   - Veto energy\n");
    if (fCalib_Veto_Time)   printf("   - Veto time\n");
    if (fCalib_Tagger_Time) printf("   - Tagger time\n");
    
    printf("\n");
    
    // prepare for target position calibration
    if (fCalib_Target_Position)
    {
        fHCalib_CB_IM_TargPos = new TH2F("CaLib_CB_IM_TargPos", "CaLib_CB_IM_TargPos;2#gamma invariant mass [MeV];target position [cm]",      
                                         1000, 0, 1000, 200, -10, 10);
    }
    
    // prepare for CB energy calibration
    if (fCalib_CB_Energy)
    {
        fHCalib_CB_IM      = new TH2F("CaLib_CB_IM", "CaLib_CB_IM;2#gamma invariant mass [MeV];CB element",      
                                      1000, 0, 1000, fNelemCB, 0, fNelemCB);
        fHCalib_CB_IM_Neut = new TH2F("CaLib_CB_IM_Neut", "CaLib_CB_IM_Neut;2#gamma invariant mass [MeV];CB element", 
                                      1000, 0, 1000, fNelemCB, 0, fNelemCB);
        fHCalib_CB_IM_2Neut = new TH2F("CaLib_CB_IM_2Neut", "CaLib_CB_IM_2Neut;2#gamma invariant mass [MeV];CB element", 
                                      1000, 0, 1000, fNelemCB, 0, fNelemCB);
        fHCalib_CB_IM_2Neut1Char = new TH2F("CaLib_CB_IM_2Neut1Char", "CaLib_CB_IM_2Neut1Char;2#gamma invariant mass [MeV];CB element", 
                                      1000, 0, 1000, fNelemCB, 0, fNelemCB);
    }
    
    // prepare for CB quadratic energy correction
    if (fCalib_CB_Quad)
    {   
        fHCalib_CB_Quad_IM         = new TH2F("CaLib_CB_Quad_IM", "CaLib_CB_Quad_IM;2#gamma invariant mass [MeV];CB element",
                                              1000, 0, 1000, fNelemCB, 0, fNelemCB);
        fHCalib_CB_Quad_Pi0_Mean_E = new TH2F("CaLib_CB_Quad_Pi0_Mean_E", "CaLib_CB_Quad_Pi0_Mean_E;Mean photon energy of #pi^{0} [MeV];CB element", 
                                              1000, 0, 1000, fNelemCB, 0, fNelemCB);
        fHCalib_CB_Quad_Eta_Mean_E = new TH2F("CaLib_CB_Quad_Eta_Mean_E", "CaLib_CB_Quad_Eta_Mean_E;Mean photon energy of #eta [MeV];CB element", 
                                              1000, 0, 1000, fNelemCB, 0, fNelemCB);
    }
    
    // prepare for CB time calibration
    if (fCalib_CB_Time)
    {
        fHCalib_CB_Time      = new TH2F("CaLib_CB_Time", "CaLib_CB_Time;CB cluster time [ns];CB element",
                                        2000, -100, 100, fNelemCB, 0, fNelemCB);
        fHCalib_CB_Time_Neut = new TH2F("CaLib_CB_Time_Neut", "CaLib_CB_Time_Neut;CB neutral cluster time [ns];CB element", 
                                        2000, -100, 100, fNelemCB, 0, fNelemCB);
        fHCalib_CB_Time_Ind = new TH2F("CaLib_CB_Time_Ind", "CaLib_CB_Time_Ind;CB neutral cluster time [ns];CB element", 
                                        1700, -200.1, 200.1, fNelemCB, 0, fNelemCB);
    }
    
    // prepare for CB time walk calibration
    if (fCalib_CB_Walk)
    {
        fHCalib_CB_Walk_IM           = new TH1F("CaLib_CB_Walk_IM", "CaLib_CB_Walk_IM;2#gamma invariant mass [MeV];Counts",
                                                1000, 0, 1000);
        fHCalib_CB_Walk_MM           = new TH2F("CaLib_CB_Walk_MM", "CaLib_CB_Walk_MM;Missing mass [MeV];Tagger element",
                                                2000, -1000, 1000, fNelemTAGG, 0, fNelemTAGG);
        fHCalib_CB_Walk_Rand_Time_CB = new TH1F("CaLib_CB_Walk_Rand_Time_CB", "CaLib_CB_Walk_Rand_Time_CB;CB-tagger time [ns];Counts", 
                                                2000, -1000, 1000);
        
        fHCalib_CB_Walk_E_T = new TH2*[fNelemCB];

        for (Int_t i = 0; i < fNelemCB; i++)
        {
            sprintf(name, "CaLib_CB_Walk_E_T_%03d", i);
            sprintf(title, "CaLib_CB_Walk_E_T_%03d;CB energy [MeV];CB time [ns]", i);
            fHCalib_CB_Walk_E_T[i] = new TH2F(name, title, 400, 0, 400, 250, -200, 50);
        }
    }
    
    // prepare for TAPS energy calibration
    if (fCalib_TAPS_Energy)
    {
        fHCalib_TAPS_IM          = new TH2F("CaLib_TAPS_IM", "CaLib_TAPS_IM;2#gamma invariant mass [MeV];TAPS element",
                                            1000, 0, 1000, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_IM_Neut     = new TH2F("CaLib_TAPS_IM_Neut", "CaLib_TAPS_IM_Neut;2#gamma invariant mass [MeV];TAPS element", 
                                            1000, 0, 1000, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_IM_Neut_2Cl = new TH2F("CaLib_TAPS_IM_Neut_2Cl", "CaLib_TAPS_IM_Neut_2Cl;2#gamma invariant mass [MeV];TAPS element", 
                                            1000, 0, 1000, fNelemTAPS, 0, fNelemTAPS);
    }
    
    // prepare for TAPS time calibration
    if (fCalib_TAPS_Time)
    {
        fHCalib_TAPS_Time      = new TH2F("CaLib_TAPS_Time", "CaLib_TAPS_Time;TAPS cluster time [ns];TAPS element", 
                                          2000, -100, 100, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_Time_Neut = new TH2F("CaLib_TAPS_Time_Neut", "CaLib_TAPS_Time_Neut;TAPS neutral cluster time [ns];TAPS element", 
                                          2000, -100, 100, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_Time_Ind = new TH2F("CaLib_TAPS_Time_Ind", "CaLib_TAPS_Time_Ind;TAPS neutral cluster time [ns];TAPS element", 
                                          2000, -200, 200, fNelemTAPS, 0, fNelemTAPS);
    }
    
    // prepare for TAPS quadratic energy correction
    if (fCalib_TAPS_Quad)
    {   
        fHCalib_TAPS_Quad_IM         = new TH2F("CaLib_TAPS_Quad_IM", "CaLib_TAPS_Quad_IM;2#gamma invariant mass [MeV];cluster #theta angle [deg]",
                                                1000, 0, 1000, 30, 0, 30);
        fHCalib_TAPS_Quad_Pi0_Mean_E = new TH2F("CaLib_TAPS_Quad_Pi0_Mean_E", "CaLib_TAPS_Quad_Pi0_Mean_E;Mean photon energy of #pi^{0} [MeV];cluster #theta angle [deg]", 
                                                1000, 0, 1000, 30, 0, 30);
        fHCalib_TAPS_Quad_Eta_Mean_E = new TH2F("CaLib_TAPS_Quad_Eta_Mean_E", "CaLib_TAPS_Quad_Eta_Mean_E;Mean photon energy of #eta [MeV];cluster #theta angle [deg]",
                                                1000, 0, 1000, 30, 0, 30);
    }
    
    // prepare for TAPS PSA calibration
    if (fCalib_TAPS_PSA)
    {
        fHCalib_TAPS_PSAR_PSAA = new TH2*[fNelemTAPS];

        for (Int_t i = 0; i < fNelemTAPS; i++)
        {
            sprintf(name, "CaLib_TAPS_PSAR_PSAA_%03d", i);
            sprintf(title, "CaLib_TAPS_PSAR_PSAA_%03d;PSA angle [deg];PSA radius [MeV]", i);
            fHCalib_TAPS_PSAR_PSAA[i] = new TH2F(name, title, 1800, 0, 180, 100, 0, 600);
        }
    }
    
    // prepare for TAPS LED calibration
    if (fCalib_TAPS_LED)
    {
        fHCalib_TAPS_LG       = new TH2F("CaLib_TAPS_LG", "CaLib_TAPS_LG;ADC channel;TAPS element", 
                                         2000, 0, 2000, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_LG_CFD   = new TH2F("CaLib_TAPS_LG_CFD", "CaLib_TAPS_LG_CFD;ADC channel;TAPS element",
                                         2000, 0, 2000, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_LG_NOCFD = new TH2F("CaLib_TAPS_LG_NOCFD", "CaLib_TAPS_LG_NOCFD;ADC channel;TAPS element", 
                                         2000, 0, 2000, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_LG_LED1  = new TH2F("CaLib_TAPS_LG_LED1", "CaLib_TAPS_LG_LED1;ADC channel;TAPS element",
                                         2000, 0, 2000, fNelemTAPS, 0, fNelemTAPS);
        fHCalib_TAPS_LG_LED2  = new TH2F("CaLib_TAPS_LG_LED2", "CaLib_TAPS_LG_LED2;ADC channel;TAPS element",
                                         2000, 0, 2000, fNelemTAPS, 0, fNelemTAPS);
    }

    // prepare for PID Phi calibration
    if (fCalib_PID_Phi)
    {
        fHCalib_PID_CBPhi_ID        = new TH2F("CaLib_PID_CBPhi_ID", "CaLib_PID_CBPhi_ID;CB cluster #Phi angle [deg];PID element", 
                                               720, -360, 360, fNelemPID, 0, fNelemPID);
        fHCalib_PID_CBPhi_ID_1Cryst = new TH2F("CaLib_PID_CBPhi_ID_1Cryst", "CaLib_PID_CBPhi_ID_1Cryst;CB cluster #Phi angle [deg];PID element", 
                                               720, -360, 360, fNelemPID, 0, fNelemPID);
    }
    
    // prepare for PID energy calibration
    if (fCalib_PID_Energy)
    {
        fHCalib_PID_dE_E = new TH3*[fNelemPID];
        
        for (Int_t i = 0; i < fNelemPID; i++)
        {
            if (fIsMC) 
            {
                sprintf(name, "CaLib_PID_dE_E_%03d", i);
                sprintf(title, "CaLib_PID_dE_E_%03d;CB cluster energy [MeV];PID energy [MeV];CB cluster theta [deg]", i);
                fHCalib_PID_dE_E[i] = new TH3F(name, title, 1000, 0, 1000, 200, 0,   10, 14, 20, 160);
            }
            else 
            {
                sprintf(name, "CaLib_PID_dE_E_%03d", i);
                sprintf(title, "CaLib_PID_dE_E_%03d;CB cluster energy [MeV];PID energy [Channel];CB cluster theta [deg]", i);
                fHCalib_PID_dE_E[i] = new TH3F(name, title,  500, 0, 1000, 375, 0, 1500, 14, 20, 160);
            }
        }
    }

    // prepare for PID time calibration
    if (fCalib_PID_Time)
    {
        fHCalib_PID_Time = new TH2F("CaLib_PID_Time", "CaLib_PID_Time;PID time [ns];PID element", 
				    1700, -200, 200, fNelemPID, 0, fNelemPID);
        fHCalib_PID_Time_Ind = new TH2F("CaLib_PID_Time_Ind", "CaLib_PID_Time_Ind;PID time [ns];PID element", 
                                    1700, -200.1, 200.1, fNelemPID, 0, fNelemPID);
    }
    
    // prepare for veto-crystal correlation
    if (fCalib_Veto_Corr)
    {
        fHCalib_Veto_Corr = new TH2F("CaLib_Veto_Corr", "CaLib_Veto_Corr;Veto element;TAPS element", 
                                     fNelemVeto, 0, fNelemVeto, fNelemTAPS, 0, fNelemTAPS);
    }
    
    // prepare for Veto energy calibration
    if (fCalib_Veto_Energy)
    {
        fHCalib_Veto_dE_E = new TH2*[fNelemVeto];
        
        for (Int_t i = 0; i < fNelemVeto; i++)
        {
            sprintf(name, "CaLib_Veto_dE_E_%03d", i);
            sprintf(title, "CaLib_Veto_dE_E_%03d;TAPS cluster energy [MeV];Veto energy [MeV]", i);
            fHCalib_Veto_dE_E[i] = new TH2F(name, title, 1000, 0, 1000, 400, 0, 20);
        }
    }
     
    // prepare for PID time calibration
    if (fCalib_Veto_Time)
    {
        fHCalib_Veto_Time = new TH2F("CaLib_Veto_Time", "CaLib_Veto_Time;Veto time [ns];Veto element", 
				     1000, -1000, 1000, fNelemVeto, 0, fNelemVeto);
        fHCalib_Veto_Time_Ind = new TH2F("CaLib_Veto_Time_Ind", "CaLib_Veto_Time_Ind;Veto time [ns];Veto element", 
                                     2000, -200, 200, fNelemVeto, 0, fNelemVeto);
    }

    // prepare for tagger time calibration
    if (fCalib_Tagger_Time)
    {
        fHCalib_Tagger_Time      = new TH2F("CaLib_Tagger_Time_Tot", "CaLib_Tagger_Time_Tot;Tagger-TAPS time [ns];Tagger element",
                                            1000, -100, 100, fNelemTAGG, 0, fNelemTAGG);
        fHCalib_Tagger_Time_Neut = new TH2F("CaLib_Tagger_Time_Tot_Neut", "CaLib_Tagger_Time_Tot_Neut;Tagger-TAPS time [ns];Tagger element", 
                                            1000, -100, 100, fNelemTAGG, 0, fNelemTAGG);
        fHCalib_Tagger_Time_Ind  = new TH2F("CaLib_Tagger_Time_Tot_Ind", "CaLib_Tagger_Time_Tot_Ind;Tagger time [ns];Tagger element",
                                            2200, -588.55, 706.26, fNelemTAGG, 0, fNelemTAGG);
    }
}

//______________________________________________________________________________
void TA2MyCaLib::ReconstructPhysics()
{
    // Main analysis method that is called for every event.
    
    
    // ---------------------------------- target position ---------------------------------- 
    
    if (fCalib_Target_Position)
    {
        // look for two neutral hits in CB
        if (fNNeutral == 2 && fCBNCluster == 2 && fNCharged == 0)
        {
            // get theta of particles
            Double_t theta1 = fPartCB[0]->GetTheta()*TMath::RadToDeg();

            // apply cut theta cut
            if (theta1 > 70 && theta1 < 110)
            {
                // loop over target positions
                Int_t nBins = fHCalib_CB_IM_TargPos->GetNbinsY();
                for (Int_t i = 1; i <= nBins; i++)
                {
                    // get target position
                    Double_t targetPos = fHCalib_CB_IM_TargPos->GetYaxis()->GetBinCenter(i);

                    // clone detected particles
                    TOA2DetParticle part1(*fPartCB[0]);
                    TOA2DetParticle part2(*fPartCB[1]);
                    
                    // shift coordinates
                    part1.SetZ(part1.GetZ() - targetPos);
                    part2.SetZ(part2.GetZ() - targetPos);

                    // calculate the 4-vectors assuming photons
                    TLorentzVector p4Gamma_1;
                    TLorentzVector p4Gamma_2;
                    part1.Calculate4Vector(&p4Gamma_1, 0);
                    part2.Calculate4Vector(&p4Gamma_2, 0);
                
                    // calculate the invariant mass
                    Double_t im = (p4Gamma_1 + p4Gamma_2).M();
                    
                    // fill invariant mass vs target position
                    fHCalib_CB_IM_TargPos->Fill(im, targetPos);
                }
            }
        }
    }

    
    // ------------------------------------- CB energy ------------------------------------- 
    
    if (fCalib_CB_Energy)
    {
        // loop over CB hits
        for (UInt_t i = 0; i < fCBNCluster; i++)
        {
            // calculate the 4-vector assuming a photon
            TLorentzVector p4Gamma_1;
            fPartCB[i]->Calculate4Vector(&p4Gamma_1, 0);

            // loop over CB hits
            for (UInt_t j = i+1; j < fCBNCluster; j++)
            {
                // calculate 4-vector assuming a photon
                TLorentzVector p4Gamma_2;
                fPartCB[j]->Calculate4Vector(&p4Gamma_2, 0);

                // calculate invariant mass of hit combination
                Double_t im = (p4Gamma_1 + p4Gamma_2).M();

                // fill invariant mass
                fHCalib_CB_IM->Fill(im, fPartCB[i]->GetCentralElement());
                fHCalib_CB_IM->Fill(im, fPartCB[j]->GetCentralElement());

                // fill invariant mass for neutral hits
                if (fPartCB[i]->GetPIDEnergy() == 0 && fPartCB[j]->GetPIDEnergy() == 0)
                {
                    fHCalib_CB_IM_Neut->Fill(im, fPartCB[i]->GetCentralElement());
                    fHCalib_CB_IM_Neut->Fill(im, fPartCB[j]->GetCentralElement());
                }
                
                // fill invariant mass for only 2 neutral hits
                if (fCBNCluster == 2 && fNCharged == 0 && fPartCB[i]->GetPIDEnergy() == 0 && fPartCB[j]->GetPIDEnergy() == 0)
                {
                    fHCalib_CB_IM_2Neut->Fill(im, fPartCB[i]->GetCentralElement());
                    fHCalib_CB_IM_2Neut->Fill(im, fPartCB[j]->GetCentralElement());
                }
                
                // fill invariant mass for 2 neutral hits and one charged hit
                if (fCBNCluster == 3 && fNCharged == 1 && fPartCB[i]->GetPIDEnergy() == 0 && fPartCB[j]->GetPIDEnergy() == 0)
                {
                    fHCalib_CB_IM_2Neut1Char->Fill(im, fPartCB[i]->GetCentralElement());
                    fHCalib_CB_IM_2Neut1Char->Fill(im, fPartCB[j]->GetCentralElement());
                }
            }
        }
    }


    // -------------------------- CB quadratic energy correction --------------------------- 
    
    if (fCalib_CB_Quad)
    {
        // look for two neutral hits in CB
        if (fNNeutral == 2 && fCBNCluster == 2 && fNCharged == 0)
        {
            // calculate the 4-vectors assuming photons
            TLorentzVector p4Gamma_1;
            TLorentzVector p4Gamma_2;
            fPartCB[0]->Calculate4Vector(&p4Gamma_1, 0);
            fPartCB[1]->Calculate4Vector(&p4Gamma_2, 0);
            
            // calculate the invariant mass
            Double_t im = (p4Gamma_1 + p4Gamma_2).M();
            
            // calculate the mean photon energy
            Double_t meanE = 0.5 * (p4Gamma_1.E() + p4Gamma_2.E());

            // get the central elements
            Int_t center_1 = fPartCB[0]->GetCentralElement();
            Int_t center_2 = fPartCB[1]->GetCentralElement();
            
            // fill the invariant mass
            fHCalib_CB_Quad_IM->Fill(im, center_1);
            fHCalib_CB_Quad_IM->Fill(im, center_2);

            // fill the pi0 mean energy
            if (im > fCalib_CB_Quad_Pi0_Min && im < fCalib_CB_Quad_Pi0_Max)
            {
                fHCalib_CB_Quad_Pi0_Mean_E->Fill(meanE, center_1);
                fHCalib_CB_Quad_Pi0_Mean_E->Fill(meanE, center_2);
            }
            
            // fill the eta mean energy
            if (im > fCalib_CB_Quad_Eta_Min && im < fCalib_CB_Quad_Eta_Max)
            {
                fHCalib_CB_Quad_Eta_Mean_E->Fill(meanE, center_1);
                fHCalib_CB_Quad_Eta_Mean_E->Fill(meanE, center_2);
            }
        }
    }


    // -------------------------------------- CB time ------------------------------------- 
    
    if (fCalib_CB_Time)
    {
        // loop over CB hits
        for (UInt_t i = 0; i < fCBNCluster; i++)
        {
            // get the time
            Double_t time_1 = fPartCB[i]->GetTime();

            // loop over CB hits
            for (UInt_t j = i+1; j < fCBNCluster; j++)
            {
                // get the time 
                Double_t time_2 = fPartCB[j]->GetTime();

                // fill time difference
                fHCalib_CB_Time->Fill(time_1 - time_2, fPartCB[i]->GetCentralElement());
                fHCalib_CB_Time->Fill(time_2 - time_1, fPartCB[j]->GetCentralElement());

                // fill time difference for neutral hits
                if (fPartCB[i]->GetPIDEnergy() == 0 && fPartCB[j]->GetPIDEnergy() == 0)
                {
                    fHCalib_CB_Time_Neut->Fill(time_1 - time_2, fPartCB[i]->GetCentralElement());
                    fHCalib_CB_Time_Neut->Fill(time_2 - time_1, fPartCB[j]->GetCentralElement());
                }
            }

	    if (fPartCB[i]->GetPIDEnergy() == 0) fHCalib_CB_Time_Ind->Fill(time_1, fPartCB[i]->GetCentralElement());

        }
    }


    // ----------------------------------- CB time walk ----------------------------------- 
    
    if (fCalib_CB_Walk)
    {
        // Clear the time walk parameters before analyzing
        if (fEventCounter == 0) ClearCBTimeWalk();

        // look for two neutral hits in CB
        if (fNNeutral == 2 && fCBNCluster == 2 && fNCharged == 0)
        {
            // reconstruct pi0
            TOA2RecPi02g pi0(fNNeutral);
            if (!pi0.Reconstruct(fNNeutral, fPartNeutral)) goto label_end_cb_timewalk;
            TOA2DetParticle** decay_photons = pi0.GetDetectedProducts();
        
            // get the invariant mass
            TLorentzVector* p4Pi0 = pi0.Get4Vector();
            Double_t im = p4Pi0->M();
            fHCalib_CB_Walk_IM->Fill(im);

            // invariant mass cut
            if (im < fCalib_CB_Walk_Pi0_Min || im > fCalib_CB_Walk_Pi0_Max) goto label_end_cb_timewalk;

            // photon 1 cluster properties
            Int_t g1_nhits      = decay_photons[0]->GetClusterSize();
            UInt_t* g1_hits     = decay_photons[0]->GetClusterHits();
            Double_t* g1_energy = decay_photons[0]->GetClusterHitEnergies();
            Double_t* g1_time   = decay_photons[0]->GetClusterHitTimes();
            
            // photon 1 cluster properties
            Int_t g2_nhits      = decay_photons[1]->GetClusterSize();
            UInt_t* g2_hits     = decay_photons[1]->GetClusterHits();
            Double_t* g2_energy = decay_photons[1]->GetClusterHitEnergies();
            Double_t* g2_time   = decay_photons[1]->GetClusterHitTimes();
            
            // target 4-vector
            TLorentzVector p4Target(0., 0., 0., TOGlobals::kProtonMass);
        
            // tagger loop
            for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
            {
                Int_t tagg_element = fTaggerPhotonHits[i];
                
                // skip bad tagger channels
                if (IsBadTaggerChannel(tagg_element)) continue;
                 
                // calculate background subtraction factor
                Double_t subtr_weight = GetBGSubtractionWeight(pi0, fTaggerPhotonTime[i], 
                                                               fCalib_CB_Walk_Prompt_Min, fCalib_CB_Walk_Prompt_Max, 
                                                               fCalib_CB_Walk_BG1_Min, fCalib_CB_Walk_BG1_Max,
                                                               fCalib_CB_Walk_BG2_Min, fCalib_CB_Walk_BG2_Max,
                                                               0, 0, 0, 0, 0, 0);
                
                // fill tagger coincidence time
                FillTaggerCoincidence(pi0, fTaggerPhotonTime[i], fHCalib_CB_Walk_Rand_Time_CB, 0);

                // skip useless background events (not belonging to the background windows)
                if (subtr_weight == 0) continue;

                // beam 4-vector
                TLorentzVector p4Beam(0., 0., fTaggerPhotonEnergy[i], fTaggerPhotonEnergy[i]);
                
                // missing mass
                Double_t mm = (p4Beam + p4Target - *p4Pi0).M() - TOGlobals::kProtonMass;
                fHCalib_CB_Walk_MM->Fill(mm, tagg_element, subtr_weight);
 
                // missing mass cut
                Bool_t mmOk = kFALSE;
                if (mm > fCalib_CB_Walk_MM_Min && mm < fCalib_CB_Walk_MM_Max) mmOk = kTRUE;

                if (mmOk)
                {
                    // fill energies and times of all elements of the photon cluster 1
                    for (Int_t j = 0; j < g1_nhits; j++) 
                        fHCalib_CB_Walk_E_T[g1_hits[j]]->Fill(g1_energy[j], g1_time[j] - fTaggerPhotonTime[i], subtr_weight);
                
                    // fill energies and times of all elements of the photon cluster 2
                    for (Int_t j = 0; j < g2_nhits; j++) 
                        fHCalib_CB_Walk_E_T[g2_hits[j]]->Fill(g2_energy[j], g2_time[j] - fTaggerPhotonTime[i], subtr_weight);
                }

            } // tagger loop
        
        } // if: two neutral hits in CB

    } // end CB time walk
    label_end_cb_timewalk:
    
    
    // ----------------------------------- TAPS energy ------------------------------------ 
    
    if (fCalib_TAPS_Energy)
    {
        // loop over CB hits
        for (UInt_t i = 0; i < fCBNCluster; i++)
        {
            // calculate the 4-vector assuming a photon
            TLorentzVector p4Gamma_1;
            fPartCB[i]->Calculate4Vector(&p4Gamma_1, 0);

            // loop over TAPS hits
            for (UInt_t j = 0; j < fTAPSNCluster; j++)
            {
                // calculate 4-vector assuming a photon
                TLorentzVector p4Gamma_2;
                fPartTAPS[j]->Calculate4Vector(&p4Gamma_2, 0);

                // calculate invariant mass of hit combination
                Double_t im = (p4Gamma_1 + p4Gamma_2).M();
                
                // fill invariant mass
                fHCalib_TAPS_IM->Fill(im, fPartTAPS[j]->GetCentralElement());

                // fill invariant mass for neutral hits
                if (fPartCB[i]->GetPIDEnergy() == 0 && fPartTAPS[j]->GetVetoEnergy() == 0)
                {
                    fHCalib_TAPS_IM_Neut->Fill(im, fPartTAPS[j]->GetCentralElement());

                    // fill invariant mass for exactly 1 cluster in CB and 1 cluster in TAPS
                    if (fNNeutral == 2) fHCalib_TAPS_IM_Neut_2Cl->Fill(im, fPartTAPS[j]->GetCentralElement());
                }
            }
        }
    }
    
    
    // ------------------------------------- TAPS time ------------------------------------ 
    
    if (fCalib_TAPS_Time)
    {
        // loop over TAPS hits
        for (UInt_t i = 0; i < fTAPSNCluster; i++)
        {
            // get the time
            Double_t time_1 = fPartTAPS[i]->GetTime();

            // loop over TAPS hits
            for (UInt_t j = i+1; j < fTAPSNCluster; j++)
            {
                // get the time 
                Double_t time_2 = fPartTAPS[j]->GetTime();

                // fill time difference
                fHCalib_TAPS_Time->Fill(time_1 - time_2, fPartTAPS[i]->GetCentralElement());
                fHCalib_TAPS_Time->Fill(time_2 - time_1, fPartTAPS[j]->GetCentralElement());

                // fill time difference for neutral hits
                if (fPartTAPS[i]->GetVetoEnergy() == 0 && fPartTAPS[j]->GetVetoEnergy() == 0)
                {
                    fHCalib_TAPS_Time_Neut->Fill(time_1 - time_2, fPartTAPS[i]->GetCentralElement());
                    fHCalib_TAPS_Time_Neut->Fill(time_2 - time_1, fPartTAPS[j]->GetCentralElement());
                }
            }

	    if (fPartTAPS[i]->GetVetoEnergy() == 0) fHCalib_TAPS_Time_Ind->Fill(time_1, fPartTAPS[i]->GetCentralElement());

        }
    }
    
    
    // ------------------------- TAPS quadratic energy correction ------------------------- 
    
    if (fCalib_TAPS_Quad)
    {
        // request 2 neutral clusters
        if (fNNeutral == 2)
        {
            Double_t neutInCB = kFALSE;
            Double_t neutInTAPS = kFALSE;
            TLorentzVector p4Gamma_taps;
            TLorentzVector p4Gamma_cb;

            // search neutral hit in CB and calculate the photon 4-vector
            for (UInt_t i = 0; i < fCBNCluster; i++)
            {
                // check if cluster is neutral
                if (fPartCB[i]->GetPIDEnergy() == 0) 
                {
                    fPartCB[i]->Calculate4Vector(&p4Gamma_cb, 0);
                    neutInCB = kTRUE;
                }
            }

            // search neutral hit in TAPS and calculate the photon 4-vector
            for (UInt_t i = 0; i < fTAPSNCluster; i++)
            {
                if (fPartTAPS[i]->GetVetoEnergy() == 0) 
                {
                    fPartTAPS[i]->Calculate4Vector(&p4Gamma_taps, 0);
                    neutInTAPS = kTRUE;
                }
            }

            // check if 1 cluster in CB and 1 cluster in TAPS were found
            if (neutInCB && neutInTAPS)
            {
                // calculate the invariant mass
                Double_t im = (p4Gamma_cb + p4Gamma_taps).M();
                
                // calculate the mean photon energy
                Double_t meanE = 0.5 * (p4Gamma_cb.E() + p4Gamma_taps.E());

                // get the theta of the cluster
                Int_t theta = fPartTAPS[0]->GetTheta()*TMath::RadToDeg();
             
                // fill the invariant mass
                fHCalib_TAPS_Quad_IM->Fill(im, theta);

                // fill the pi0 mean energy
                if (im > fCalib_TAPS_Quad_Pi0_Min && im < fCalib_TAPS_Quad_Pi0_Max)
                {
                    fHCalib_TAPS_Quad_Pi0_Mean_E->Fill(meanE, theta);
                }
                
                // fill the eta mean energy
                if (im > fCalib_TAPS_Quad_Eta_Min && im < fCalib_TAPS_Quad_Eta_Max)
                {
                    fHCalib_TAPS_Quad_Eta_Mean_E->Fill(meanE, theta);
                }
            }
        }
    }
    
    
    // ------------------------------- TAPS PSA calibration ------------------------------- 

    if (fCalib_TAPS_PSA)
    {
        // loop over TAPS clusters
        for (UInt_t i = 0; i < fTAPSNCluster; i++)
        {
            // get particle
            TOA2DetParticle* p = fPartTAPS[i];
            Int_t elem = p->GetCentralElement();

            // calculate the PSA information
            Double_t lgE = p->GetCentralEnergy();
            Double_t sgE = p->GetCentralSGEnergy();
            Double_t r = TMath::Sqrt(sgE*sgE + lgE*lgE); 
            Double_t a = TMath::ATan(sgE/lgE)*TMath::RadToDeg();
            
            // fill histogram
            fHCalib_TAPS_PSAR_PSAA[elem]->Fill(a, r);
        }
    }
    
    
    // ------------------------------- TAPS LED calibration ------------------------------- 

    if (fCalib_TAPS_LED)
    {
        // fill raw uncut and LED1/2 cut ADC spectra
        if (fBaF2PWO->IsRawHits())
        {
            // loop over ADC hits
            for (UInt_t i = 0; i < fBaF2PWO->GetNADChits(); i++)
            {
                Bool_t cfd_fired = kFALSE;
                
                // get element number
                Int_t elem = fBaF2PWO->GetRawEnergyHits()[i];
                
                // get ADC value
                UShort_t value_adc = fBaF2PWO->GetElement(elem)->GetRawADCValue();
                
                // calculate corresponding energy
                Double_t gain = fBaF2PWO->GetLGElement(elem)->GetA1();
                Double_t ped = fBaF2PWO->GetLGElement(elem)->GetA0();
                Double_t energy = gain * (value_adc - ped);

                // fill uncut ADC hits
                fHCalib_TAPS_LG->Fill(energy, elem);
                
                // fill hits were CFD fired and didn't fired
                for (UInt_t j = 0; j < fBaF2CFDNhits; j++)
                {
                    if (fBaF2CFDHits[j] == elem) 
                    {
                        fHCalib_TAPS_LG_CFD->Fill(energy, elem);
                        cfd_fired = kTRUE;
                    }
                }
                if (!cfd_fired) fHCalib_TAPS_LG_NOCFD->Fill(energy, elem);

                // fill hits were LED1 fired
                for (UInt_t j = 0; j < fBaF2LED1Nhits; j++)
                {
                    if (fBaF2LED1Hits[j] == elem) fHCalib_TAPS_LG_LED1->Fill(energy, elem);
                }

                // fill hits were LED2 fired
                for (UInt_t j = 0; j < fBaF2LED2Nhits; j++)
                {
                    if (fBaF2LED2Hits[j] == elem) fHCalib_TAPS_LG_LED2->Fill(energy, elem);
                }
            }
        }
    }


    // -------------------------------------- PID Phi ------------------------------------- 
    
    if (fCalib_PID_Phi)
    {
        // select 1 cluster in CB and 1 hit in the PID
        if (fCBNCluster == 1 && fPIDNhits == 1)
        {
            fHCalib_PID_CBPhi_ID->Fill(fPartCB[0]->GetPhi()*TMath::RadToDeg(), fPIDHits[0]);
            
            // 1 crystal only
            if (fPartCB[0]->GetClusterSize() == 1)
                fHCalib_PID_CBPhi_ID_1Cryst->Fill(fPartCB[0]->GetPhi()*TMath::RadToDeg(), fPIDHits[0]);
        }
    }
 

    // ------------------------------------ PID energy ------------------------------------ 
    
    if (fCalib_PID_Energy)
    {
        // Set pedestal and gain to get raw ADC values
        if (fEventCounter == 0)
        {
            // loop over PID elements
            for (Int_t i = 0; i < fNelemPID; i++)
            {
                // set pedestal to 0
                fPID->GetElement(i)->SetA0(0);

                // set gain to 1
                fPID->GetElement(i)->SetA1(1);

                // set large energy cuts
                fPID->GetElement(i)->SetEnergyLowThr(0);
                fPID->GetElement(i)->SetEnergyHighThr(5000);
            }
        }

        // loop over CB hits
        for (UInt_t i = 0; i < fCBNCluster; i++)
        {
            // get PID energy
            Double_t de = fPartCB[i]->GetPIDEnergy();
            
            // skip neutral hits
            if (de == 0) continue;
            
            // get CB theta
            Double_t th = fPartCB[i]->GetTheta()*TMath::RadToDeg();
            
            // get CB energy
            Double_t e = fPartCB[i]->GetEnergy();

            // get PID element
            Int_t elem = fPartCB[i]->GetPIDIndex();

            // fill dE vs E
            fHCalib_PID_dE_E[elem]->Fill(e, de, th);
        }
    }


    // -------------------------------------- PID time ------------------------------------- 
    
    if (fCalib_PID_Time)
    {
        // loop over PID hits
        for (UInt_t i = 0; i < fPIDNhits; i++)
        {
            // get the time
            Double_t time_1 = fPIDTime[fPIDHits[i]];

            // loop over PID hits
            for (UInt_t j = i+1; j < fPIDNhits; j++)
            {
                // get the time 
                Double_t time_2 = fPIDTime[fPIDHits[j]];

                // fill time difference
                fHCalib_PID_Time->Fill(time_1 - time_2, fPIDHits[i]);
                fHCalib_PID_Time->Fill(time_2 - time_1, fPIDHits[j]);
            }

	    fHCalib_PID_Time_Ind->Fill(time_1, fPIDHits[i]);

        }
    }


    // ----------------------------- veto-crystal correlation ----------------------------- 
    
    if (fCalib_Veto_Corr)
    {
        // loop over BaF2/PWO hits
        for (UInt_t i = 0; i < fBaF2PWONhits; i++)
        {
            // loop over veto hits
            for (UInt_t j = 0; j < fVetoNhits; j++) 
            {   
                fHCalib_Veto_Corr->Fill(fVetoHits[j], fBaF2PWOHits[i]);
            }
        }
    }
 

    // ----------------------------------- Veto energy ------------------------------------ 
    
    if (fCalib_Veto_Energy)
    {
        // loop over TAPS hits
        for (UInt_t i = 0; i < fTAPSNCluster; i++)
        {
            // get Veto energy
            Double_t de = fPartTAPS[i]->GetVetoEnergy();
            
            // skip neutral hits
            if (de == 0) continue;
            
            // get TAPS energy
            Double_t e = fPartTAPS[i]->GetEnergy();

            // get Veto element
            Int_t elem = fPartTAPS[i]->GetVetoIndex();

            // fill dE vs E
            fHCalib_Veto_dE_E[elem]->Fill(e, de);
        }
    }


    // -------------------------------------- Veto time ------------------------------------- 
    
    if (fCalib_Veto_Time)
    {
        // loop over Veto hits
        for (UInt_t i = 0; i < fVetoNhits; i++)
        {
            // get the time
            Double_t time_1 = fVetoTime[fVetoHits[i]];

            // loop over Veto hits
            for (UInt_t j = i+1; j < fVetoNhits; j++)
            {
                // get the time 
                Double_t time_2 = fVetoTime[fVetoHits[j]];

                // fill time difference
                fHCalib_Veto_Time->Fill(time_1 - time_2, fVetoHits[i]);
                fHCalib_Veto_Time->Fill(time_2 - time_1, fVetoHits[j]);
            }

	    fHCalib_Veto_Time_Ind->Fill(time_1, fVetoHits[i]);

        }
    }
 

    // ------------------------------------ tagger time ----------------------------------- 
    
    if (fCalib_Tagger_Time)
    {
        // tagger loop
        for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
        {
            // get tagger element and time
            Int_t tagg_element = fTaggerPhotonHits[i];
            Double_t tagg_time = fTaggerPhotonTime[i];
            
            // loop over TAPS hits
            for (UInt_t j = 0; j < fTAPSNCluster; j++)
            {
                // get the time 
                Double_t taps_time = fPartTAPS[j]->GetTime();

                // fill time difference
                fHCalib_Tagger_Time->Fill(tagg_time + taps_time, tagg_element);
                
                // neutral hits
                if (fPartTAPS[j]->GetVetoEnergy() == 0)
                {
                    fHCalib_Tagger_Time_Neut->Fill(tagg_time + taps_time, tagg_element);
                }
            }

	    fHCalib_Tagger_Time_Ind->Fill(tagg_time, tagg_element);

        }
    }


}

