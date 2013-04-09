//--Author	JRM Annand    9th Sep 2004
//--Rev
//--Update
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// ROOT dictionary specification
// All classes linked in libUserRoot.so must included in the ROOT
// dictionary so that they are recognised by the CINT command processor

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Global pointers to core TAcquRoot and TA2Analysis classes
#pragma link C++ global gAR;
#pragma link C++ global gAN;

// Any new user written class must be included in this list
// Main control and analysis
#pragma link C++ class TA2UserControl+;
#pragma link C++ class TA2UserAnalysis+;
// Apparatus classes
#pragma link C++ class TA2GenericApp+;
// Detector classes
#pragma link C++ class TA2PlasticVETO+;
#pragma link C++ class TA2PlasticPID+;
#pragma link C++ class TA2CalArray+;
#pragma link C++ class TA2FPMicro+;
// TA2My... extension classes
#pragma link C++ class TA2MyAnalysis+;
#pragma link C++ class TA2MyCalibration+;
#pragma link C++ class TA2MyCaLib+;
#pragma link C++ class TA2MyClusterCalib+;
#pragma link C++ class TA2MyRateEstimation+;
#pragma link C++ class TA2MyPhysics+;
#pragma link C++ class TA2MyCrystalBall+;
#pragma link C++ class TA2MyTAPS+;
#pragma link C++ class TA2MyTAPS_BaF2PWO+;
#endif

