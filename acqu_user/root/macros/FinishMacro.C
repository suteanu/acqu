void FinishMacro()
{
  //Stuff to do at the end of an analysis run
  //Here all spectra are saved to disk
  printf("End-of-Run macro executing\n");

  gUAN->SaveAll();
  printf("All histograms saved to ARHistograms.root\n");
}
