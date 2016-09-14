#ifndef __Point2dH_
#define __Point2dH_

#include <cmath>

namespace eea{

//////////////////////////////////////////////////////////
//	Simple structure to abstract 2D points
//////////////////////////////////////////////////////////
/// \brief The Point2d struct
///
struct Point2d
{
    Point2d(float x0=0, float y0=0) :x(x0), y(y0)
    { }

    Point2d(float x0, float y0, bool toroidal):x(x0), y(y0)
    {
        if (toroidal){
            x = x0 - floor(x0) ;
            y = y0 - floor(y0) ;
        }
    }

    float x, y;
};

}

#endif // __Point2dH_
