//--Author	JRM Annand   27th Apr 2003
//--Rev 	JRM Annand...30th Sep 2003  New Detector bases
//--Rev 	JRM Annand...15th Oct 2003  ReadDecoded...MC data
//--Rev 	JRM Annand... 5th Feb 2004  3v8 compatible
//--Rev 	JRM Annand... 7th Jun 2005  ReadDecoded...use fEnergyScale
//--Rev 	JRM Annand...25th Oct 2005  ReadDecoded...energy thresh
//--Rev 	D.Glazier ...24th Aug 2007  ReadDecoded...include detector time
//--Update	JRM Annand ..17th Nov 2007  ReadDecoded total energy fix
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2CalArray
//
// Decoding and calibration methods for the Crystall Ball array of NaI(Tl)
// Configured as forward wall to work in conjunction with the CB
// This can use standard TA2ClusterDetector methods or ones redifined here
//

#include "TA2CalArray.h"

ClassImp(TA2CalArray)

//---------------------------------------------------------------------------
TA2CalArray::TA2CalArray( const char* name, TA2System* apparatus )
  :TA2ClusterDetector(name, apparatus)
{
  // Do not allocate any "new" memory here...Root will wipe
  // Set private variables to zero/false/undefined state
  // Pass kLaddKeys (command-control keywords) and
  // kLaddHist (names of histograms) to progenitor classes
  
  fRandom = new TRandom3(123);
}

//---------------------------------------------------------------------------
TA2CalArray::~TA2CalArray()
{
  // Free up all allocated memory
  // ...arrays created at the initialisation stage
  // Start with common arrays of TA2Detector class
  DeleteClusterArrays();
}

//-----------------------------------------------------------------------------
void TA2CalArray::ParseDisplay( char* line )
{
  // Input private histogram spec to general purpose parse
  // and setup routine

  //  const Name2Variable_t hist[] = {
  // Name          ->variable  single/mult    Fill-condition
  //    {"Nphoton_Minv", fM_Nphoton, EHistSingleX},
  //    {NULL,          0,         0}
  //  };
  // Do not remove the final NULL line

  TA2ClusterDetector::ParseDisplay(line);
  return;
}

//---------------------------------------------------------------------------
void TA2CalArray::PostInit()
{
  // Some further initialisation after all setup parameters read in
  // Start with alignment offsets
  // Create space for various output arrays
  TA2ClusterDetector::PostInit();
}

//---------------------------------------------------------------------------
void TA2CalArray::SaveDecoded( )
{
  // Save decoded info to Root Tree file
}
