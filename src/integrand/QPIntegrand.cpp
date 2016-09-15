#include <QPIntegrand.h>
#include <cmdlnparser.h>
#include <iostream>
using namespace std;

const string QuadPixelIntegrand::RandStr = "--random" ;
const string QuadPixelIntegrand::PointsStr = "--points" ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				White quad within black pixel
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand* QuadPixelIntegrand::GenIntegrand(const vector<string>& IntegParams)
{
    return new QuadPixelIntegrand(IntegParams) ;
}

/////////////////////////////////////////////
// Constructor:
//   Either randomly choose 4 points on the edges of a unit square
//     or use the input values. using --points cmd line option
//   The reference value is the area of the quad formed by connecting the 4 points
/////////////////////////////////////////////
QuadPixelIntegrand::QuadPixelIntegrand(const vector<string>& IntegParams)
{
    IntegrandType = "QuadPix";

    _randomize = CLParser::FindSwitch(IntegParams, RandStr) ;

    vector<double> MultiArg;
    CLParser::FindMultiArgs<double>(4, MultiArg, IntegParams, PointsStr) ;


    // initialise points
    double f[4] ;
    for (int i(0); i<4;i++)
        f[i] = _randomize ? MyRandom():MultiArg[i] ;
    pts[0] = Point(0,f[0]) ;
    pts[1] = Point(f[1],1) ;
    pts[2] = Point(1,1-f[2]) ;
    pts[3] = Point(1-f[3],0) ;

    RefVal = 1 - 0.5f*(f[0] + f[1] + f[2] + f[3] - (f[0]*f[1] +
                f[1]*f[2] + f[2]*f[3] + f[3]*f[0]) ) ;

// 	cout << "Refval = " << RefVal << endl ;
}

/////////////////////////////////////////////////////////////
// Evaluation returns 1 if p is inside the quad and zero otherwise
/////////////////////////////////////////////////////////////
double QuadPixelIntegrand::operator () (const Point2d& p) const
{
    double eval(0);
    Point pt(p.x, p.y) ;
    CGAL::Bounded_side test = CGAL::bounded_side_2(pts, pts+4, pt, K()) ;
    eval =  (test == CGAL::ON_BOUNDED_SIDE) ? 1: 0 ;
    return eval ;
}

QuadPixelIntegrand::~QuadPixelIntegrand()
{}

