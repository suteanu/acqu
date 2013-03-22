
/*************************************************************************
 * Author: Patrik Ott
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// RemoveSet.C                                                          //
//                                                                      //
// Remove manually a dataset.   			                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void RemoveSet()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // macro configuration: just change here for your beamtime and leave
    // the other parts of the code unchanged
    const Char_t calibName[]        = "ETAP_Aug_12";
    const Int_t  setNumber	        = 1;
    const Char_t calibType[]        = "Type.Tagger.Eff";

    // add set
    TCMySQLManager::GetManager()->RemoveSet(calibType, calibName, setNumber);
    
    gSystem->Exit(0);
}

