// SVN Info: $Id: TOFCM.cxx 1257 2012-07-26 15:33:13Z werthm $

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


#include "TOFCM.h"

ClassImp(TOFCM)


//______________________________________________________________________________
TOFCM::TOFCM(Double_t** data, Int_t n, Int_t dim, Int_t c)
    : TObject()
{
    // Constructor.
    //
    // Parameters:
    // data :   data array
    // n    :   number of data points
    // dim  :   dimension of data
    // c    :   number of clusters

    // init members
    fN = n;
    fDim = dim;
    fDist = kFCMEuclidean;
    fC = c;
    fMaxIter = 300;
    fM = 2;
    fEps = 1e-4;
    fData = data;
    fV = new Double_t*[fC];
    fU = new Double_t*[fC];
    fCM = new Int_t[fN];
    for (Int_t i = 0; i < fC; i++) 
    {
        fV[i] = new Double_t[fDim];
        fU[i] = new Double_t[fN];
    }
    
    // zero arrays
    for (Int_t i = 0; i < fN; i++)
    {   
        // init cluster membership
        fCM[i] = -1;
        
        // membership matrix
        for (Int_t j = 0; j < fC; j++) fU[j][i] = 0.;
    }

    // cluster centers
    for (Int_t j = 0; j < fC; j++) 
        for (Int_t d = 0; d < fDim; d++) fV[j][d] = 0.;
}
 
//______________________________________________________________________________
TOFCM::~TOFCM()
{
    // Destructor.
    
    // if there is fV, there is also fU
    if (fV)
    {
        for (Int_t i = 0; i < fC; i++)
        {
            delete [] fV[i];
            delete [] fU[i];
        }
        delete [] fV;
        delete [] fU;
    }
    if (fCM) delete [] fCM;
}

//______________________________________________________________________________
Double_t TOFCM::CalculateObjectiveFunction()
{
    // Return the value of the objective function.

    Double_t value = 0;

    // loop over data points
    for (Int_t k = 0; k < fN; k++)
    {
        // loop over cluster centers
        for (Int_t i = 0; i < fC; i++)
        {
            Double_t dist = GetDistance(fData[k], fV[i]);
            value += TMath::Power(fU[i][k], fM) * dist * dist;
        }
    }

    return value;
}

//______________________________________________________________________________
Double_t TOFCM::CalculateXieBeniIndex()
{
    // Return the Xie-Beni cluster validity index.

    Double_t num = 0;

    //
    // calculate numerator
    //

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        // loop over data points
        for (Int_t k = 0; k < fN; k++)
        {
            Double_t dist = GetDistance(fData[k], fV[i]);
            num += fU[i][k] * fU[i][k] * dist * dist;
        }
    }
    
    //
    // calculate denominator
    //
    
    Double_t min = 1e30;

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        // loop over clusters
        for (Int_t j = i+1; j < fC; j++)
        {
            min = TMath::Min(GetDistance(fV[i], fV[j]), min);
        }
    }

    return num / (fN * min);
}

//______________________________________________________________________________
Double_t TOFCM::CalculateKwonIndex()
{
    // Return the Kwon cluster validity index.

    Double_t sum_1 = 0;
    Double_t sum_2 = 0;

    //
    // calculate numerator
    //

    // calculate v_bar
    Double_t v_bar[fDim];
    for (Int_t d = 0; d < fDim; d++) v_bar[d] = 0;
    for (Int_t i = 0; i < fN; i++)
    {
        for (Int_t d = 0; d < fDim; d++) v_bar[d] += fData[i][d] / fN;
    }

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        // loop over data points
        for (Int_t k = 0; k < fN; k++)
        {
            Double_t dist = GetDistance(fData[k], fV[i]);
            sum_1 += fU[i][k] * fU[i][k] * dist * dist;
        }

        // calculate punishing function
        Double_t dist = GetDistance(fV[i], v_bar);
        sum_2 += dist * dist / fC;
    }
    
    //
    // calculate denominator
    //
    
    Double_t min = 1e30;

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        // loop over clusters
        for (Int_t j = i+1; j < fC; j++)
        {
            min = TMath::Min(GetDistance(fV[i], fV[j]), min);
        }
    }

    return (sum_1 + sum_2) / min;
}

//______________________________________________________________________________
Double_t TOFCM::CalculateSVFIndex()
{
    // Return the SV cluster validity index.

    Double_t num = 0;
    Double_t denom= 0;

    //
    // calculate numerator
    //

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        Double_t min = 1e30;
        
        // loop over clusters
        for (Int_t j = 0; j < fC; j++)
        {
            if (j != i) min = TMath::Min(GetDistance(fV[i], fV[j]), min);
        }

        num += min;
    }
    
    //
    // calculate denominator
    //

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        Double_t max = 0;

        // loop over data points
        for (Int_t j = 0; j < fN; j++)
        {
            max = TMath::Max(GetDistance(fData[j], fV[i])*TMath::Power(fU[i][j], fM), max);
        }

        denom += max;
    }
    
    return num / denom;
}

//______________________________________________________________________________
void TOFCM::CalculateClusterMembership()
{
    // Calculate the cluster membership for each data point according to the fuzzy
    // membership grade u_ik.

    // loop over data points
    for (Int_t k = 0; k < fN; k++)
    {
        Double_t max = 0;
        Int_t cluster = -1;

        // loop over all clusters
        for (Int_t i = 0; i < fC; i++)
        {
            // check membership grade
            if (fU[i][k] > max)
            {
                max = fU[i][k];
                cluster = i;
            }

        }

        // set cluster membership
        fCM[k] = cluster;
    }
}

//______________________________________________________________________________
void TOFCM::InitU()
{
    // Init the membership matrix for a U-minimization.

    // initialize membership matrix
    for (Int_t i = 0; i < fN; i++)
    {   
        // membership matrix
        Double_t sum = 0;
        for (Int_t j = 0; j < fC; j++)
        {
            fU[j][i] = gRandom->Rndm();
            sum += fU[j][i];
        }

        // normalize sum of memberships for each data point to 1
        for (Int_t j = 0; j < fC; j++) fU[j][i] /= sum;
    }
}

//______________________________________________________________________________
void TOFCM::InitV(Double_t** vinit)
{
    // Init the cluster centers for a V-minimization.
    // If 'vinit' is not zero, the values in 'vinit' are used to initialize
    // the cluster centers.

    // check if cluster centers are initalized manually
    if (vinit)
    {
        // initialize cluster centers
        for (Int_t i = 0; i < fC; i++)
        {
            for (Int_t d = 0; d < fDim; d++) 
            {
                fV[i][d] = vinit[i][d];
            }
        }
    }
    else
    {
        // get data minima and maxima for every dimension
        Double_t dataMin[fDim];
        Double_t dataMax[fDim];
        for (Int_t i = 0; i < fN; i++)
        {
            for (Int_t d = 0; d < fDim; d++)
            {
                if (i == 0)
                {
                    dataMin[d] = fData[i][d];
                    dataMax[d] = fData[i][d];
                }
                else
                {
                    dataMin[d] = TMath::Min(fData[i][d], dataMin[d]);
                    dataMax[d] = TMath::Max(fData[i][d], dataMax[d]);
                }
            }
        }

        // initialize cluster centers
        for (Int_t i = 0; i < fC; i++)
        {
            for (Int_t d = 0; d < fDim; d++) 
            {
                fV[i][d] = gRandom->Uniform(dataMin[d], dataMax[d]);
            }
        }
    }
}

//______________________________________________________________________________
void TOFCM::InitV(Double_t* vinit, Int_t c)
{
    // Init the cluster center 'c' with 'vinit' for a V-minimization.

    for (Int_t d = 0; d < fDim; d++)
    {
        fV[c][d] = vinit[d];
    }
}

//______________________________________________________________________________
Bool_t TOFCM::MinimizeU(const Char_t* opt)
{
    // Method that performs the alternating optimization (AO) using the cycle
    // U(t-1) -> V(t) -> U(t).
    // Return kTRUE if the minimization converged, otherwise kFALSE.

    // quiet mode
    Bool_t quiet = kFALSE;
    if (!strcmp(opt, "q") || !strcmp(opt, "Q")) quiet = kTRUE;
    
    // iterate until convergence or maximum number of iterations reached
    Int_t t = 0;
    for (t = 0; t < fMaxIter; t++)
    {
        //
        // calculate cluster centers
        //

        // loop over cluster centers
        for (Int_t i = 0; i < fC; i++)
        {
            // loop over dimensions
            for (Int_t d = 0; d < fDim; d++)
            {
                Double_t num = 0;
                Double_t denom = 0;
                
                // loop over data points
                for (Int_t k = 0; k < fN; k++)
                {
                    num += TMath::Power(fU[i][k], fM) * fData[k][d];
                    denom += TMath::Power(fU[i][k], fM);
                }
                fV[i][d] = num / denom;
            }
        }

        //
        // update the membership matrix
        //

        Double_t u_new[fC][fN];
        
        // loop over clusters
        for (Int_t i = 0; i < fC; i++)
        {
            // loop over data points
            for (Int_t k = 0; k < fN; k++) 
            {
                Double_t sum = 0;

                // loop over clusters
                for (Int_t j = 0; j < fC; j++)
                {
                    Double_t dist_ik = GetDistance(fData[k], fV[i]);
                    Double_t dist_jk = GetDistance(fData[k], fV[j]);
                    if (dist_ik == 0) printf("singularity detected   %f =? %f / %f =? %f!\n", fData[k][0], fV[i][0], fData[k][1], fV[i][1]);
                    sum += TMath::Power(dist_ik/dist_jk, 2./(fM-1.));
                }
                u_new[i][k] = 1. / sum;
            }
        }
        
        //
        // calculate termination condition
        //
        
        // calculate membership matrix difference
        Double_t diff[fC][fN];
        for (Int_t i = 0; i < fC; i++)
        {
            for (Int_t k = 0; k < fN; k++)
            {
                diff[i][k] = u_new[i][k] - fU[i][k];
            }
        }

        // calculate 1-norm of difference matrix
        Double_t norm = 0;
        for (Int_t k = 0; k < fN; k++)
        {
            Double_t sum = 0;
            for (Int_t i = 0; i < fC; i++) sum += TMath::Abs(diff[i][k]);
            norm = TMath::Max(sum, norm);
        }

        // check norm 
        if (norm < fEps) break;

        // update membership matrix
        for (Int_t i = 0; i < fC; i++)
        {
            for (Int_t k = 0; k < fN; k++)
            {
                fU[i][k] = u_new[i][k];
            }
        }
    }
    
    // calculate cluster membership if converged
    if (t < fMaxIter) CalculateClusterMembership();

    // user info
    if (!quiet)
    {
        printf("Fuzzy C-Means minimizer status : ");
        if (t < fMaxIter) printf("CONVERGED\n");
        else printf("ABORTED\n");
        printf("Iterations: %d    Epsilon = %e    J = %e\n", 
               t+1, fEps, CalculateObjectiveFunction());
        printf("\n");
    }
    
    // return status
    if (t < fMaxIter) return kTRUE;
    else return kFALSE;
}

//______________________________________________________________________________
Bool_t TOFCM::MinimizeV(const Char_t* opt)
{
    // Method that performs the alternating optimization (AO) using the cycle
    // V(t-1) -> U(t) -> V(t).
    // Return kTRUE if the minimization converged, otherwise kFALSE.

    // quiet mode
    Bool_t quiet = kFALSE;
    if (!strcmp(opt, "q") || !strcmp(opt, "Q")) quiet = kTRUE;
    
    // iterate until convergence or maximum number of iterations reached
    Int_t t = 0;
    for (t = 0; t < fMaxIter; t++)
    {
        //
        // calculate the membership matrix
        //

        // loop over clusters
        for (Int_t i = 0; i < fC; i++)
        {
            // loop over data points
            for (Int_t k = 0; k < fN; k++) 
            {
                Double_t sum = 0;

                // loop over clusters
                for (Int_t j = 0; j < fC; j++)
                {
                    Double_t dist_ik = GetDistance(fData[k], fV[i]);
                    Double_t dist_jk = GetDistance(fData[k], fV[j]);
                    if (dist_ik == 0) printf("singularity detected   %f =? %f / %f =? %f!\n", fData[k][0], fV[i][0], fData[k][1], fV[i][1]);
                    sum += TMath::Power(dist_ik/dist_jk, 2./(fM-1.));
                }
                fU[i][k] = 1. / sum;
            }
        }
 
        //
        // update cluster centers
        //
        
        Double_t v_new[fC][fDim];

        // loop over cluster centers
        for (Int_t i = 0; i < fC; i++)
        {
            // loop over dimensions
            for (Int_t d = 0; d < fDim; d++)
            {
                Double_t num = 0;
                Double_t denom = 0;
                
                // loop over data points
                for (Int_t k = 0; k < fN; k++)
                {
                    num += TMath::Power(fU[i][k], fM) * fData[k][d];
                    denom += TMath::Power(fU[i][k], fM);
                }
                v_new[i][d] = num / denom;
            }
        }

        //
        // calculate termination condition
        //
        
        // calculate cluster center difference
        Double_t diff[fC][fDim];
        for (Int_t i = 0; i < fC; i++)
        {
            for (Int_t d = 0; d < fDim; d++)
            {
                diff[i][d] = v_new[i][d] - fV[i][d];
            }
        }

        // calculate 1-norm of difference matrix
        Double_t norm = 0;
        for (Int_t d = 0; d < fDim; d++)
        {
            Double_t sum = 0;
            for (Int_t i = 0; i < fC; i++) sum += TMath::Abs(diff[i][d]);
            norm = TMath::Max(sum, norm);
        }

        // check norm 
        if (norm < fEps) break;

        // update cluster centers
        for (Int_t i = 0; i < fC; i++)
        {
            for (Int_t d = 0; d < fDim; d++)
            {
                fV[i][d] = v_new[i][d];
            }
        }
    }
    
    // calculate cluster membership if converged
    if (t < fMaxIter) CalculateClusterMembership();

    // user info
    if (!quiet)
    {
        printf("Fuzzy C-Means minimizer status : ");
        if (t < fMaxIter) printf("CONVERGED\n");
        else printf("ABORTED\n");
        printf("Iterations: %d    Epsilon = %e    J = %e\n", 
               t+1, fEps, CalculateObjectiveFunction());
        printf("\n");
    }
    
    // return status
    if (t < fMaxIter) return kTRUE;
    else return kFALSE;
}

//______________________________________________________________________________
Double_t TOFCM::CalculateMembership(Double_t* v, Int_t c)
{
    // Calculate the membership of the data point 'v' to the cluster 'c'.
    
    Double_t sum = 0;

    // loop over clusters
    for (Int_t j = 0; j < fC; j++)
    {
        Double_t dist_cv = GetDistance(v, fV[c]);
        Double_t dist_jv = GetDistance(v, fV[j]);
        if (dist_cv == 0) printf("singularity detected   %f =? %f / %f =? %f!\n", v[0], fV[c][0], v[1], fV[c][1]);
        sum += TMath::Power(dist_cv/dist_jv, 2./(fM-1.));
    }
    
    return 1. / sum;
}

//______________________________________________________________________________
Double_t TOFCM::GetMaximumMembership(Double_t* v, Int_t* outCluster)
{
    // Return the maximum cluster membership of the data point 'v'.
    // If 'outCluster' is not zero write the cluster number into it.
    
    Double_t max = 0;
    Int_t cl = -1;

    // loop over clusters
    for (Int_t i = 0; i < fC; i++)
    {
        Double_t mship = CalculateMembership(v, i);
        if (mship > max)
        {
            cl = i;
            max = mship;
        }
    }

    // write cluster index
    if (outCluster) *outCluster = cl;

    return max;
}

//______________________________________________________________________________
void TOFCM::IncrementDataSet()
{
    // Increment the dataset from fN to fN+1 (check that fData contains a
    // sufficient number of elements).
    // The membership matrix and the cluster membership array are extended
    // and filled with the value of an existing data point that is closest
    // to the new data point.
    
    // create new arrays
    Double_t** u = new Double_t*[fC];
    Int_t* cm = new Int_t[fN+1];
    for (Int_t i = 0; i < fC; i++) u[i] = new Double_t[fN+1];

    // copy old values
    for (Int_t i = 0; i < fN; i++)
    {
        cm[i] = fCM[i];
        for (Int_t j = 0; j < fC; j++)
        {
            u[j][i] = fU[j][i];
        }
    }
    
    // set values for new point
    for (Int_t i = 0; i < fC; i++) u[i][fN] = CalculateMembership(fData[fN], i);
    Int_t maxMember;
    GetMaximumMembership(fData[fN], &maxMember);
    cm[fN] = maxMember;

    // delete old arrays
    for (Int_t i = 0; i < fC; i++) delete [] fU[i];
    delete [] fU;
    delete [] fCM;

    // link new arrays
    fU = u;
    fCM = cm;

    // increment number of points
    fN++;
}

//______________________________________________________________________________
void TOFCM::Print(Option_t* option) const
{
    // Print out the content of this class.

    printf("TOFCM content:\n");
    printf("Data points            : %d\n", fN);
    printf("Dimensions             : %d\n", fDim);
    printf("Distance measure       : ");
    switch (fDist)
    {
        case kFCMEuclidean:
            printf("Euclidean\n");
            break;
        case kFCMOnSphere:
            printf("On sphere\n");
            break;
    }
    printf("Clusters               : %d\n", fC);
    printf("Maximum iterations     : %d\n", fMaxIter);
    printf("Fuzziness              : %e\n", fM);
    printf("Epsilon                : %e\n", fEps);
    printf("Centers                :\n");
    for (Int_t i = 0; i < fC; i++)
    {
        for (Int_t j = 0; j < fDim; j++) printf("%6.3e  ", fV[i][j]);
        printf("\n");
    }
    printf("Membership matrix      :\n");
    for (Int_t i = 0; i < fC; i++)
    {
        for (Int_t j = 0; j < fN; j++) printf("%6.3e  ", fU[i][j]);
        printf("\n");
    }
    printf("Cluster membership     :\n");
    for (Int_t i = 0; i < fN; i++) printf("%d  ", fCM[i]);
    printf("\n");
}

