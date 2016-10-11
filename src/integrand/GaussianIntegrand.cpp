//
//  GaussianIntegrand.cpp
//  eea
//
//  Created by Gurpreet Singh Bagga on 23/07/16.
//
//

#include <GaussianIntegrand.h>
#include <cmdlnparser.h>
#include <iostream>
//#include <boost/math/special_functions/erf.hpp>

const string GaussianIntegrand::SigmaStr = "--sigma" ;
const string GaussianIntegrand::CenterStr = "--center" ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Gaussian function
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand* GaussianIntegrand::GenIntegrand(const vector<string>& IntegParams)
{
    return new GaussianIntegrand(IntegParams) ;
}



/////////////////////////////////////////////
// Constructor:
/////////////////////////////////////////////
GaussianIntegrand::GaussianIntegrand(const vector<string>& IntegParams)
{
    IntegrandType = "Gaussian";
    std::vector<double> MultiArgCenter, MultiArgSigma;
    CLParser::FindMultiArgs<double>(2, MultiArgCenter, IntegParams, CenterStr) ;
    CLParser::FindMultiArgs<double>(2, MultiArgSigma, IntegParams, SigmaStr) ;

    _gaussianCenter = Point2d(MultiArgCenter[0], MultiArgCenter[1]);

    _xsigma = MultiArgSigma[0];
    _ysigma = MultiArgSigma[1];

    //RefVal = 0.0;

    double mux = _gaussianCenter.x;
    double muy = _gaussianCenter.y;

    double sqrt2sigmax = sqrt(2.0)*_xsigma;
    double sqrt2sigmay = sqrt(2.0)*_ysigma;
    double I1 = (std::erf((1-mux)/sqrt2sigmax) + std::erf(mux / sqrt2sigmax));
    double I2 = (-std::erf((-1+muy)/sqrt2sigmay) + std::erf(muy / sqrt2sigmay));
    //double I1 = (boost::math::erf((1-mux)/sqrt2sigmax) + boost::math::erf(mux / sqrt2sigmax));
    //double I2 = (-boost::math::erf((-1+muy)/sqrt2sigmay) + boost::math::erf(muy / sqrt2sigmay));

    RefVal = 0.25 * I1 * I2;
}

/////////////////////////////////////////////////////////////
// Evaluation computes the Gaussian function, centered at _gaussianCenter,
// at a given location p
/////////////////////////////////////////////////////////////

double GaussianIntegrand::operator () (const Point2d& p) const
{
    double eval(0);
    double cx = p.x - _gaussianCenter.x;
    double cy = p.y - _gaussianCenter.y;

    double normalizationFactor = 1.0/(2*M_PI*_xsigma*_ysigma);
    double xRatio = cx / _xsigma;
    double yRatio = cy / _ysigma;
    eval  = normalizationFactor * exp(-0.5*(xRatio*xRatio + yRatio*yRatio));
    return eval ;
}

GaussianIntegrand::~GaussianIntegrand(){
}

