// SVN Info: $Id: TOF1G.h 1266 2012-08-02 11:48:14Z werthm $

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


#ifndef OSCAR_TOF1G
#define OSCAR_TOF1G

#include "TGraph.h"

#include "TOF1.h"


class TOF1G : public TOF1
{

protected:
    Int_t fNGraph;                               // number of graphs
    TGraph** fGraph;                             //[fNGraph] array of graphs

public:
    TOF1G() : TOF1(), fNGraph(0), fGraph(0) { }
    TOF1G(const Char_t* name, Int_t ngraph, TGraph** graphs, Double_t xmin, Double_t xmax);
    virtual ~TOF1G();
    
    virtual Double_t Evaluate(Double_t* x, Double_t* par);
    virtual void Express(Char_t* outString);
    
    TGraph* GetGraph(Int_t n) const; 

    ClassDef(TOF1G, 1)  // Function using a sum of graphs
};

#endif

