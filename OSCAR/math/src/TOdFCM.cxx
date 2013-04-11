// SVN Info: $Id: TOdFCM.cxx 1257 2012-07-26 15:33:13Z werthm $

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


#include "TOdFCM.h"

ClassImp(TOdFCM)


//______________________________________________________________________________
TOdFCM::TOdFCM(Double_t** data, Int_t n, Int_t dim, Int_t cmin, Int_t cmax)
    : TObject()
{
    // Constructor.
    //
    // Parameters:
    // data :   data array
    // n    :   number of data points
    // dim  :   dimension of data
    // cmin :   minimum number of clusters
    // cmax :   maximum number of clusters

    // init members
    fN = n;
    fDim = dim;
    fDist = kFCMEuclidean;
    fMaxIter = 300;
    fM = 2;
    fEps = 1e-4;
    fMshipThr = 0.8;
    fC = 0;
    if (cmin < 2)
    {
        Warning("TOdFCM", "Minimum number of clusters has to be larger than 1! Setting it to 2.");
        cmin = 2;
    }
    fCMin = cmin;
    fCMax = cmax;
    fData = data;
    
    // create arrays
    fFCM = new TOFCM*[fCMax+1];
    for (Int_t i = 0; i < fCMax+1; i++) fFCM[i] = 0;
}
 
//______________________________________________________________________________
TOdFCM::~TOdFCM()
{
    // Destructor.
    
    if (fFCM)
    {
        for (Int_t i = 0; i < fCMax+1; i++)
            if (fFCM[i]) delete fFCM[i];
        delete [] fFCM;
    }
}

//______________________________________________________________________________
void TOdFCM::Minimize(Int_t nDataStart, Int_t nC)
{
    // Find clusters according to the dFCM algorithm.
    // 
    // Parameters:
    // nDataStart   :   initial number of data points
    // nC           :   initial number of clusters
    
    // first FCM
    fC = nC;
    fFCM[fC] = new TOFCM(fData, nDataStart, fDim, fC);
    fFCM[fC]->SetDistanceMeasure(fDist);
    fFCM[fC]->SetFuzziness(fM);
    fFCM[fC]->SetEpsilon(fEps);
    fFCM[fC]->SetMaxIterations(fMaxIter);
    fFCM[fC]->InitV();
    fFCM[fC]->MinimizeV("q");
    
    // add data points one by one
    for (Int_t n = nDataStart; n < fN; n++)
    {
        // calculate maximum membership of the new data point
        Double_t maxMembership = fFCM[fC]->GetMaximumMembership(fData[n]);
        
        // check if maximum membership of data point is larger than threshold
        if (maxMembership > fMshipThr)
        {
            // add data point
            fFCM[fC]->IncrementDataSet();
            
            // AO update
            fFCM[fC]->MinimizeV("q");
        }
        else
        {
            // search bounds for the number of clusters
            // maximum range is fC-2 ... fC+2
            Int_t start = fC-2 < fCMin ? fCMin : fC-2;
            Int_t end   = fC+2 > fCMax ? fCMax : fC+2;
            
            // iterate over cluster scenarios
            for (Int_t c = start; c <= end; c++)
            {
                // check if fC clusters were generated before
                if (fFCM[c])
                {
                    // increment the size of the data set until 
                    // current number of data points reached
                    while (fFCM[c]->GetNData() < n+1) fFCM[c]->IncrementDataSet();

                    // AO update
                    fFCM[c]->MinimizeV("q");
                }
                else
                {
                    // create new FCM for c clusters
                    fFCM[c] = new TOFCM(fData, n+1, fDim, c);
                    fFCM[c]->SetDistanceMeasure(fDist);
                    fFCM[c]->SetFuzziness(fM);
                    fFCM[c]->SetEpsilon(fEps);
                    fFCM[c]->SetMaxIterations(fMaxIter);
 
                    // initialize with c-1 cluster centers or random centers
                    if (fFCM[c-1])
                    {
                        for (Int_t i = 0; i < c-1; i++) 
                            fFCM[c]->InitV(fFCM[c-1]->GetCenters()[i], i);
                    }
                    else fFCM[c]->InitV();

                    // set the c-th cluster center close to the new data point
                    Double_t cPerturb[fDim];
                    for (Int_t d = 0; d < fDim; d++) 
                        cPerturb[d] = fData[n][d] * gRandom->Uniform(0.99, 1.01);
                    fFCM[c]->InitV(cPerturb, c-1);

                    // AO update
                    fFCM[c]->MinimizeV("q");
                }
            }

            // get the cluster number with the best validity index
            Double_t minIndex = 1e30;
            for (Int_t c = start; c <= end; c++)
            {
                Double_t index = fFCM[c]->CalculateKwonIndex();
                if (index < minIndex) 
                {
                    fC = c;
                    minIndex = index;
                }
            }
        }
    }
}

//______________________________________________________________________________
void TOdFCM::Print(Option_t* option) const
{
    // Print out the content of this class.

    printf("TOdFCM content:\n");
    printf("Data points                 : %d\n", fN);
    printf("Dimensions                  : %d\n", fDim);
    printf("Distance measure            : ");
    switch (fDist)
    {
        case kFCMEuclidean:
            printf("Euclidean\n");
            break;
        case kFCMOnSphere:
            printf("On sphere\n");
            break;
    }
    printf("Maximum iterations          : %d\n", fMaxIter);
    printf("Fuzziness                   : %e\n", fM);
    printf("Epsilon                     : %e\n", fEps);
    printf("Membership threshold        : %e\n", fMshipThr);
    printf("Current number of clusters  : %d\n", fC);
    printf("Minimum number of clusters  : %d\n", fCMin);
    printf("Maximum number of clusters  : %d\n", fCMax);
}

