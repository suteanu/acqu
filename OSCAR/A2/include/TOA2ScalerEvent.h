// SVN Info: $Id: TOA2ScalerEvent.h 1257 2012-07-26 15:33:13Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOA2ScalerEvent                                                      //
//                                                                      //
// Monitor scaler events and rates.                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOA2ScalerEvent
#define OSCAR_TOA2ScalerEvent

#include "TObject.h"
#include "TClass.h"


class TOA2ScalerEvent : public TObject
{

private:
    Int_t fEvent;                       // event number
    Double_t fTaggInh;                  // inhibited tagger clock scaler
    Double_t fTaggFree;                 // free tagger clock scaler
    Double_t fTotInh;                   // inhibited total clock scaler
    Double_t fTotFree;                  // free total clock scaler
    Double_t fP2;                       // P2 scaler
    Double_t fFaraday;                  // Faraday cup scaler
    Double_t fTagger;                   // total tagger scaler

public:
    TOA2ScalerEvent();
    virtual ~TOA2ScalerEvent() { }
    
    Int_t GetEvent() const { return fEvent; }
    Double_t GetTaggInh() const { return fTaggInh; }
    Double_t GetTaggFree() const { return fTaggFree; }
    Double_t GetTotInh() const { return fTotInh; }
    Double_t GetTotFree() const { return fTotFree; }
    Double_t GetP2() const { return fP2; }
    Double_t GetFaraday() const { return fFaraday; }
    Double_t GetTagger() const { return fTagger; }

    void SetEvent(Int_t event) { fEvent = event; }
    void SetTaggInh(Double_t value) { fTaggInh = value; }
    void SetTaggFree(Double_t value) { fTaggFree = value; }
    void SetTotInh(Double_t value) { fTotInh = value; }
    void SetTotFree(Double_t value) { fTotFree = value; }
    void SetP2(Double_t value) { fP2 = value; }
    void SetFaraday(Double_t value) { fFaraday = value; }
    void SetTagger(Double_t value) { fTagger = value; }

    Double_t GetTaggLive();
    Double_t GetTotLive();
    
    void Clear(Option_t* option = "");

    void Print(Option_t* option = "") const;

    ClassDef(TOA2ScalerEvent, 1)  // Scaler event class for A2 experiments
};

#endif

