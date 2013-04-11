// SVN Info: $Id: TOFuncManager.cxx 1362 2012-09-25 13:58:36Z werthm $

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


#include "TOFuncManager.h"

ClassImp(TOFuncManager)


//______________________________________________________________________________
TOFuncManager::TOFuncManager(const Char_t* cfgFile)
    : TOASCIIReader(cfgFile)
{
    // Constructor using the function configuration file 'cfgFile'.

    // init members
    fFunctions = new THashTable();
    fFunctions->SetOwner(kTRUE);

    // parse configuration file
    ReadFile();
    
    // user info
    Info("TOFuncManager", "Added and configured %d function(s)", fFunctions->GetSize());
}

//______________________________________________________________________________
TOFuncManager::~TOFuncManager()
{
    // Destructor.

    if (fFunctions) delete fFunctions;
}

//______________________________________________________________________________
void TOFuncManager::ParseLine(const char* inLine)
{
    // Parse the function configuration file 'filename'.
    
    // skip empty and commented lines
    Char_t* l = TOSUtils::Trim(inLine);
    if (l[0] == '\0' || TOSUtils::IsComment(inLine)) return;
    
    // some variables
    string line(inLine);
    istringstream iss(line);
    string id;
    string fid;
    Double_t min;
    Double_t max;
    Int_t nPar;
    
    // read the configuration from the line
    iss >> id >> fid >> min >> max >> nPar;
    
    // create function
    TOF1* f = GetFunction(fid.c_str(), min, max, id.c_str());
    
    // check function
    if (f)
    {
        // read the function parameters from the line
        string par;
        for (Int_t i = 0; i < nPar; i++) 
        {
            // read parameter statement
            iss >> par;

            // check for limits and set parameter configuration
            if (TOSUtils::IndexOf(par.c_str(), "[") != -1)
            {
                Double_t p, pMin, pMax;
                sscanf(par.c_str(), "%lf[%lf,%lf]", &p, &pMin, &pMax);
                f->SetParameter(i, p);
                f->SetParLimits(i, pMin, pMax);
            }
            else if (TOSUtils::IndexOf(par.c_str(), "=") != -1)
            {
                Double_t p;
                sscanf(par.c_str(), "=%lf", &p);
                f->FixParameter(i, p);
            }
            else
            {
                Double_t p;
                sscanf(par.c_str(), "%lf", &p);
                f->SetParameter(i, p);
            }
        }
        
        // add function to table
        fFunctions->Add(f);
    }
    else
    {
        Error("ParseLine", "Function '%s' not defined!", fid.c_str());
    }
}

//______________________________________________________________________________
TOF1* TOFuncManager::GetFunction(const Char_t* funcID, Double_t min, Double_t max, const Char_t* name)
{
    // Return the function 'funcID' with the minimum 'min', the maximum 'max' 
    // and the name 'name'.

    Char_t tmp[256];

    // return the function
    if (!strcmp(funcID, "empty_func"))
    {
        TOF1Empty* f = new TOF1Empty(name, min, max);
        return f;
    }
    else if (!strcmp(funcID, "gaus"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        return f;
    }
    else if (!strcmp(funcID, "gaus_pol0"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 0);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "gaus_pol1"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 1);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "gaus_pol2"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 2);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "gaus_pol3"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 3);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "gaus_gaus"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Gauss* fadd = new TOF1Gauss(tmp, min, max);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "gaustail"))
    {
        TOF1GaussTail* f = new TOF1GaussTail(name, min, max);
        return f;
    }
    else if (!strcmp(funcID, "gaustail_pol1"))
    {
        TOF1GaussTail* f = new TOF1GaussTail(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 1);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "gaustail_pol2"))
    {
        TOF1GaussTail* f = new TOF1GaussTail(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 2);
        f->SetAddFunc(fadd);
        return f;
    }
    else if (!strcmp(funcID, "pol0"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 0);
        return f;
    }
    else if (!strcmp(funcID, "pol1"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 1);
        return f;
    }
    else if (!strcmp(funcID, "pol2"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 2);
        return f;
    }
    else if (!strcmp(funcID, "pol3"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 3);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta2g_fulinc"))
    {
        TOF1GaussTail* f = new TOF1GaussTail(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 3);
        fadd->SetExcludeInterval(420, 610);
        f->SetAddFunc(fadd);
        f->SetParLimits(0, 1, 5000);
        f->SetParLimits(1, 535, 565);
        f->SetParLimits(2, 10, 30);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta2g_qfinc"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 2);
        fadd->SetExcludeInterval(420, 630);
        f->SetAddFunc(fadd);
        f->SetParLimits(0, 0, 1e5);
        f->SetParLimits(1, 520, 555);
        f->SetParLimits(2, 10, 30);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta2g_bg_pol1"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 1);
        f->SetExcludeInterval(420, 630);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta2g_bg_pol2"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 2);
        f->SetExcludeInterval(420, 630);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta2g_bg_pol3"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 3);
        f->SetExcludeInterval(420, 630);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta6g_fulinc"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 3);
        fadd->SetExcludeInterval(510, 610);
        f->SetAddFunc(fadd);
        f->SetParLimits(0, 1, 5000);
        f->SetParLimits(1, 535, 565);
        f->SetParLimits(2, 10, 30);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta6g_free"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 2);
        fadd->SetExcludeInterval(480, 620);
        f->SetAddFunc(fadd);
        f->SetParLimits(0, 1, 1e5);
        f->SetParLimits(1, 535, 565);
        f->SetParLimits(2, 10, 30);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta6g_bg_pol1"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 1);
        f->SetExcludeInterval(480, 620);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta6g_bg_pol2"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 2);
        f->SetExcludeInterval(480, 620);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta6g_bg_pol3"))
    {
        TOF1Pol* f = new TOF1Pol(name, min, max, 3);
        f->SetExcludeInterval(480, 620);
        return f;
    }
    else if (!strcmp(funcID, "IM_gauss"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        f->SetParLimits(0, 0, 2000);
        f->SetParLimits(1, 520, 555);
        f->SetParLimits(2, 10, 30);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta2g_fulexc_p"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        f->SetAddFunc("landau");
        f->SetParLimits(0, 1, 2000.);
        f->SetParLimits(1, 520., 555.);
        f->SetParLimits(2, 10., 30.);
        f->SetParLimits(3, 1, 2000.);
        f->SetParLimits(4, 400., 500.);
        f->SetParLimits(5, 20., 200);
        return f;
    }
    else if (!strcmp(funcID, "IM_k0_lambda"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        f->SetAddFunc("landau");
        f->SetParLimits(0, 1, 2000.);
        f->SetParLimits(1, 470., 520.);
        f->SetParLimits(2, 10., 20.);
        f->SetParLimits(3, 1, 2000.);
        f->SetParLimits(4, 400., 500.);
        f->SetParLimits(5, 20., 200);
        return f;
    }
    else if (!strcmp(funcID, "IM_eta6g_qfinc"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        sprintf(tmp, "%s_addfunc", name);
        TOF1Pol* fadd = new TOF1Pol(tmp, min, max, 3);
        fadd->SetExcludeInterval(500, 590);
        f->SetAddFunc(fadd);
        f->SetParLimits(0, 0, 1e5);
        f->SetParLimits(1, 520, 555);
        f->SetParLimits(2, 10, 20);
        return f;
    }
    else if (!strcmp(funcID, "MM_eta2g_bg"))
    {
        TOF1Gauss* f = new TOF1Gauss(name, min, max);
        f->SetParLimits(1, 100, 400);
        return f;
    }
    else 
    {
        printf("Warning in <TOFuncManager::GetFunction>: Function '%s' is not defined!\n", funcID);
        return 0;
    }
}

//______________________________________________________________________________
TOF1* TOFuncManager::GetFunction(const Char_t* name)
{
    // Return the function with the identifier 'name'. Return 0 if no such function
    // could be found.

    TObject* o = fFunctions->FindObject(name);
    return o ? (TOF1*) o : 0;
}

//______________________________________________________________________________
void TOFuncManager::Print(Option_t* option) const
{
    // Print information about this class.
    
    printf("TOFuncManager content:\n");
    printf("number of functions : %d\n", fFunctions->GetSize());
    TIter next(fFunctions);
    TOF1* f;
    while ((f = (TOF1*)next())) f->Print();
}

