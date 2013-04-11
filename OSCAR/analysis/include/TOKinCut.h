// SVN Info: $Id: TOKinCut.h 1257 2012-07-26 15:33:13Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOKinCut                                                             //
//                                                                      //
// Class representing a kinematical cut.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOKinCut
#define OSCAR_TOKinCut

#include "TLorentzVector.h"

#include "TOA2BaseDetParticle.h"


// define type of cut
enum EOKinCut {
    kNoCut,                 // no cut
    kEnergyCut,             // energy
    kThetaCut,              // theta angle [deg]
    kPhiCut,                // phi angle [deg]
}; 
typedef EOKinCut OKinCut_t;


class TOKinCut : public TObject
{

protected:
    OKinCut_t fType;                            // type of the cut
    Double_t fMin;                              // minimum value
    Double_t fMax;                              // maximum value

public:
    TOKinCut() : TObject(),
                 fType(kNoCut), fMin(0), fMax(0) { }
    TOKinCut(OKinCut_t type, Double_t min, Double_t max) : TObject(),
                                                           fType(type), fMin(min), fMax(max) { }
    TOKinCut(const TOKinCut& orig);
    virtual ~TOKinCut() { }
    
    void SetType(OKinCut_t t) { fType = t; }
    void SetMinimum(Double_t min) { fMin = min; }
    void SetMaximum(Double_t max) { fMax = max; }

    OKinCut_t GetType() const { return fType; }
    Double_t GetMinimum() const { return fMin; }
    Double_t GetMaximum() const { return fMax; }
    
    Bool_t IsInside(TLorentzVector* p4);
    Bool_t IsInside(TOA2BaseDetParticle* p);
    
    void PrintToString(Char_t* outString);

    ClassDef(TOKinCut, 1)  // kinematical cut class
};

#endif

