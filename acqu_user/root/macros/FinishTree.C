void FinishTree(Char_t* file = NULL)
{
	// Stuff to do at the end of an analysis run
	// Here all spectra are saved to disk

    	TString name;

	printf("\nEnd-of-Run macro executing:\n");

        printf("Closing tree files..."); 
 	TA2Cristina* comp = (TA2Cristina*)(gAN->GetPhysics());
 	comp->CloseTrees();
	printf(" done. \n");

	printf("Saving defined histograms...");
	if( !file) file = "ARHistograms.root";
	TFile f1(file,"RECREATE");
	gROOT->GetList()->Write();
	f1.Close();
  	printf("done.\n",file);
  	printf("All histograms saved to %s\n",file);
}
