//
//  DiskIntegrand.h
//  eea
//
//  Created by Gurpreet Singh Bagga on 23/07/16.
//
//

#ifndef eea_DiskIntegrand_h
#define eea_DiskIntegrand_h

#include <integrand.h>
using namespace eea;

class DiskIntegrand : public Integrand {

public:
  virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
  virtual double operator () (const Point2d& p, const string &SamplerType) const ;
  virtual ~DiskIntegrand() ;

protected:
  DiskIntegrand() { RefVal = 0.0; IntegrandType = "Disk" ; }
  DiskIntegrand(const vector<string>& IntegParams);
  friend class IntegrandPrototype;

private:
  double _diskRadius;
  Point2d _diskCenter;
  static const string RadiusStr ; // = "--rad"
  static const string CenterStr ; // = "--center"
};

#endif
