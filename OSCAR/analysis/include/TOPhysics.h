// SVN Info: $Id: TOPhysics.h 1633 2013-01-24 14:00:17Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, Manuel Dieterle, 2009
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOPhysics                                                            //
//                                                                      //
// This namespace contains some often used physics calculations.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOPhysics
#define OSCAR_TOPhysics

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include "TLorentzVector.h"
#include "TRandom3.h"

#include "TOFermiDistribution.h"


namespace TOPhysics
{
    void Calculate4Vector(Double_t x, Double_t y, Double_t z, Double_t energy, Double_t mass,
                          TLorentzVector* outP4);

    Double_t CalcQFThreeBodyRecoilPartT(Double_t beamE, TLorentzVector* p4Meson, TLorentzVector* p4Part, 
                                        Double_t mTarg, Double_t mPart, Double_t mSpec);
    
    Double_t CalcPhotoprodPSFactorEg(Double_t eG, Double_t tMass, Double_t mMass);
    Double_t CalcPhotoprodPSFactorW(Double_t W, Double_t tMass, Double_t mMass);
    Double_t CalcWSpecPart(Double_t eG, Double_t fermiP, Double_t fermiPz, 
                           Double_t specMass, Double_t nuclMass,
                           Double_t deG = 0, Double_t* outdW = 0);
}

#endif

