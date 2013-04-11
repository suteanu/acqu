// SVN Info: $Id: TOGlobals.h 1030 2011-11-02 22:23:48Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOGlobals                                                            //
//                                                                      //
// This namespace grants access to global objects and methods.          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOGlobals
#define OSCAR_TOGlobals

#include "TDatabasePDG.h"


namespace TOGlobals
{
    extern TDatabasePDG* fPDG_DB;

    extern const Double_t kDeuteronMass;
    extern const Double_t kProtonMass;
    extern const Double_t kNeutronMass;
    extern const Double_t kLambdaMass;  
    extern const Double_t kEtaMass;
    extern const Double_t kPi0Mass;
    extern const Double_t kPiChargedMass;
    extern const Double_t kK0Mass;
    extern const Double_t kEtaPrimeMass;
    extern const Double_t kHe3Mass;
    extern const Double_t kDiprotonMass;
    extern const Double_t kHbarGeVfm;
    
    extern const Int_t kDeuteronID;
    
    TDatabasePDG* GetPDG_DB();
}

#endif

