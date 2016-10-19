#include <iostream>
#include <stdexcept>
#include <cstdlib>

#if defined(_OPENMP)
#ifdef __APPLE__
#include <libiomp/omp.h>
#else
#include <omp.h>
#endif
#endif

#include <cmdlnparser.h>
#include <integrand.h>
#include <DiskIntegrand.h>
#include <GaussianIntegrand.h>
#include <QPIntegrand.h>
#include <PBRTIntegrand.h>
#include <PWConstIntegrand.h>


using std::vector ;
using std::map ;
using std::cout ;
using std::endl ;
using std::invalid_argument ;

map<string, Integrand*> IntegrandPrototype::exemplars ;
IntegrandPrototype i1;

/////////////////////////////////////////////
// When implementing a new type of integrand,
//  say MyNewIntegrand
// add an extra line to "MODIFY THIS" block
/////////////////////////////////////////////
IntegrandPrototype::IntegrandPrototype()
{
    vector<Integrand*> vi ;

    ////////////// MODIFY THIS /////////////
    vi.push_back(new DiskIntegrand());
    vi.push_back(new QuadPixelIntegrand());
    vi.push_back(new PWConstantIntegrand());
    vi.push_back(new GaussianIntegrand());
    vi.push_back(new PBRTIntegrand());
    // vi.push_back(new MyNewIntegrand());// add a line like this

    for (int i(0); i<vi.size(); i++)
        exemplars[vi[i]->GetType()] = vi[i] ;
}

/////////////////////////////////////////////
// You should not need to modify this
/////////////////////////////////////////////
Integrand *IntegrandPrototype::Generate(const vector<string>& IntegSection)
{
    string type = CLParser::FindArgument<string>(IntegSection, CLArg::IntegrandType) ;

    map<string, Integrand*>::iterator it = exemplars.find(type) ;
    if (it==exemplars.end())
        throw invalid_argument("Unknown integrand type") ;
    return it->second->GenIntegrand(IntegSection) ;
}


/////////////////////////////////////////////
// Evaluates the integrand at multiple points
// this is independent of how to evaluate the integrand
// the evaluation at each Point2d will be as
// specified in the derived class
/////////////////////////////////////////////
void Integrand::MultipointEval (vector<double>& out, const vector<Point2d>& vp) const
{
    string currIntegrand = GetType();

    ///
    /// For PBRTIntegrand we are using the samples directly provided within Pbrt source code,
    /// therefore, we use a dummy sample instead.
    ///
    if(currIntegrand == "Pbrt"){
        out.resize(1);

        //Store the size of the samples as a point to pass to the PBRTIntegrand
        Point2d dummySample(vp.size(), 0);
        out[0] = (*this)(dummySample);
    }
    else{
        const  int n (vp.size());
        out.resize(n) ;

//#pragma omp parallel for
        for(int i=0; i<n; i++)
            out[i] = (*this)(vp[i]) ;
    }
}



