#ifndef __QPINTEGRANDH_
#define __QPINTEGRANDH_

#include <integrand.h>

// CGAL is used to test if points are within the quad
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2_algorithms.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				White quad within black pixel
// Quad defined by
//    1) 4 points on the left, top, right and bottom edge of a [0,1]x[0,1] pixel
//    2) 4 random values along pixel edges, connected to form a quad
//
// Param settings
//   1) 4 floats to choose points on L, T, R, B of [0,1]x[0,1]
//   2) RandomPoints=true overrides above
//
// Evaluation returns 1 if inside the quad and 0 outside
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QuadPixelIntegrand : public Integrand
{
     public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
    virtual double operator () (const Point2d& p, const string &SamplerType) const ;
    virtual ~QuadPixelIntegrand() ;

     protected:
    QuadPixelIntegrand(): _randomize(false) { RefVal = 0 ; IntegrandType = "QuadPix"; }
    QuadPixelIntegrand(const vector<string>& IntegParams);
    friend class IntegrandPrototype;

     private:
    typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
    typedef K::Point_2 Point;

    Point pts[4];
    static const string PointsStr ; // = "--points"


    bool _randomize ;
    static const string RandStr ; // = "--rand"
} ;

#endif
