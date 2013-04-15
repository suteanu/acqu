#include "TProcessor.h"
#include "TRint.h"
#include "TThread.h"

#define CORES 16

TProcessor* Proc[CORES];
TThread*    Work[CORES];

//-----------------------------------------------------------------------------

void StartThread(Int_t Number)
{
  Proc[Number]->Run();
}

//-----------------------------------------------------------------------------

void NOP()
{
  return;
}

//-----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  Int_t Count = 0;
  Int_t Process;
  Int_t Chips = 4;
  Bool_t Busy;
  Char_t Macro[256];
  Char_t Buffer[2][256];
  Char_t Line[1024];
  Char_t Name[1024][256];
  Char_t MacroText[524288];
  Char_t* Slash;
  Char_t* Dot;
  FILE* MacroFile;
  FILE* ThreadMacro;
  FILE* ChangeCores;

  //Handle command-line options or setup file
  for(int i=1; i<argc; i++)
    if(strncmp("--", argv[i], 1))
      strcpy(Macro, argv[i]);
    else
    {
      if(!strcmp("--cores", argv[i])) sscanf(argv[i+1], "%d", &Chips);
      if(!strcmp("-c", argv[i])) sscanf(argv[i+1], "%d", &Chips);
    }

  //Print useless startup information
  printf("\n*** Worker - parallel A2 simulation processing ***\n\n");
  if(Chips > CORES)
  {
    printf("Error: Too many cores requested\n");
    return 0;
  }
  printf("Using %d cores\n", Chips);
  printf("Using A2 run macro from file %s\n", Macro);

  //For path-less config filename, append 'macros' directory
  Slash = strrchr(Macro, '/');
  if(!Slash)
  {
    sprintf(Buffer[0], "macros/%s", Macro);
    strcpy(Macro, Buffer[0]);
  }

  //Open config file and search for a) .root file names and b) input/output file commands
  MacroFile = fopen(Macro, "r");
  while(!feof(MacroFile))
  {
    if(!fgets(Line, sizeof(Line), MacroFile)) break;
    Buffer[0][0] = '\0';
    sscanf(Line, "%s %s", Buffer[0], Buffer[1]);
    //If current line contains a .root file name information...
    if(!strcmp(Buffer[0], "#TreeFile:"))
    {
      //...copy filename to processing list...
      strcpy(Name[Count], Buffer[1]);
      Count++;
    }
    else if(!strcmp(Buffer[0], "/A2/generator/InputFile"))
    {
      NOP();//...otherwise remove any input file...
    }
    else if(!strcmp(Buffer[0], "/A2/event/setOutputFile"))
    {
      NOP();//...or output file statements...
    }
    else
    {
      //...or add line to macro file template
      strcat(MacroText, Line);
    }
  }
  fclose(MacroFile);

  //Create worker threads for all cores
  for(Int_t Number=0; Number<CORES; Number++)
  {
    sprintf(Buffer[0], "Processor%d", Number);
    Proc[Number] = new TProcessor(Buffer[0]);
    Proc[Number]->SetNumber(Number);
    Proc[Number]->SetRunning(false);
    Work[Number] = new TThread("ProcessorThread", (void(*)(void*))&(StartThread), (void*)(Number), TThread::kNormalPriority);
    //Delete old log files
    sprintf(Buffer[0], "Thread%XLog.txt", Number);
    unlink(Buffer[0]);
  }

  Process = 0;
  while(Process < Count)
  {
    for(Int_t Number=0; Number<Chips; Number++)
    {
      if(!(Proc[Number]->GetRunning()))
      {
        //Lock thread
        Proc[Number]->SetRunning(true);

        //Create macro file
        sprintf(Buffer[0], "macros/Thread%XMacro.mac", Number);
        ThreadMacro = fopen(Buffer[0], "w");
        fprintf(ThreadMacro, "%s", MacroText);
        //Write input filename
        fprintf(ThreadMacro, "/A2/generator/InputFile  %s\n", Name[Process]);
        //Cut .root file extension
        Dot = strrchr(Name[Process], '.');
        *Dot = '\0';
        //Write output filename (with suffix '_sim' and re-appended .root extension)
        fprintf(ThreadMacro, "/A2/event/setOutputFile  %s_sim.root\n", Name[Process]);
        fclose(ThreadMacro);

        //Create A2 parameters
        sprintf(Buffer[0], "macros/Thread%XMacro.mac", Number);
        Proc[Number]->SetMacro(Buffer[0]);
        //Start A2 process
        Work[Number]->Run();
        //Print some process informations
        printf("Processing file %s.root on thread %2d\n", Name[Process], Number);
        //Some delay to prevent starting all processes at the same time)
        sleep(5);

        Process++;
        if(Process==Count) break;
      }
    }

    //Go to sleep before checking again for free threads
    sleep(30);

    //Check for signal file to change number of running cores
    for(Int_t c=1; c<CORES+1; c++)
    {
      sprintf(Buffer[0], "use%dcores", c);
      ChangeCores = fopen(Buffer[0], "r");
      if(ChangeCores)
      {
        printf("Changing number of cores from %d ", Chips);
        Chips = c;
        fclose(ChangeCores);
        unlink(Buffer[0]);
        printf("to %d\n", Chips);
      }
    }
  }

  //When all files have been distributed, wait for running threads to be finished
  Busy = true;
  while(Busy)
  {
    Busy = false;
    //for(Int_t Number=0; Number<Chips; Number++)
    for(Int_t Number=0; Number<CORES; Number++)
      Busy = (Busy || Proc[Number]->GetRunning());
    sleep(30);
  }

  printf("Finished\n");
  return 0;
}

//-----------------------------------------------------------------------------
