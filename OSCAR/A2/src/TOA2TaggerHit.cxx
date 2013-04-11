// SVN Info: $Id: TOA2TaggerHit.cxx 925 2011-05-29 03:25:14Z werthm $

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


#include "TOA2TaggerHit.h"

ClassImp(TOA2TaggerHit)


//______________________________________________________________________________
TOA2TaggerHit::TOA2TaggerHit()
    : TObject()
{
    // Default constructor.

    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
    
    // init members
    fElement = 0;
    fTime    = 0.;
    fWeight  = 1;
}

//______________________________________________________________________________
TOA2TaggerHit::TOA2TaggerHit(const TOA2TaggerHit& orig)
    : TObject(orig)
{
    // Copy constructor.

    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
    
    fElement = orig.fElement;
    fTime    = orig.fTime;
    fWeight  = orig.fWeight;
}

//______________________________________________________________________________
TOA2TaggerHit::TOA2TaggerHit(Int_t element, Double_t time, Double_t weight)
    : TObject()
{
    // Constructor that sets the members.
 
    // do not save bit mask and unique ID
    Class()->IgnoreTObjectStreamer();
    
    fElement = element;
    fTime    = time;
    fWeight  = weight;
}

