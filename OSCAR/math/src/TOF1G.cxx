// SVN Info: $Id: TOF1G.cxx 1266 2012-08-02 11:48:14Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2012
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TOF1G                                                                //
//                                                                      //
// TOF1 based function evaluating a sum of graphs.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TOF1G.h"

ClassImp(TOF1G)


//______________________________________________________________________________
TOF1G::TOF1G(const Char_t* name, Int_t ngraph, TGraph** graphs, Double_t xmin, Double_t xmax)
    : TOF1(name, xmin, xmax, ngraph)
{
    // Constructor.
    
    Char_t tmp[256];

    // check number of graphs
    if (ngraph < 1)
    {
        Error("TOF1G", "Number of graphs has to be greater than 0! (was %d)", ngraph);
        return;
    }
    
    // init members
    fNGraph = ngraph;
    fGraph = new TGraph*[fNGraph];
    
    // set parameter names
    for (Int_t i = 0; i < fNGraph; i++)
    {
        sprintf(tmp, "Constant_%d", i);
        SetParName(i, tmp);
    }

    // copy the graphs to keep them in this class
    // (used for serialization)
    for (Int_t i = 0; i < fNGraph; i++)
    {
        // check if graph exists
        if (graphs[i])
        {
            // clone the graph
            fGraph[i] = (TGraph*) graphs[i]->Clone();
        }
        else
        {
            Error("TOF1G", "Could not load all %d graphs!", fNGraph);
            
            // cleanup and leave
            delete [] fGraph;
            fNGraph = 0;
            fGraph = 0;
            return;
        }
    }
}

//______________________________________________________________________________
TOF1G::~TOF1G()
{
    // Destructor.
    
    if (fGraph)
    {
        for (Int_t i = 0; i < fNGraph; i++) delete fGraph[i];
        delete [] fGraph;
    }
}

//______________________________________________________________________________
Double_t TOF1G::Evaluate(Double_t* x, Double_t* par)
{
    // Return the parameter-weighted sum of the graphs.

    Double_t out = 0.;
 
    // add graph contributions
    for (Int_t i = 0; i < fNGraph; i++) 
        out += par[i] * fGraph[i]->Eval(x[0]);
    
    return out;
}

//______________________________________________________________________________
void TOF1G::Express(Char_t* outString)
{
    // Write the function expression to 'outString'.
    
    Char_t tmp[256];

    // clear output string
    outString[0] = '\0';

    // loop over all graphs
    for (Int_t i = 0; i < fNGraph; i++)
    {
        if (i > 0) strcat(outString, "+");
        sprintf(tmp, "TH1(%s)", fGraph[i]->GetName());
        strcat(outString, tmp);
    }
}

//______________________________________________________________________________
TGraph* TOF1G::GetGraph(Int_t n) const
{
    // Return the 'n'-th graph.

    // check graph index
    if (n >= fNGraph || n < 0)
    {
        Error("GetGraph", "Cannot find graph with index '%d'! (number of graphs: %d)", n, fNGraph);
        return 0;
    }
    else
    {
        return fGraph[n];
    }
}

