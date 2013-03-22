{
  gSystem->Load("libGui.so");

  if ( !gSystem->Load("lib/libCaLib.so") )
    cout << "libCaLib.so is loaded" << endl;

}
