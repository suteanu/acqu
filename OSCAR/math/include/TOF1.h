// SVN Info: $Id: TOF1.h 1618 2012-12-21 00:23:32Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2009-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1                                                                 //
//                                                                      //
// Base class for customized and serializable TF1 based functions.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOF1
#define OSCAR_TOF1

#include "TF1.h"

#include "TOSUtils.h"


class TOF1 : public TF1
{

protected:
    TF1* fAddFunc;                      // Additional function
    Int_t fParOffset;                   // Parameter offset for the add. function
    Char_t fExpression[256];            // Function expression
    Bool_t fIsExcludeInterval;          // flag for the exclusion of an interval
    Double_t fExcludeMin;               // minimum of exclusion interval
    Double_t fExcludeMax;               // maximum of exclusion interval

    void ResetParArrays(Int_t npar);
    void UpdateExpression();

public:
    TOF1() : TF1(), fAddFunc(0), fParOffset(0), 
                    fIsExcludeInterval(kFALSE), fExcludeMin(0.), fExcludeMax(0.)
                    { fExpression[0] = '\0'; }
    TOF1(const Char_t* name, Double_t xmin, Double_t xmax, Int_t npar);
    virtual ~TOF1();
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par) = 0;
    virtual void Express(Char_t* outString) = 0;
    void PrintParameters();
    void UpdateParametersFromAddFunc();
    void UpdateParametersToAddFunc();

    Double_t operator()(Double_t* x, Double_t* par);
    void RemoveAddFunc();

    void SetAddFunc(const Char_t* formula);
    void SetAddFunc(TOF1* func);
    void SetExcludeInterval(Double_t min, Double_t max)
    {
        fIsExcludeInterval = kTRUE;
        fExcludeMin = min;
        fExcludeMax = max;
    }

    void EnableExcludeInterval() { fIsExcludeInterval = kTRUE; }
    void DisableExcludeInterval() { fIsExcludeInterval = kFALSE; }

    TF1* GetAddFunc();
    Char_t* GetExpression();
    Bool_t IsExcludeInterval() const { return fIsExcludeInterval; }

    ClassDef(TOF1, 1)  // Base class for customized functions
};

#endif

