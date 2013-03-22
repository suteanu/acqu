// SVN Info: $Id: AddTable.C 923 2011-05-28 17:53:17Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AddTable.C                                                           //
//                                                                      //
// This macro can be used to add a data table to an old-format data-    //
// base.                                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void AddTable()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // create a new data table for a datatype and a number of parameters
    TCMySQLManager::GetManager()->CreateDataTable("Data.Tagger.Eff", 352);
     
    gSystem->Exit(0);
}

