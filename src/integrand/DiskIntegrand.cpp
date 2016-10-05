//
//  DiskIntegrand.cpp
//  eea
//
//  Created by Gurpreet Singh Bagga on 23/07/16.
//
//

#include <DiskIntegrand.h>
#include <cmdlnparser.h>

#include <iostream>
#include <memory>

const string DiskIntegrand::RadiusStr = "--rad" ;
const string DiskIntegrand::CenterStr = "--center" ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				White Disk within a black pixels
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand* DiskIntegrand::GenIntegrand(const vector<string>& IntegParams)
{
  return new DiskIntegrand(IntegParams) ;
}

/////////////////////////////////////////////
// Constructor:
/////////////////////////////////////////////
DiskIntegrand::DiskIntegrand(const vector<string>& IntegParams)
{
  IntegrandType = "Disk";
  _diskRadius = CLParser::FindArgument<double>(IntegParams, RadiusStr) ;

  std::vector<double> MultiArgs;
  CLParser::FindMultiArgs<double>(2, MultiArgs, IntegParams, CenterStr) ;

  _diskCenter = Point2d(MultiArgs[0], MultiArgs[1]);

  RefVal = M_PI * _diskRadius * _diskRadius;
}

/////////////////////////////////////////////////////////////
// Evaluation returns 1 if p is inside the disk and zero otherwise
/////////////////////////////////////////////////////////////

double DiskIntegrand::operator () (const Point2d& p) const
{
  double eval(0);
  double cx = p.x - _diskCenter.x;
  double cy = p.y - _diskCenter.y;
  eval = (sqrt(cx*cx + cy*cy) < _diskRadius) ? 1: 0 ;
  return eval ;
}

DiskIntegrand::~DiskIntegrand()
{}

