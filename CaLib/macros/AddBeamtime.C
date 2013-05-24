// SVN Info: $Id: AddBeamtime.C 923 2011-05-28 17:53:17Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AddBeamtime.C                                                        //
//                                                                      //
// Add a beamtime including raw data files and initial calibrations     //
// from AcquRoot configuration files to a CaLib database.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void AddBeamtime()
{
    // load CaLib
   // gSystem->Load("lib/libCaLib.so");
 
    // macro configuration: just change here for your beamtime and leave
    // the other parts of the code unchanged
    const Char_t rawfilePath[]      = "/data/RAID0_a2datastore1_a2cb/2013_04_UnpolPi0";
    const Bool_t fileSystemMk2      = kTRUE;
    const Char_t target[]           = "LH";
    const Int_t firstRun            = 1445;
    const Int_t lastRun             = 1455;
    const Char_t calibName[]        = "UnPol_Pi0";
    const Char_t calibDesc[]        = "Standard calibration for unpolarized Pi0 May 2013 beamtime";

    const Char_t calibFileTagger[]  = "/home/peter/acqu/acqu_user/data/Tagger/FP.dat";
    const Char_t calibFileCB[]      = "/home/peter/acqu/acqu_user/data/CB/NaI.dat";
    const Char_t calibFileTAPS[]    = "/home/peter/acqu/acqu_user/data/TAPS/BaF2_PWO.dat";
    const Char_t calibFilePID[]     = "/home/peter/acqu/acqu_user/data/PID/PID.dat";
    const Char_t calibFileVeto[]    = "/home/peter/acqu/acqu_user/data/TAPS/Veto.dat";


	// Set file System
	if(fileSystemMk2)
		TCMySQLManager::GetManager()->SetMk2();
	
    // add raw files to the database
    TCMySQLManager::GetManager()->AddRunFiles(rawfilePath, target);
    
    
    // read AcquRoot calibration of tagger
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_TAGG, calibFileTagger,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // init tagging efficiency table
    TCMySQLManager::GetManager()->AddSet("Type.Tagger.Eff", calibName, calibDesc,
                                         firstRun, lastRun, 0);
      
    // read AcquRoot calibration of CB
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_CB, calibFileCB,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // init CB time walk calibration
    TCMySQLManager::GetManager()->AddSet("Type.CB.Time.Walk", calibName, calibDesc,
                                         firstRun, lastRun, 0);
     
    // init CB quadratic energy correction
    TCMySQLManager::GetManager()->AddSet("Type.CB.Energy.Quad", calibName, calibDesc,
                                         firstRun, lastRun, 0);
    
    // init CB LED calibration
    TCMySQLManager::GetManager()->AddSet("Type.CB.LED", calibName, calibDesc,
                                         firstRun, lastRun, 0);
  
    // read AcquRoot calibration of TAPS
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_TAPS, calibFileTAPS,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // init TAPS quadratic energy correction
    TCMySQLManager::GetManager()->AddSet("Type.TAPS.Energy.Quad", calibName, calibDesc,
                                         firstRun, lastRun, 0);
 
    // init TAPS LED calibration
    TCMySQLManager::GetManager()->AddSet("Type.TAPS.LED1", calibName, calibDesc,
                                         firstRun, lastRun, 0);
    TCMySQLManager::GetManager()->AddSet("Type.TAPS.LED2", calibName, calibDesc,
                                         firstRun, lastRun, 0);
 
    // read AcquRoot calibration of PID
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_PID, calibFilePID,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
    
    // read AcquRoot calibration of Veto 
    TCMySQLManager::GetManager()->AddCalibAR(kDETECTOR_VETO, calibFileVeto,
                                             calibName, calibDesc,
                                             firstRun, lastRun);
     
    gSystem->Exit(0);
}

