// SVN Info: $Id: TOA2TaggerHit.h 925 2011-05-29 03:25:14Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2TaggerHit                                                        //
//                                                                      //
// This class represents a hit in the photon tagger.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2TaggerHit
#define OSCAR_TOA2TaggerHit

#include "TClass.h"
#include "TObject.h"


class TOA2TaggerHit : public TObject
{

protected:
    Int_t fElement;                         // tagger element number
    Double_t fTime;                         // hit time
    Double_t fWeight;                       // weight

public:
    TOA2TaggerHit();
    TOA2TaggerHit(const TOA2TaggerHit& orig);
    TOA2TaggerHit(Int_t element, Double_t time, Double_t weight = 1);
    virtual ~TOA2TaggerHit() { }
    
    void SetElement(Int_t n) { fElement = n; }
    void SetTime(Double_t t) { fTime = t; }
    void SetWeight(Double_t w) { fWeight = w; }

    Int_t GetElement() const { return fElement; }
    Double_t GetTime() const { return fTime; }
    Double_t GetWeight() const { return fWeight; }
    
    ClassDef(TOA2TaggerHit, 1)  // Tagger hit class for A2 experiments
};

#endif

