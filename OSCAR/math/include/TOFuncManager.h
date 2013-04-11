// SVN Info: $Id: TOFuncManager.h 1257 2012-07-26 15:33:13Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009-2012
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOFuncManager                                                        //
//                                                                      //
// Class for managing functions.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOFuncManager
#define OSCAR_TOFuncManager

#include <sstream>

#include "THashTable.h"

#include "TOASCIIReader.h"
#include "TOF1Gauss.h"
#include "TOF1GaussTail.h"
#include "TOF1Pol.h"
#include "TOF1Expo.h"
#include "TOF1Empty.h"


class TOFuncManager : public TOASCIIReader
{

protected:
    THashTable* fFunctions;                     // hash table of functions

    virtual void ParseLine(const char* inLine);

public:
    TOFuncManager() : TOASCIIReader(), fFunctions(0) { }
    TOFuncManager(const Char_t* cfgFile);
    virtual ~TOFuncManager();
 
    static TOF1* GetFunction(const Char_t* funcID, Double_t min, Double_t max, const Char_t* name = "func");
    TOF1* GetFunction(const Char_t* name);
    
    virtual void Print(Option_t* option = "") const;

    ClassDef(TOFuncManager, 1)  // Class for managing functions
};

#endif

