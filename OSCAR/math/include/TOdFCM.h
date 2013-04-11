// SVN Info: $Id: TOdFCM.h 1257 2012-07-26 15:33:13Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOdFCM                                                               //
//                                                                      //
// Dynamic fuzzy c-means algorithm for data clustering.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOdFCM
#define OSCAR_TOdFCM

#include "TOFCM.h"


class TOdFCM : public TObject
{

protected:
    Int_t fN;               // number of data points
    Int_t fDim;             // dimension of data
    FCMDistance_t fDist;    // distance measure
    Int_t fMaxIter;         // maximum number of iterations
    Double_t fM;            // fuzziness parameter
    Double_t fEps;          // precision for termination condition
    Double_t fMshipThr;     // membership threshold
    Int_t fC;               // current number of clusters
    Int_t fCMin;            // minimum number of clusters
    Int_t fCMax;            // maximum number of clusters
    Double_t** fData;       // data points (not owned by this class)
    TOFCM** fFCM;           // array of FCM minimizers

public:
    TOdFCM() : TObject(),
               fN(0), fDim(0), fDist(kFCMEuclidean),
               fMaxIter(0), fM(0), fEps(0), fMshipThr(0), 
               fC(0), fCMin(0), fCMax(0),
               fData(0), fFCM(0) { }
    TOdFCM(Double_t** data, Int_t n, Int_t dim, Int_t cmin, Int_t cmax);
    virtual ~TOdFCM();

    void SetDistanceMeasure(FCMDistance_t d) { fDist = d; }
    void SetFuzziness(Double_t m) { fM = m; }
    void SetEpsilon(Double_t eps) { fEps = eps; }
    void SetMaxIterations(Int_t maxIter) { fMaxIter = maxIter; }
    void SetMembershipThreshold(Double_t thr) { fMshipThr = thr; }
 
    TOFCM* GetResult() const { return fFCM ? fFCM[fC] : 0; }

    void Minimize(Int_t nDataStart, Int_t nC);

    virtual void Print(Option_t* option = "") const;
    
    ClassDef(TOdFCM, 1)  // Dynamic fuzzy c-means algorithm
};

#endif

