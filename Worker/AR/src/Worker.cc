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

int main(int argc, char **argv)
{
  Int_t CountRd0 = 0;
  Int_t CountDat = 0;
  Int_t Count;
  Int_t Process;
  Int_t Chips = 4;
  Bool_t Offline = false;
  Bool_t Busy;
  Char_t Config[256];
  Char_t Server[256];
  Char_t Buffer[2][256];
  Char_t Line[1024];
  Char_t NameRd0[1024][256];
  Char_t NameDat[1024][256];
  Char_t ConfigText[524288];
  Char_t ServerText[524288];
  Char_t* Slash;
  Int_t Records[1024][2];
  Int_t Number[2];
  FILE* ConfigFile;
  FILE* ServerFile;
  FILE* ThreadConfig;
  FILE* ThreadServer;
  FILE* ChangeCores;

  //Handle command-line option (dat/rd0 processing) or setup file
  for(int i=1; i<argc; i++)
    if(strncmp("--", argv[i], 1))
      strcpy(Config, argv[i]);
    else
    {
      if(!strcmp("--offline", argv[i])) Offline = true;
      if(!strcmp("-o", argv[i])) Offline = true;
      if(!strcmp("--cores", argv[i])) sscanf(argv[i+1], "%d", &Chips);
      if(!strcmp("-c", argv[i])) sscanf(argv[i+1], "%d", &Chips);
    }

  //Print useless startup information
  printf("\n*** Worker - parallel AcquRoot processing ***\n\n");
  if(Offline)
    printf("Using offline mode (processing .rd0 files)\n");
  else
    printf("Using Dataserver mode (processing .dat files)\n");
  if(Chips > CORES)
  {
    printf("Error: Too many cores requested\n");
    return 0;
  }
  printf("Using %d cores\n", Chips);
  printf("Using AcquRoot configuration from file %s\n", Config);

  //For path-less config filename, append 'data' directory
  Slash = strrchr(Config, '/');
  if(!Slash)
  {
    sprintf(Buffer[0], "data/%s", Config);
    strcpy(Config, Buffer[0]);
  }

  //Open config file and search for a) .rd0 file names and b) server configuration file name
  ConfigFile = fopen(Config, "r");
  while(!feof(ConfigFile))
  {
    if(!fgets(Line, sizeof(Line), ConfigFile)) break;
    Buffer[0][0] = '\0';
    sscanf(Line, "%s %s", Buffer[0], Buffer[1]);
    //If current line contains a .rd0 file name information...
    if(!strcmp(Buffer[0], "TreeFile:"))
    {
      //...copy filename to processing list...
      strcpy(NameRd0[CountRd0], Buffer[1]);
      CountRd0++;
    }
    else if(!strcmp(Buffer[0], "ServerSetup:"))
    {
      //..otherwise pick up server configuration file...
      strcpy(Server, Buffer[1]);
    }
    else
    {
      //...or add line to config file template
      strcat(ConfigText, Line);
    }
  }
  fclose(ConfigFile);

  //For path-less server filename, append 'data' directory
  Slash = strrchr(Server, '/');
  if(!Slash)
  {
    sprintf(Buffer[0], "data/%s", Server);
    strcpy(Server, Buffer[0]);
  }

  //Open server file and search for .dat file names
  ServerFile = fopen(Server, "r");
  while(ServerFile && !feof(ServerFile))
  {
    if(!fgets(Line, sizeof(Line), ServerFile)) break;
    Buffer[0][0] = '\0';
    sscanf(Line, "%s %s %d %d", Buffer[0], Buffer[1], &Number[0], &Number[1]);
    //If current line contains a file name information...
    if(!strcmp(Buffer[0], "File-Name:"))
    {
      //...copy filename to processing list...
      strcpy(NameDat[CountDat], Buffer[1]);
      Records[CountDat][0] = Number[0];
      Records[CountDat][1] = Number[1];
      CountDat++;
    }
    else
    {
      //...otherwise add line to server file template
      strcat(ServerText, Line);
    }
  }
  fclose(ServerFile);

  //Create worker threads for all cores
  for(Int_t Number=0; Number<CORES; Number++)
  {
    sprintf(Buffer[0], "Processor%d", Number);
    Proc[Number] = new TProcessor(Buffer[0]);
    Proc[Number]->SetNumber(Number);
    Proc[Number]->SetRunning(false);
    Work[Number] = new TThread("ProcessorThread",
                               (void(*)(void*))&(StartThread),
                               (void*)(intptr_t)(Number), TThread::kNormalPriority);
    //Delete old log files
    sprintf(Buffer[0], "Thread%XLog.txt", Number);
    unlink(Buffer[0]);
  }

  if(Offline) Count = CountRd0; else Count = CountDat;

  Process = 0;
  while(Process < Count)
  {
    for(Int_t Number=0; Number<Chips; Number++)
    {
      if(!(Proc[Number]->GetRunning()))
      {
        //Lock thread
        Proc[Number]->SetRunning(true);

        //Create config and (if required) server file
        sprintf(Buffer[0], "data/Thread%XConfig.dat", Number);
        ThreadConfig = fopen(Buffer[0], "w");
        fprintf(ThreadConfig, "%s", ConfigText);
        if(Offline)
          fprintf(ThreadConfig, "TreeFile:  %s\n#", NameRd0[Process]);
        else
        {
          sprintf(Buffer[1], "Thread%XServer.dat", Number);
          fprintf(ThreadConfig, "ServerSetup:  %s\n#", Buffer[1]);
          sprintf(Buffer[1], "data/Thread%XServer.dat", Number);
          ThreadServer = fopen(Buffer[1], "w");
          fprintf(ThreadServer, "%s", ServerText);
          fprintf(ThreadServer, "File-Name:  %s  %d  %d\n#", NameDat[Process], Records[Process][0], Records[Process][1]);
          fclose(ThreadServer);
        }
        fclose(ThreadConfig);

        //Create AcquRoot parameters and (if required) command-line flags
        if(Offline)
          sprintf(Buffer[0], "--offline Thread%XConfig.dat", Number);
        else
          sprintf(Buffer[0], "Thread%XConfig.dat", Number);
        Proc[Number]->SetConfig(Buffer[0]);
        //Start AcquRoot process
        Work[Number]->Run();
        //Print some process informations
        if(Offline)
          printf("Processing file %s (offline mode) on thread %2d\n", NameRd0[Process], Number);
        else
          printf("Processing file %s (Dataserver mode) on thread %2d\n", NameDat[Process], Number);
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
