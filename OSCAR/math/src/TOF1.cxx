// SVN Info: $Id: TOF1.cxx 1631 2013-01-24 13:58:34Z werthm $

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


#include "TOF1.h"

ClassImp(TOF1)


//______________________________________________________________________________
TOF1::TOF1(const Char_t* name, Double_t xmin, Double_t xmax, Int_t npar) 
    : TF1(name, this, xmin, xmax, npar)
{
    // Constructor.

    // init members
    fAddFunc = 0;
    fParOffset = 0;
    fIsExcludeInterval = kFALSE;
    fExcludeMin = 0;
    fExcludeMax = 0;
}
 
//______________________________________________________________________________
TOF1::~TOF1()
{
    // Destructor.

    //if (fAddFunc) delete fAddFunc;    // seems to double-free somehow
}

//______________________________________________________________________________
Double_t TOF1::operator()(Double_t* x, Double_t* par)
{
    // Implementation of the () operator needed by TF1.
    
    // exclusion interval
    if (fIsExcludeInterval)
    {
        if (x[0] > fExcludeMin && x[0] < fExcludeMax) TF1::RejectPoint();
    }

    Double_t out = 0.;

    // add the return value from the subclass's evaluation method
    out += Evaluate(x, par);

    // add the return value from the additional function if it exists
    // notice the shift in the parameter array
    if (fAddFunc) 
    {
        if (fAddFunc->InheritsFrom("TOF1"))
        {
            TOF1* h = (TOF1*) fAddFunc;
            out += h->Evaluate(x, par + fParOffset);
            if (TF1* addFunc2 = h->GetAddFunc())
            {
                Int_t add_offset = h->GetNpar() - addFunc2->GetNpar();
                out += addFunc2->EvalPar(x, par + fParOffset + add_offset);
            }
        }
        else
        {
            out += fAddFunc->EvalPar(x, par + fParOffset);
        }
    }

    return out;
}

//______________________________________________________________________________
void TOF1::Streamer(TBuffer& R__b)
{
    // Stream a TOF1 object. 
    // This is not used to save/load specific members of this class but to
    // just set again the functor member of the TF1 to this class's () operator 
    // (to be more exact: to the () operator implementation of the subclass as 
    // this is an abstract class).
    
    UInt_t R__s, R__c; 
    
    if (R__b.IsReading())
    { 
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c); 
        if (R__v) { } 
        
        // call streamer of parent class
        TF1::Streamer(R__b); 
        
        // copy data members
        R__b >> fAddFunc;
        R__b >> fParOffset;
        R__b >> fExpression;
        R__b >> fIsExcludeInterval;
        R__b >> fExcludeMin;
        R__b >> fExcludeMax;

        // check byte count
        R__b.CheckByteCount(R__s, R__c, TOF1::IsA()); 
       
        // set the functor to the () operator
        fFunctor = ROOT::Math::ParamFunctor(this);
    } 
    else 
    { 
        // update the expression
        UpdateExpression();

        R__c = R__b.WriteVersion(TOF1::IsA(), kTRUE); 
        
        // call streamer of parent class
        TF1::Streamer(R__b); 
        
        // copy data members
        R__b << fAddFunc;
        R__b << fParOffset;
        R__b << fExpression;
        R__b << fIsExcludeInterval;
        R__b << fExcludeMin;
        R__b << fExcludeMax;

        // set byte count
        R__b.SetByteCount(R__c, kTRUE); 
    } 
}

//______________________________________________________________________________
void TOF1::ResetParArrays(Int_t npar)
{
    // Reset the parameter arrays for 'npar' parameters.

    Double_t params[fNpar];
    Char_t names[fNpar][256];
    Double_t par_errors[fNpar];
    Double_t par_min[fNpar];
    Double_t par_max[fNpar];

    // save old values
    Int_t npar_old = fNpar;
    for (Int_t i = 0; i < npar_old; i++)
    {
        params[i]     = fParams[i];
        strcpy(names[i], fNames[i].Data());
        par_errors[i] = fParErrors[i];
        par_min[i]    = fParMin[i];
        par_max[i]    = fParMax[i];
    }

    // delete old arrays
    if (npar_old)
    {
        delete [] fParams;
        delete [] fNames;
        delete [] fParErrors;
        delete [] fParMin;
        delete [] fParMax;
    }

    // set new number of parameters
    fNpar = npar;
    
    // calculate the parameter offset
    if (fAddFunc) fParOffset = fNpar - fAddFunc->GetNpar();
    else fParOffset = 0;

    // create new arrays
    fParams = new Double_t[fNpar];
    fNames = new TString[fNpar];
    fParErrors = new Double_t[fNpar];
    fParMin = new Double_t[fNpar];
    fParMax = new Double_t[fNpar];

    // init the arrays
    for (Int_t i = 0; i < fNpar; i++) 
    {
        // copy the old values from this function
        if (i < npar_old)
        {
            fParams[i]    = params[i];
            fNames[i]     = names[i];
            fParErrors[i] = par_errors[i];
            fParMin[i]    = par_min[i];
            fParMax[i]    = par_max[i];
        }
        // or the added function 
        else
        {
            fParams[i]    = fAddFunc->GetParameter(i-fParOffset);
            fNames[i]     = fAddFunc->GetParName(i-fParOffset);
            fParErrors[i] = fAddFunc->GetParError(i-fParOffset);
            fAddFunc->GetParLimits(i-fParOffset, fParMin[i], fParMax[i]);
        }
    }
}

//______________________________________________________________________________
void TOF1::SetAddFunc(const Char_t* formula)
{
    // Add an additional interpreted function to this function.

    // check formula string
    if (strlen(formula))
    {
        // remove old function
        RemoveAddFunc();

        // build the function
        Char_t tmp[256];
        sprintf(tmp, "%s_addFunc", GetName());
        fAddFunc = new TF1(tmp, formula, fXmin, fXmax);
        
        // adjust the parameter arrays
        ResetParArrays(fNpar + fAddFunc->GetNpar());

        // update the expression
        UpdateExpression();
    }
}

//______________________________________________________________________________
void TOF1::SetAddFunc(TOF1* func)
{
    // Add an additional function to this function.

    // check function
    if (func)
    {
        // remove old function
        RemoveAddFunc();

        // build the function
        fAddFunc = func;
        
        // adjust the parameter arrays
        ResetParArrays(fNpar + fAddFunc->GetNpar());

        // update the expression
        UpdateExpression();
    }
}

//______________________________________________________________________________
void TOF1::RemoveAddFunc()
{
    // Remove the additional interpreted function from this function.

    // check if the additional function exists
    if (fAddFunc) 
    {
        // get the number of additional parameters
        Int_t npar_add = fAddFunc->GetNpar();

        // delete the function
        delete fAddFunc;
        fAddFunc = 0;
        
        // adjust the parameter arrays
        ResetParArrays(fNpar - npar_add);

        // update the expression
        UpdateExpression();
    }
}

//______________________________________________________________________________
TF1* TOF1::GetAddFunc()
{
    // Returns the additional function using the current parameter values
    // set in this class.

    // check for additional function
    if (fAddFunc)
    {
        // update the parameters
        UpdateParametersToAddFunc();

        return fAddFunc;
    }
    else return 0;
}

//______________________________________________________________________________
Char_t* TOF1::GetExpression()
{
    // Return the expression.

    UpdateExpression();
    return fExpression;
}

//______________________________________________________________________________
void TOF1::PrintParameters()
{
    // Print out the values of all parameters.

    // loop over parameters
    printf(" PAR   NAME               VALUE          ERROR          MIN            MAX\n");
    for (Int_t i = 0; i < fNpar; i++)
    {
        printf(" %3d   %-15s   %12.5e   %12.5e   %12.5e   %12.5e\n", 
               i, fNames[i].Data(), fParams[i], fParErrors[i], fParMin[i], fParMax[i]);
    }
}

//______________________________________________________________________________
void TOF1::UpdateExpression()
{
    // Update the expression.
    
    Char_t tmp[256];

    // get the main function expression
    Express(tmp);

    // set the total function expression
    if (fAddFunc) 
    {
        if (fAddFunc->InheritsFrom("TOF1"))
        {
            Char_t tmp2[256];
            TOF1* h = (TOF1*) fAddFunc;
            h->Express(tmp2);
            if (h->GetAddFunc())
            {   
                Char_t tmp3[256];
                strcpy(tmp3, h->GetAddFunc()->GetExpFormula().Data());
                sprintf(fExpression, "%s+[%s+[%s]]", tmp, tmp2, tmp3);
            }   
            else sprintf(fExpression, "%s+[%s]", tmp, tmp2);
        }
        else
        {
            sprintf(fExpression, "%s+[%s]", tmp, fAddFunc->GetExpFormula().Data());
        }
    }
    else strcpy(fExpression, tmp);
}
    
//______________________________________________________________________________
void TOF1::UpdateParametersFromAddFunc()
{
    // Update the parameters (values, errors, minima, maxima) that belong to the 
    // additional function from the proper parameters of the additional function.

    // abort if there is no additional function
    if (!fAddFunc) return;

    // loop over the parameters of the additional function
    for (Int_t i = 0; i < fAddFunc->GetNpar(); i++)
    {
        fParams[fParOffset+i] = fAddFunc->GetParameter(i);
        fParErrors[fParOffset+i] = fAddFunc->GetParError(i);
        Double_t min, max;
        fAddFunc->GetParLimits(i, min, max);
        fParMin[fParOffset+i] = min;
        fParMax[fParOffset+i] = max;
    }
}

//______________________________________________________________________________
void TOF1::UpdateParametersToAddFunc()
{
    // Update the proper parameters (values, errors, minima, maxima) of the
    // additional function from the parameters of the main function.

    // abort if there is no additional function
    if (!fAddFunc) return;

    // loop over the parameters of the additional function
    for (Int_t i = 0; i < fAddFunc->GetNpar(); i++)
    {
        fAddFunc->SetParameter(i, fParams[fParOffset+i]);
        fAddFunc->SetParError(i, fParErrors[fParOffset+i]);
        fAddFunc->SetParLimits(i, fParMin[fParOffset+i], fParMax[fParOffset+i]);
    }
}

