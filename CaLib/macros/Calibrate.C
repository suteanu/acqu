// SVN Info: $Id: Calibrate.C 764 2011-01-25 21:16:52Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Calibrate.C                                                          //
//                                                                      //
// Non-GUI calibrations using CaLib.                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void Calibrate()
{
    // load CaLib
    gSystem->Load("libCaLib.so");
 
    // get the calibration module
    TCCalibVetoEnergy c;
    c.Start(Domi_Calib, 0);
    c.ProcessAll();
}

