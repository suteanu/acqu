// SVN Info: $Id: TOA2ScalerEvent.cxx 1257 2012-07-26 15:33:13Z werthm $

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


#include "TOA2ScalerEvent.h"

ClassImp(TOA2ScalerEvent)


//______________________________________________________________________________
TOA2ScalerEvent::TOA2ScalerEvent()
    : TObject()
{
    // Constructor.
    
    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
 
    // init members
    fEvent = 0;
    fTaggInh = 0;
    fTaggFree = 0;
    fTotInh = 0;
    fTotFree = 0;
    fP2 = 0;
    fFaraday = 0;
    fTagger = 0;
}

//______________________________________________________________________________
Double_t TOA2ScalerEvent::GetTaggLive()
{
    // Return the tagger live-time.

    if (fTaggFree != 0) return fTaggInh / fTaggFree;
    else 
    {
        Warning("GetTaggLive", "Free tagger clock scaler is zero!");
        return 0;
    }
}

//______________________________________________________________________________
Double_t TOA2ScalerEvent::GetTotLive()
{
    // Return the total live-time.

    if (fTotFree != 0) return fTotInh / fTotFree;
    else 
    {
        Warning("GetTotLive", "Free total clock scaler is zero!");
        return 0;
    }
}

//______________________________________________________________________________
void TOA2ScalerEvent::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.
    
    fEvent = 0;
    fTaggInh = 0;
    fTaggFree = 0;
    fTotInh = 0;
    fTotFree = 0;
    fP2 = 0;
    fFaraday = 0;
    fTagger = 0;
}

//______________________________________________________________________________
void TOA2ScalerEvent::Print(Option_t* option) const
{
    // Print out the content of this class.

    printf("TOA2ScalerEvent content:\n");
    printf("Event number           : %d\n", fEvent);
    printf("Inhibited tagger       : %le\n", fTaggInh);
    printf("Free tagger            : %le\n", fTaggFree);
    printf("Inhibited total        : %le\n", fTotInh);
    printf("Free total             : %le\n", fTotFree);
    printf("P2                     : %le\n", fP2);
    printf("Faraday                : %le\n", fFaraday);
    printf("Total tagger           : %le\n", fTagger);
}

