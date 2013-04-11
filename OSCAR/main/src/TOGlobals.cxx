// SVN Info: $Id: TOGlobals.cxx 1030 2011-11-02 22:23:48Z werthm $

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


#include "TOGlobals.h"


namespace TOGlobals
{
    TDatabasePDG* fPDG_DB = 0;
    
    const Double_t kDeuteronMass  = 1875.612762;
    const Double_t kProtonMass    = 938.27203;
    const Double_t kNeutronMass   = 939.56536;
    const Double_t kLambdaMass    = 1115.683;
    const Double_t kEtaMass       = 547.853;
    const Double_t kPi0Mass       = 134.9766;
    const Double_t kPiChargedMass = 139.57018;
    const Double_t kK0Mass        = 497.614;
    const Double_t kEtaPrimeMass  = 957.78;
    const Double_t kHe3Mass       = 2808.391;
    const Double_t kDiprotonMass  = 1871.4;
    
    const Double_t kHbarGeVfm     = 0.197463569747999998;     // unit: GeV*fm/c
    
    const Int_t kDiProtonID       = 99998;
    const Int_t kDeuteronID       = 99999;

    TDatabasePDG* GetPDG_DB()
    {
        // Returns a pointer to the global PDG database object.

        // Create database if necessary
        if (!fPDG_DB) 
        {
            fPDG_DB = new TDatabasePDG;
            fPDG_DB->ReadPDGTable();
        
            // add the deuteron to the database
            fPDG_DB->AddParticle("diproton", "diproton", kDiprotonMass/1000., kTRUE,
                                 0., 6., "diproton", kDiProtonID, 0, 99);
            
            // add the deuteron to the database
            fPDG_DB->AddParticle("deuteron", "deuteron", kDeuteronMass/1000., kTRUE,
                                 0., 3., "deuteron", kDeuteronID, 0, 45);
        }

        return fPDG_DB;
    }
}

