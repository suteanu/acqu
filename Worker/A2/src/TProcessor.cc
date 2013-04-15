#include "TProcessor.h"

ClassImp(TProcessor)

//-----------------------------------------------------------------------------

TProcessor::TProcessor()
{

}
//-----------------------------------------------------------------------------

TProcessor::TProcessor(const Char_t* name) : TNamed(name, "Processor")
{

}

//-----------------------------------------------------------------------------

TProcessor::~TProcessor()
{

}

//-----------------------------------------------------------------------------

void TProcessor::Run()
{
  Char_t Line[1024];
  Char_t Log[256];

  Running = true;

  sprintf(Log, "Thread%XLog.txt", Number);
  sprintf(Line, "$G4WORKDIR/bin/Linux-g++/A2 %s >> %s 2>&1", Macro, Log);
  gSystem->Exec(Line);

  Running = false;
}

//-----------------------------------------------------------------------------
