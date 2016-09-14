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
  RefVal = 0.0;
  std::vector<double> MultiArgCenter, MultiArgSigma;
  CLParser::FindMultiArgs<double>(2, MultiArgCenter, IntegParams, CenterStr) ;
  CLParser::FindMultiArgs<double>(2, MultiArgSigma, IntegParams, SigmaStr) ;

  _gaussianCenter = Point2d(MultiArgCenter[0], MultiArgCenter[1]);

  _xsigma = MultiArgSigma[0];
  _ysigma = MultiArgSigma[1];

}

/////////////////////////////////////////////////////////////
// Evaluation computes the Gaussian function, centered at _gaussianCenter,
// at a given location p
/////////////////////////////////////////////////////////////

double GaussianIntegrand::operator () (const Point2d& p, const string &SamplerType) const
{
  double eval(0);
  double cx = p.x - _gaussianCenter.x;
  double cy = p.y - _gaussianCenter.y;

  double normalizationFactor = 1.0;//1.0/(2*PI*_xsigma*_ysigma);
  double xRatio = cx / _xsigma;
  double yRatio = cy / _ysigma;
  eval  = normalizationFactor * exp(-0.5*(xRatio*xRatio + yRatio*yRatio));
  return eval ;
}

GaussianIntegrand::~GaussianIntegrand()
{}

