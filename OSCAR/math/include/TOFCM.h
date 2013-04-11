// SVN Info: $Id: TOFCM.h 1257 2012-07-26 15:33:13Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOFCM                                                                //
//                                                                      //
// Fuzzy c-means algorithm for data clustering.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef OSCAR_TOFCM
#define OSCAR_TOFCM

#include "TRandom.h"
#include "TMath.h"


// define distance measures
enum EFCMDistance {
    kFCMEuclidean,
    kFCMOnSphere,
};
typedef EFCMDistance FCMDistance_t;


class TOFCM : public TObject
{

protected:
    Int_t fN;               // number of data points
    Int_t fDim;             // dimension of data
    FCMDistance_t fDist;    // distance measure
    Int_t fC;               // number of clusters
    Int_t fMaxIter;         // maximum number of iterations
    Double_t fM;            // fuzziness parameter
    Double_t fEps;          // precision for termination condition
    Double_t** fData;       // data points (not owned by this class)
    Double_t** fV;          // cluster centers
    Double_t** fU;          // membership matrix
    Int_t* fCM;             // cluster membership array
    
    void CalculateClusterMembership();

    inline Double_t CalculateDistEuclidean(Double_t* v1, Double_t* v2)
    {
        // Calculate the Euclidean distance 'v1' and 'v2'.
        
        Double_t sum = 0;

        // loop over dimensions
        for (Int_t i = 0; i < fDim; i++) sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
        
        return TMath::Sqrt(sum);
    }
    
    inline Double_t CalculateDistOnSphere(Double_t* x1, Double_t* x2)
    {
        // Calculate the distance (orthodrome) on a sphere with center
        // (0,0,0) between 'x1' and 'x2'.
        
        // check dimension
        if (fDim != 2) 
        {
            Error("CalculateDistOnSphere", "Cannot calculate distance in other than 2 dimensions!");
            return 0;
        }
        
        Double_t radius = 45.4;

        // cos(theta_1)*cos(theta_2)
        Double_t term_1 = TMath::Cos(x1[0]) * TMath::Cos(x2[0]);

        // sin(theta_1)*sin(theta_2)
        Double_t term_2 = TMath::Sin(x1[0]) * TMath::Sin(x2[0]);

        // cos(phi_2-phi_1)
        Double_t term_3 = TMath::Cos(x2[1] - x1[1]);

        return radius * TMath::ACos(term_1 + term_2 * term_3);
    }

    inline Double_t GetDistance(Double_t* x1, Double_t* x2)
    {
        // Calculate the distance between 'x1' and 'x2'.
    
        // select distance measure
        switch (fDist)
        {
            case kFCMEuclidean:
                return CalculateDistEuclidean(x1, x2);
            case kFCMOnSphere:
                return CalculateDistOnSphere(x1, x2);
            default:
                return 0;
        }
   }

public:
    TOFCM() : TObject(),
              fN(0), fDim(0), fDist(kFCMEuclidean), fC(0), fMaxIter(0), fM(0), fEps(0), 
              fData(0), fV(0), fU(0), fCM(0) { }
    TOFCM(Double_t** data, Int_t n, Int_t dim, Int_t c);
    virtual ~TOFCM();

    void SetDistanceMeasure(FCMDistance_t d) { fDist = d; }
    void SetFuzziness(Double_t m) { fM = m; }
    void SetEpsilon(Double_t eps) { fEps = eps; }
    void SetMaxIterations(Int_t maxIter) { fMaxIter = maxIter; }
    
    Int_t GetNData() const { return fN; }
    Int_t GetNCluster() const { return fC; }
    Double_t** GetCenters() const { return fV; }
    Double_t** GetMembershipMatrix() const { return fU; }
    Int_t* GetClusterMembership() const { return fCM; }
    Double_t GetMaximumMembership(Double_t* v, Int_t* outCluster = 0);

    void InitU();
    void InitV(Double_t** vinit = 0);
    void InitV(Double_t* vinit, Int_t c);
    Bool_t MinimizeU(const Char_t* opt = "");
    Bool_t MinimizeV(const Char_t* opt = "");
    void IncrementDataSet();

    Double_t CalculateObjectiveFunction();
    Double_t CalculateXieBeniIndex();
    Double_t CalculateKwonIndex();
    Double_t CalculateSVFIndex();
    Double_t CalculateMembership(Double_t* v, Int_t c);

    virtual void Print(Option_t* option = "") const;
    
    ClassDef(TOFCM, 1)  // Fuzzy c-means algorithm
};

#endif

