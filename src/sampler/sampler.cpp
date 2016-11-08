#include <sampler.h>
#include <cmdlnparser.h>

#include <stdexcept>
#include <unistd.h>
#include <sstream>
#include <random>
#include <thread>
#include <iostream>

<<<<<<< HEAD
#if HAS_CXX11_THREAD_LOCAL
    #define ATTRIBUTE_TLS thread_local
#elif defined (__GNUC__)
    #define ATTRIBUTE_TLS __thread
#elif defined (_MSC_VER)
    #define ATTRIBUTE_TLS __declspec(thread)
#else // !C++11 && !__GNUC__ && !_MSC_VER
    #error "Define a thread local storage qualifier for your compiler/platform!"
#endif
=======
#if defined(_OPENMP)
#ifdef __APPLE__
#include <libiomp/omp.h>
#else
#include <omp.h>
#endif
#endif

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb

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
<<<<<<< HEAD
        vs.push_back(new gridSampler());
        vs.push_back(new jitteredSampler());
        vs.push_back(new gjSampler());
        vs.push_back(new bjSampler());
        vs.push_back(new latinhypercubeSampler());
        vs.push_back(new haltonSampler());
        vs.push_back(new sobolSampler());
        vs.push_back(new zerotwosequenceSampler());
        // vs.push_back(new MyNewSampler()); // add a line like this
=======
    vs.push_back(new gridSampler());
    vs.push_back(new jitteredSampler());
    vs.push_back(new gjSampler());
    vs.push_back(new bjSampler());
    // vs.push_back(new MyNewSampler()); // add a line like this
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb

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


vector<Point2d>& Sampler::Sample(int n)
{
    MTSample(p, n) ;
    return p;
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

void randomSampler::MTSample(vector<Point2d>& pts, int n) const
{
    pts.resize(n) ;
    double maxRange = bBoxMax - bBoxMin;
<<<<<<< HEAD
=======
    std::random_device rd;

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    for (int i=0; i<n; i++)
    {
        ///
        /// \brief Thread safe version of random number generator
        ///
<<<<<<< HEAD
//        static thread_local std::mt19937 generatorX;
//        std::uniform_real_distribution<double> distribution(0,1);
//        double x = distribution(generatorX);
//        double y = distribution(generatorY);
        double x = drand48();
        double y = drand48();
        ///
        /// Change the range of (x,y) from [0,1) to [bBoxMin, bBoxMax)
        ///
        x = (maxRange * x) + bBoxMin;
        y = (maxRange * y) + bBoxMin;

        pts[i] = Point2d(x,y);
=======
        static thread_local std::mt19937 generator(rd());
        std::uniform_real_distribution<double> distribution(0,1);
        double tx = distribution(generator);
        double ty = distribution(generator);
        ///
        /// Change the range of (x,y) from [0,1) to [bBoxMin, bBoxMax)
        ///
        double px = (maxRange * tx) + bBoxMin;
        double py = (maxRange * ty) + bBoxMin;

        pts[i] = Point2d(px,py);
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
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

void jitteredSampler::MTSample(vector<Point2d> &pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);

    pts.resize(n) ;

    double maxRange = bBoxMax - bBoxMin;
<<<<<<< HEAD

    #pragma omp parallel for
    for (int r=0; r<sqrtN; r++){
        for (int c=0; c<sqrtN; c++){
            double x = (c + drand48()) * dX;
            double y = (r + drand48()) * dY;
=======
    std::random_device rd;

#pragma omp parallel for
    for (int r=0; r<sqrtN; r++){
        for (int c=0; c<sqrtN; c++){
            ///
            /// \brief Thread safe version of random number generator
            ///
            static thread_local std::mt19937 generator(rd());
            std::uniform_real_distribution<double> distribution(0,1);
            double tx = distribution(generator);
            double ty = distribution(generator);

            double x = (c + tx) * dX;
            double y = (r + ty) * dY;
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb

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

void gridSampler::MTSample(vector<Point2d>& pts, int n) const
{
    int sqrtN (ceil(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);

    pts.resize(n) ;

   #pragma omp parallel for
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

void gjSampler::MTSample(vector<Point2d>& pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);
    pts.resize(n) ;
<<<<<<< HEAD
    std::normal_distribution<double> ND(0,1);

=======

    std::random_device rd;
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
<<<<<<< HEAD
        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(ND(RGen)*dX*.5), r2(ND(RGen)*dX*.5);
=======
        ///
        /// \brief Thread safe version of random number generator
        ///
        static thread_local std::mt19937 generator(rd());
        std::normal_distribution<double> distribution(0,1);
        double tx = distribution(generator);
        double ty = distribution(generator);

        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(tx*dX*.5), r2(ty*dX*.5);
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
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

void bjSampler::MTSample(vector<Point2d>& pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);
    pts.resize(n) ;
<<<<<<< HEAD
=======
    std::random_device rd;
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb

    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
<<<<<<< HEAD
        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(-.5+drand48()), r2(-.5+drand48());
=======
        ///
        /// \brief Thread safe version of random number generator
        ///
        static thread_local std::mt19937 generator(rd()  );
        std::uniform_real_distribution<double> distribution(0,1);
        double tx = distribution(generator);
        double ty = distribution(generator);

        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(-.5+tx), r2(-.5+ty);
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
        pts[i*sqrtN+j] = Point2d(x+(r1)*_boxWidth*dX,y+(r2)*_boxWidth*dY, true);
    }
    }
}


<<<<<<< HEAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				LatinHyperCube
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* latinhypercubeSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new latinhypercubeSampler(SamplerParams) ;
}

latinhypercubeSampler::latinhypercubeSampler(const vector<string>& SamplerParams)
{
    SamplingType = "latinhypercube" ;
}

void latinhypercubeSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Halton
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* haltonSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new haltonSampler(SamplerParams) ;
}

haltonSampler::haltonSampler(const vector<string>& SamplerParams)
{
    SamplingType = "halton" ;
}

void haltonSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Sobol
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* sobolSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new sobolSampler(SamplerParams) ;
}

sobolSampler::sobolSampler(const vector<string>& SamplerParams)
{
    SamplingType = "sobol" ;
}

void sobolSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				02sequence
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* zerotwosequenceSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new zerotwosequenceSampler(SamplerParams) ;
}

zerotwosequenceSampler::zerotwosequenceSampler(const vector<string>& SamplerParams)
{
    SamplingType = "02sequence" ;
}

void zerotwosequenceSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}
=======

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb




