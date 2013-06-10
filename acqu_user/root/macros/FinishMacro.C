void FinishMacro()
{
  printf( "End-of-Run macro executing\n");

  TFile f( "histograms/ARHistograms.root","recreate");
  gROOT->GetList()->Write();
  f.Close();

  printf("All histograms saved to ARHistograms.root\n\n");

//  gSystem->Exit(0);
}
