#include <sampler.h>
#include <cmdlnparser.h>

#include <stdexcept>
#include <unistd.h>
#include <sstream>
#include <random>
#include <thread>
#include <iostream>

#if defined(_OPENMP)
#ifdef __APPLE__
#include <libiomp/omp.h>
#else
#include <omp.h>
#endif
#endif

using std::cout ;
using std::stringstream ;
using std::endl ;
using std::invalid_argument ;

map<string, Sampler*> SamplerPrototype::exemplars ;
SamplerPrototype s1;


/////////////////////////////////////////////
// When implementing a new type of sampler,
//  say MyNewSampler
// add an extra line to "MODIFY THIS" block
/////////////////////////////////////////////
SamplerPrototype::SamplerPrototype()
{
    vector<Sampler*> vs ;
    vs.push_back(new randomSampler());
    vs.push_back(new gridSampler());
    vs.push_back(new jitteredSampler());
    vs.push_back(new gjSampler());
    vs.push_back(new bjSampler());
    // vs.push_back(new MyNewSampler()); // add a line like this

    for (int i(0); i<vs.size(); i++)
    exemplars[vs[i]->GetType()] = vs[i] ;
}

/////////////////////////////////////////////
// You should not need to modify this
/////////////////////////////////////////////
Sampler* SamplerPrototype::Generate(const vector<string>& SamplerString)
{
    string type = CLParser::FindArgument<string>(SamplerString, CLArg::SamplerType) ;

    map<string, Sampler*>::iterator it = exemplars.find(type) ;
    if (it==exemplars.end()) throw invalid_argument("Unknown sampler type") ;
    return it->second->GenSampler(SamplerString) ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& os, Sampler& s)
{
    for (int i(0); i< s.p.size(); i++)
    os << s.p[i].x << " " << s.p[i].y << endl ;
}

Sampler::~Sampler()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Homogenize samples
////////////////////////////////////////////////////////////////////////////////////////////////////////////

///
/// \brief Sampler::homogenize_samples
/// \param inSamples
///
void Sampler::homogenize_samples(std::vector<Point2d>& inSamples){

    int nPoints = inSamples.size();
    double domain[] = {bBoxMin, bBoxMin, bBoxMax, bBoxMax};

    Point2d random_shift_vector = Point2d((2*drand48()-1)*domain[2], (2*drand48()-1)*domain[3]);

    for(int i=0; i < nPoints; i++){

        inSamples[i].x = inSamples[i].x + random_shift_vector.x;
        inSamples[i].y = inSamples[i].y + random_shift_vector.y;

        if(inSamples[i].x < domain[0])
            inSamples[i].x = domain[2] + inSamples[i].x - domain[0];
        else if(inSamples[i].x > domain[2])
            inSamples[i].x = domain[0] + inSamples[i].x - domain[2];

        if(inSamples[i].y < domain[1])
            inSamples[i].y = domain[3] + inSamples[i].y - domain[1];
        else if(inSamples[i].y > domain[3])
            inSamples[i].y = domain[1] + inSamples[i].y - domain[3];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Toroidal wrapping of samples
////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sampler::toroidalWrapping between [0,1)
/// \param inSamples
///

void Sampler::toroidal_wrapping(std::vector<Point2d> &inSamples){

    int nPoints = inSamples.size();

    for(int i=0; i < nPoints; i++){

        if(inSamples[i].x < 0){
            double temp = - inSamples[i].x;
            int itemp = floor(temp);
            double frac = temp - itemp;
            inSamples[i].x = 1 - frac;
        }
        else if(inSamples[i].x > 1){
            int itemp = floor(inSamples[i].x);
            double frac = inSamples[i].x - itemp;
            inSamples[i].x = frac;
        }

        if(inSamples[i].y < 0){
            double temp = - inSamples[i].y;
            int itemp = floor(temp);
            double frac = temp - itemp;
            inSamples[i].y = 1 - frac;
        }
        else if(inSamples[i].y > 1){
            int itemp = floor(inSamples[i].y);
            double frac = inSamples[i].y - itemp;
            inSamples[i].y = frac;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* randomSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new randomSampler(SamplerParams) ;
}

randomSampler::randomSampler(const vector<string>& SamplerParams)
{
    SamplingType = "random" ;
}

void randomSampler::Sample(vector<Point2d>& pts, int n) const
{
    pts.resize(n) ;
    double maxRange = bBoxMax - bBoxMin;
    std::random_device rd;

    for (int i=0; i<n; i++)
    {
        ///
        /// \brief Thread safe version of random number generator
        ///
        static std::mt19937 generator(rd());
        std::uniform_real_distribution<double> distribution(0,1);
        double tx = distribution(generator);
        double ty = distribution(generator);
        ///
        /// Change the range of (x,y) from [0,1) to [bBoxMin, bBoxMax)
        ///
        double px = (maxRange * tx) + bBoxMin;
        double py = (maxRange * ty) + bBoxMin;

        pts[i] = Point2d(px,py);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Stratified/Jittered sampler (with one sample per strata)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* jitteredSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new jitteredSampler(SamplerParams) ;
}

jitteredSampler::jitteredSampler(const vector<string>& SamplerParams)
{
    SamplingType = "stratified" ;
}

void jitteredSampler::Sample(vector<Point2d> &pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);

    pts.resize(n) ;

    double maxRange = bBoxMax - bBoxMin;
    std::random_device rd;

    for (int r=0; r<sqrtN; r++){
        for (int c=0; c<sqrtN; c++){
            ///
            /// \brief Thread safe version of random number generator
            ///
            static std::mt19937 generator(rd());
            std::uniform_real_distribution<double> distribution(0,1);
            double tx = distribution(generator);
            double ty = distribution(generator);

            double x = (c + tx) * dX;
            double y = (r + ty) * dY;

            ///
            /// Change the range of (x,y) from [0,1) to [bBoxMin, bBoxMax)
            ///
            x = (maxRange * x) + bBoxMin;
            y = (maxRange * y) + bBoxMin;

            pts[r*sqrtN+c] = Point2d(x, y);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Regular grid
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* gridSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new gridSampler(SamplerParams) ;
}

gridSampler::gridSampler(const vector<string>& SamplerParams)
{
    SamplingType = "Grid" ;
}

void gridSampler::Sample(vector<Point2d>& pts, int n) const
{
    int sqrtN (ceil(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);

    pts.resize(n) ;

    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        pts[i*sqrtN+j] =  Point2d(x,y) ;
    }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Gaussian jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const string gjSampler::SigStr = "--sigma" ;

Sampler* gjSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new gjSampler(SamplerParams) ;
}

gjSampler::gjSampler(const vector<string>& SamplerParams) : _sigma (0.5)
{
    SamplingType = "GJittered" ;
    ParseParameters(SamplerParams) ;
}

void gjSampler::ParseParameters(const vector<string>& SamplerParams)
{
    _sigma  = CLParser::FindArgument<double>(SamplerParams, SigStr) ;
}

void gjSampler::Sample(vector<Point2d>& pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);
    pts.resize(n) ;
    std::random_device rd;

    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
        ///
        /// \brief Thread safe version of random number generator
        ///
        static std::mt19937 generator(rd());
        std::normal_distribution<double> distribution(0,1);
        double tx = distribution(generator);
        double ty = distribution(generator);

        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(tx*dX*.5), r2(ty*dX*.5);
        pts[i*sqrtN+j] = Point2d(x+(r1),y+(r2), true);
    }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Box-jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const string bjSampler::BWStr = "--boxwidth" ;
Sampler* bjSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new bjSampler(SamplerParams) ;
}

bjSampler::bjSampler(const vector<string>& SamplerParams) : _boxWidth(0.5)
{
    SamplingType = "BJittered" ;
    ParseParameters(SamplerParams) ;
}

void bjSampler::ParseParameters(const vector<string>& SamplerParams)
{
    _boxWidth = CLParser::FindArgument<double>(SamplerParams, BWStr) ;
}

void bjSampler::Sample(vector<Point2d>& pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);
    pts.resize(n) ;
    std::random_device rd;

    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
        ///
        /// \brief Thread safe version of random number generator
        ///
        static std::mt19937 generator(rd()  );
        std::uniform_real_distribution<double> distribution(0,1);
        double tx = distribution(generator);
        double ty = distribution(generator);

        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(-.5+tx), r2(-.5+ty);
        pts[i*sqrtN+j] = Point2d(x+(r1)*_boxWidth*dX,y+(r2)*_boxWidth*dY, true);
    }
    }
}





