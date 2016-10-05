#include <PWConstIntegrand.h>
#include <cmdlnparser.h>
// #include <CGAL/Polygon_2_algorithms.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			Piecewise constant simplicial complex in 2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand *PWConstantIntegrand::GenIntegrand(const vector<string>& IntegParams)
{
    return new PWConstantIntegrand(IntegParams) ;
}

const string PWConstantIntegrand::RandStr = "--random" ;
const string PWConstantIntegrand::NPtsStr = "--npts" ;

// Evaluation at p returns weight associated with the triangle in which p is located
double PWConstantIntegrand::operator () (const Point2d& p) const
{
    return 0;//  _dt.locate(Point(p.x, p.y))->info() ;
}

PWConstantIntegrand::~PWConstantIntegrand()
{}

////////////////////////////////////////////////////////////////////////////////
// Constructor:
//     Generates random points within unit square, triangulates them and associates
//        random a weight with each of them.
// The reference value is the sum of weight times the area
////////////////////////////////////////////////////////////////////////////////

PWConstantIntegrand::PWConstantIntegrand(const vector<string>& IntegParams)
{
    IntegrandType = "PWConstant";

    _randomize=true ; // for now this is the only option

    _npts = CLParser::FindArgument<double>(IntegParams, NPtsStr) ;

    // vector<Point> vp(_npts+4) ;
    // vp[0] =  Point(0, 0) ;
    // vp[1] =  Point(0, 1) ;
    // vp[2] =  Point(1, 0) ;
    // vp[3] =  Point(1, 1) ;
    // for (int i(0); i<_npts; i++)
    //     vp[i+4] = Point(MyRandom(), MyRandom()) ;

    // _dt.insert(vp.begin(), vp.end()) ;


    double refInt(0);
    // for(Finite_faces_iterator fc = _dt.finite_faces_begin(); fc != _dt.finite_faces_end(); ++fc)
    // {
    //     const double r(MyRandom()) ;
    //     fc->info() = r;

    //     Triangulation::Vertex_handle vh[3] ;
    //     for (int i(0); i<3; i++) vh[i] = fc->vertex(i) ;

    //     refInt +=  r*(CGAL::area<K>(vh[0]->point(), vh[1]->point(), vh[2]->point()));
    // }

    RefVal = refInt ;
}
