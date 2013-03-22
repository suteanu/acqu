// SVN Info: $Id: AddSet.C 923 2011-05-28 17:53:17Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AddSet.C                                                             //
//                                                                      //
// Add manually a new set to the database.                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void AddSet()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // macro configuration: just change here for your beamtime and leave
    // the other parts of the code unchanged
    const Int_t firstRun            = 41950;
    const Int_t lastRun             = 41960;
    const Char_t calibName[]        = "ETAP_Aug_12";
    const Char_t calibDesc[]        = "Standard calibration test for August 2012 beamtime";
    const Char_t calibType[]        = "Type.Tagger.Eff";

    // add set
    TCMySQLManager::GetManager()->AddSet(calibType, calibName, calibDesc, firstRun, lastRun, 0);
     
    gSystem->Exit(0);
}

