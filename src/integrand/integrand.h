#ifndef __INTEGRANDH_
#define __INTEGRANDH_

#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <point2d.h>
#include <memory>

using namespace eea;

using std::vector ;
using std::map ;
using std::string ;

class Integrand ;

///////////////////////////////////////////////
//	Basic implementation of the prototype
//  creational software design pattern, for
//  creating the appropriate type of integrand object
// given command line arguments for type and parameters.
///////////////////////////////////////////////
class IntegrandPrototype
{
    public:
    IntegrandPrototype() ;

    static Integrand* Generate(const vector<string>& IntegSection) ;

    private:
    static map<string, Integrand*> exemplars;
};

///////////////////////////////////////////////
// Abstract base class for Integrands
// See QPIntegrand for an example of how
// to extend this class.
// All integrands are assumed to be defined on [0,1]x[0x1]
///////////////////////////////////////////////
class Integrand
{
     public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) = 0 ;
    virtual double operator () (const Point2d& p) const = 0;
    virtual void MultipointEval (vector<double>& out, const vector<Point2d>& vp) const ;
    virtual double ReferenceValue() const {return RefVal;}
    virtual string GetType() const {return IntegrandType; }
    virtual ~Integrand() {}

     protected:
    double RefVal ; // reference value for the integral.
    string IntegrandType ;
} ;

///////////////////////////////////////////////
// Old school implementation of random number generator
///////////////////////////////////////////////
inline double MyRandom()
{
    static const double invrmax(1/static_cast<double>(RAND_MAX)) ;
    return rand()*invrmax ;
}


#endif //__INTEGRANDH_
