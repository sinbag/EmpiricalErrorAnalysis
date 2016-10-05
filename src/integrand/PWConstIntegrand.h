#ifndef __PWCINTEGRANDH_
#define __PWCINTEGRANDH_

#include <integrand.h>

// CGAL is used to triangulate the piecewise constant integrand
// and for point location while evaluating the integrand
// #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Triangulation_euclidean_traits_2.h>
// #include <CGAL/Triangulation_2.h>
// #include <CGAL/Triangulation_face_base_with_info_2.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			Piecewise constant simplicial complex in 2D
// Defined by
//    1) random points within unit square along with 4 corners of the quad
//    2) random weights for each of the triangles each in [0,1]
// Params
//   1) --npts : number of points to be triangulated (within the unit square)
//
// Evaluation at p returns weight associated with the triangle in which p is located
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PWConstantIntegrand : public Integrand
{
     public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
    virtual double operator () (const Point2d& p) const ;
    virtual ~PWConstantIntegrand() ;

     protected:
    PWConstantIntegrand(): _randomize(true) { RefVal = 0 ; IntegrandType = "PWConstant"; }
    PWConstantIntegrand(const vector<string>& IntegParams);
    friend class IntegrandPrototype;

     private:
//    typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//    typedef CGAL::Triangulation_vertex_base_2<K> Vb;
//    typedef CGAL::Triangulation_face_base_with_info_2<double,K> Fb;
//    typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
//    typedef CGAL::Triangulation_2<K,Tds> Triangulation;
//    typedef K::Point_2 Point;
//    typedef Triangulation::Face_handle fh;
//    typedef Triangulation::Finite_faces_iterator Finite_faces_iterator;

    int _npts;
    static const string NPtsStr ; // = "--points"


    bool _randomize ;
    static const string RandStr ; // = "--rand"

//    Triangulation _dt;
    vector<double> _wts ;
} ;

#endif
