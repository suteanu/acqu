//--Author	JRM Annand     8th Feb 2007   Adapt from Mk1 format
//--Rev 	JRM Annand
//--Rev 	JRM Annand ...27th Apr 2007   Standard header-info interface
//--Rev 	JRM Annand ... 9th Jun 2007   Upgrade module & error struct
//--Rev 	JRM Annand ...21st Apr 2008   UpdateInfo(),ConstructHeader()
//--Rev 	JRM Annand ...31st Aug 2012   Mk2 module info fNScChannel
//--Update	JRM Annand ...21st Nov 2012   Stuff for merging data
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2Mk2Format
// Specify Mk2 (new) ACQU formatted data stream
//
//---------------------------------------------------------------------------

#ifndef _TA2Mk2Format_h_
#define _TA2Mk2Format_h_

#include "TA2DataFormat.h"

// Array sizes
enum { EMk2SizeTime = 32, EMk2SizeComment = 256, EMk2SizeFName = 128,
       EMk2SizeDesc = 256 };

struct AcquMk2Info_t {	       	      // 1st part of header buffer
  UInt_t fMk2;                        // Mark as Mk2 data
  Char_t fTime[EMk2SizeTime];	      // run start time (ascii)
  Char_t fDescription[EMk2SizeDesc];  // description of experiment
  Char_t fRunNote[EMk2SizeComment];   // particular run note
  Char_t fOutFile[EMk2SizeFName];     // output file
  Int_t fRun;		              // run number
  Int_t fNModule;		      // total no. modules
  Int_t fNADCModule;	       	      // no. ADC modules
  Int_t fNScalerModule;	              // no. scaler modules
  Int_t fNADC;		              // no. ADC's read out
  Int_t fNScaler;	    	      // no. scalers readout
  Int_t fRecLen;		      // maximum buffer length = record len
};

struct ModuleInfoMk2_t {
  Int_t fModID;                 // Acqu ID # of module
  Int_t fModIndex;		// Module list index
  Int_t fModType;		// type of module, ADC, latch etc.
  Int_t fAmin;	       	        // 1st channel index
  Int_t fNChannel;	       	// # channels
  Int_t fNScChannel;            // # scaler channels (if ADC and Scaler)
  Int_t fBits;	       	        // significant bits from output word
};

struct ReadErrorMk2_t {
  UInt_t fHeader;	       	// error block header
  Int_t fModID;                 // hardware identifier
  Int_t fModIndex;	        // list index of module 
  Int_t fErrCode;	       	// error code returned
  UInt_t fTrailer;              // end of error block marker
};

//struct ScalerBlockMk2_t {
//  UInt_t fID;
//  UInt_t fCounts;
//}

class TA2Mk2Format : public TA2DataFormat {
 protected:
  AcquMk2Info_t* fExpInfo;       // experimental info from header
  ModuleInfoMk2_t* fModInfo;     // experimental hardware modules
 public:
  TA2Mk2Format( Char_t*, Int_t, Int_t=0 );
  virtual ~TA2Mk2Format(){ }
  virtual void ConstructHeader(void* = NULL);
  virtual void MergeHeader(void*, void*, Int_t, Int_t);
  virtual void CheckHeader();
  virtual void SwapHeader(void*);	       // byte-swap header
  virtual void UpdateHeader(void*, void*);     // start of new file
  virtual void UpdateInfo();                   // update module info  
  virtual Int_t GetEventLength( void* );       // length of an event
  //
  virtual Bool_t IsHeadBuff( void* b ){         // Header buffer ?
    if( *(UInt_t*)b == EHeadBuff ) return ETrue;
    else return EFalse;
  }
  virtual Bool_t IsDataBuff( void* b ){         // Data buffer ?
    if( *(UInt_t*)b == EDataBuff ) return ETrue;
    else return EFalse;
  }
  virtual Bool_t IsEndBuff( void* b ){          // Trailer buffer ?
    if( *(UInt_t*)b == EEndBuff ) return ETrue;
    else return EFalse;
  }
  virtual Bool_t IsTermBuff( void* b ){         // Terminator buffer ?
    if( *(UInt_t*)b == EKillBuff ) return ETrue;
    else return EFalse;
  }
  virtual UInt_t GetBufferType( void* b ){ return *(UInt_t*)b; }
  // Retrieve header info
  virtual Int_t GetNScaler(){ return fExpInfo->fNScaler; }
  virtual Int_t GetNADC(){ return fExpInfo->fNADC; }
  virtual Int_t GetNModule(){ return fExpInfo->fNModule; }
  virtual Int_t GetModID(Int_t i){ return (fModInfo[i]).fModID; }
  virtual Int_t GetModIndex(Int_t i){ return (fModInfo[i]).fModIndex; }
  virtual Int_t GetModType(Int_t i){ return (fModInfo[i]).fModType; }
  virtual Int_t GetModAmin(Int_t i){ return (fModInfo[i]).fAmin; }
  virtual Int_t GetModNChan(Int_t i){ return (fModInfo[i]).fNChannel; }
  virtual Int_t GetModBits(Int_t i){ return (fModInfo[i]).fBits; }
  virtual Int_t GetErrorSize( ){ return sizeof(ReadErrorMk2_t); }
  virtual UInt_t GetDataBuffHead(){ return EMk2DataBuff; }
  //  
  ClassDef(TA2Mk2Format,1)
};

//-----------------------------------------------------------------------------
inline Int_t TA2Mk2Format::GetEventLength( void* evbuff )
{
  // Identify event's - worth of data and return event length (# of bytes )
  // Mk2 format....if 0th data source account for event number in data
  //
  Int_t length = 0;
  UInt_t* event = (UInt_t*)evbuff;

  if( !fNsrc ){
    fEventNumber = *event;
    event++; length++;     // step past event #
  }
  else fEventNumber = *(event-1);
  if( fEventNumber == EEndEvent ){
    return 0;
  }

  // If final event read of scalers only
  // For now frig synch by discarding this event
  if( *event == EScalerBuffer ){
    return 0;
  }

  if( fEventIndex == *(UShort_t*)event )
    fEventID = *(UShort_t*)( (UShort_t*)event + 1 );
  else fEventID = 0xffff;

  while( *event != EEndEvent ){
    length++;    event++;
  }
  return (length * sizeof(UInt_t));
}

#endif
