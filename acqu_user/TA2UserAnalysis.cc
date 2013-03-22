//--Author	JRM Annand   27th Jan 2004...Apapt A.Starostin code	
//--Rev         JRM Annand   12th May 2004...TA2Calorimeter etc. added	
//--Rev         JRM Annand   21st Nov 2004...TA2CosmicCal added	
//--Rev         JRM Annand   15th Jul 2005...TA2CrystalBall, TA2TAPS	
//--Update      JRM Annand   15th Jul 2005...Use TA2Analysis::LoadVariable
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2UserAnalysis
//
// User analysis recognises user-written apparatus and physics classes
// New apparati and physics should be entered in Map_t kKnownChild
// and also in the switch statement of CreateChild() where the apparatus
// constructers are called

#include "TA2UserAnalysis.h"
#include "TA2Tagger.h"
#include "TA2MyCrystalBall.h"
#include "TA2MyTAPS.h"
#include "TA2GenericApp.h"
#include "TA2MyAnalysis.h"
#include "TA2MyCalibration.h"
#include "TA2MyCaLib.h"
#include "TA2MyClusterCalib.h"
#include "TA2MyRateEstimation.h"


// Recognised apparatus classes.
// The "standard" set is held in TA2Analysis
enum { EA2Tagger, 
       EA2CrystalBall, 
       EA2MyCrystalBall, 
       EA2MyTAPS,
       EA2GenericApp,
       EA2Physics, 
       EA2MyAnalysis, 
       EA2MyCalibration, 
       EA2MyCaLib, 
       EA2MyClusterCalib, 
       EA2MyRateEstimation, 
};

static const Map_t kKnownChild[] = {
  {"TA2Tagger",           EA2Tagger},
  {"TA2MyCrystalBall",    EA2MyCrystalBall},
  {"TA2MyTAPS",           EA2MyTAPS},
  {"TA2GenericApp",       EA2GenericApp},
  // Physics
  {"TA2Physics",          EA2Physics},
  {"TA2MyAnalysis",       EA2MyAnalysis},
  {"TA2MyCalibration",    EA2MyCalibration},
  {"TA2MyCaLib",          EA2MyCaLib},
  {"TA2MyClusterCalib",   EA2MyClusterCalib},
  {"TA2MyRateEstimation", EA2MyRateEstimation},
  {NULL,             -1}
};

//-----------------------------------------------------------------------------
TA2UserAnalysis::TA2UserAnalysis( const char* name )
  :TA2Analysis( name )
{
  kValidChild = kKnownChild;              // valid apparatus list
}

//---------------------------------------------------------------------------
TA2UserAnalysis::~TA2UserAnalysis()
{
  // Free up allocated memory
}

//---------------------------------------------------------------------------
TA2DataManager* TA2UserAnalysis::CreateChild( const char* name, Int_t a )
{
  // Add creation of user-defined apparati here

  switch( a ){
  case EA2Tagger:
    // Standard tagger
    return new TA2Tagger( name, this );
  case EA2MyCrystalBall:
    // My Moded CB stuff
    return new TA2MyCrystalBall( name, this );
  case EA2MyTAPS:
    // My TAPS stuff
    return new TA2MyTAPS( name, this );
   case EA2GenericApp:
    // Generic
    return new TA2GenericApp( name, this );
    //
    // Physics stuff
  case EA2Physics:
    // Default (dummy physics)
    return new TA2Physics( name, this );
  case EA2MyAnalysis:
    // my analysis
    return new TA2MyAnalysis( name, this );
  case EA2MyCalibration:
    // calibration
    return new TA2MyCalibration( name, this );
  case EA2MyCaLib:
    // CaLib
    return new TA2MyCaLib( name, this );
  case EA2MyClusterCalib:
    // cluster calibration
    return new TA2MyClusterCalib( name, this );
  case EA2MyRateEstimation:
    // rate estimation
    return new TA2MyRateEstimation( name, this );
   default:
    PrintError((char*)name,
	       "<Unknown apparatus..cannot continue>", EErrFatal);
  }
  return NULL;
}

//-----------------------------------------------------------------------------
void TA2UserAnalysis::LoadVariable( )
{
  // Input name - variable pointer associations for any subsequent
  // cut or histogram setup.
  // LoadVariable( "name", pointer-to-variable, type-spec );
  // NB scaler variable pointers need the preceeding &
  //    array variable pointers do not.
  // type-spec ED prefix for a Double_t variable
  //           EI prefix for an Int_t variable
  // type-spec SingleX for a single-valued variable
  //           MultiX  for a multi-valued variable

  //                            name     pointer          type-spec
  //  TA2DataManager::LoadVariable("Mmiss",  &fMmiss,         EDSingleX);
  TA2Analysis::LoadVariable();
}

//-----------------------------------------------------------------------------
void TA2UserAnalysis::ParseDisplay( char* line )
{
  TA2Analysis::ParseDisplay( line );            // standard parse
  //  if( !fIsConfigPass ) return;              // was it a standard command?
  //  TA2HistManager::ParseDisplay( line );
}

