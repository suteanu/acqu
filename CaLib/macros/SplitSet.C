
/*************************************************************************
 * Author: Patrik Ott
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// SplitSet.C                                                           //
//                                                                      //
// Split manually a dataset.   				                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void SplitSet()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // macro configuration: just change here for your beamtime and leave
    // the other parts of the code unchanged
    const Int_t lastRunFirstSet     = 41960;
    const Char_t calibName[]        = "ETAP_Aug_12";
    const Int_t  setNumber	        = 0;
    const Char_t calibType[]        = "Type.Tagger.Eff";

    // add set
    TCMySQLManager::GetManager()->SplitSet(calibType, calibName, setNumber, lastRunFirstSet);
    
    gSystem->Exit(0);
}

