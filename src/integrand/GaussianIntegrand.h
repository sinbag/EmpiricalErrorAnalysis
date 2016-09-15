//
//  GaussianIntegrand.h
//  eea
//
//  Created by Gurpreet Singh Bagga on 23/07/16.
//
//

#ifndef eea_GaussianIntegrand_h
#define eea_GaussianIntegrand_h

#include "integrand.h"

class GaussianIntegrand : public Integrand {

public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
    virtual double operator () (const Point2d& p) const ;
    virtual ~GaussianIntegrand() ;
    double ReferenceValue() const {return RefVal;}

protected:
  GaussianIntegrand() { RefVal = 0.0; IntegrandType = "Gaussian" ; }
  GaussianIntegrand(const vector<string>& IntegParams);
  friend class IntegrandPrototype;

private:
  double _xsigma, _ysigma;
  Point2d _gaussianCenter;
  static const string SigmaStr ; // = "--sigma"
  static const string CenterStr ; // = "--center"
};

#endif
