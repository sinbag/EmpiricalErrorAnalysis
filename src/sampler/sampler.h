#ifndef SAMPLER_H
#define SAMPLER_H

#include <ostream>
#include <string>
#include <vector>
#include <point2d.h>

#include <map>
#include <random>

using std::vector ;
using std::map ;
using std::string;
using std::ostream ;

class Sampler ;

using namespace eea;

///////////////////////////////////////////////
//	Basic implementation of the prototype
//  creational software design pattern, for
//  creating the appropriate type of integrand object
// given command line arguments for type and parameters.
///////////////////////////////////////////////
class SamplerPrototype
{
    public:
    SamplerPrototype() ;

    static Sampler* Generate(const vector<string>& SamplingSection) ;

    private:
    static map<string, Sampler*> exemplars;
};


///////////////////////////////////////////////
// Abstract base class for samplers
///////////////////////////////////////////////
class Sampler
{
public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams) = 0 ;
    virtual vector<Point2d>& Sample(int n) ; 			// simple sampling algo.
    virtual void MTSample(vector<Point2d>& pts, int n) const = 0;   // thread-safe version
    virtual string GetType() const {return SamplingType; }
    virtual vector<Point2d>& GetPoints() {return p;}
<<<<<<< HEAD
=======

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    virtual ~Sampler();

    Sampler(){bBoxMin = 0; bBoxMax = 1;}
    friend ostream& operator << (ostream& os, Sampler& s);

    void toroidal_wrapping(std::vector<Point2d> &inSamples);
    void homogenize_samples(std::vector<Point2d> &inSamples);

protected:
    vector<Point2d> p ;
    string SamplingType ;
    double bBoxMax, bBoxMin;
    mutable std::default_random_engine RGen;
};

// 				Subclasses of Sampler

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////
class randomSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~randomSampler() {}

    private:
<<<<<<< HEAD
=======

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    randomSampler() {SamplingType = "Random" ;}
    randomSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Jittered
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class jitteredSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~jitteredSampler() {}

    private:
    jitteredSampler() {SamplingType = "Jittered" ;}
    jitteredSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;
};

// 				Regular grid
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gridSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~gridSampler() {}

    private:
<<<<<<< HEAD
=======

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    gridSampler() {SamplingType = "Grid" ;}
    gridSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Gaussian jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gjSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~gjSampler() {}

    private:
<<<<<<< HEAD
=======

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    gjSampler(): _sigma(0.5) {SamplingType = "GJittered" ;}
    gjSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;

    void ParseParameters(const vector<string>& SamplerParams) ;

    double _sigma ;
    static const string SigStr ; // = "--sigma"
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Box-jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class bjSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~bjSampler() {}

    private:
<<<<<<< HEAD
=======

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    bjSampler(): _boxWidth(0.5) {SamplingType = "BJittered" ;}
    bjSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;

    void ParseParameters(const vector<string>& SamplerParams) ;

    double _boxWidth ;
    static const string BWStr ; // = "--boxwidth"
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Poisson-disk (NOT IMPLEMENTED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pdSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~pdSampler() {}

    private:
<<<<<<< HEAD
    pdSampler() {SamplingType = "PDisk" ;}
    pdSampler(const vector<string>& SamplerParams) {} ;
=======

    pdSampler() {SamplingType = "PDisk" ;}
    pdSampler(const vector<string>& SamplerParams) {}
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				latinhypercube (NOT IMPLEMENTED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class latinhypercubeSampler: public Sampler
{
public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~latinhypercubeSampler() {}

private:
    latinhypercubeSampler() {SamplingType = "LatinHypercube" ;}
<<<<<<< HEAD
    latinhypercubeSampler(const vector<string>& SamplerParams);
=======
    latinhypercubeSampler(const vector<string>& SamplerParams) {}
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				halton (NOT IMPLEMENTED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class haltonSampler: public Sampler
{
public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~haltonSampler() {}

private:
    haltonSampler() {SamplingType = "Halton" ;}
<<<<<<< HEAD
    haltonSampler(const vector<string>& SamplerParams) ;
=======
    haltonSampler(const vector<string>& SamplerParams) {}
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				sobol (NOT IMPLEMENTED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class sobolSampler: public Sampler
{
public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~sobolSampler() {}

private:
    sobolSampler() {SamplingType = "Sobol" ;}
<<<<<<< HEAD
    sobolSampler(const vector<string>& SamplerParams) ;
=======
    sobolSampler(const vector<string>& SamplerParams) {}
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				02sequence (NOT IMPLEMENTED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class zerotwosequenceSampler: public Sampler
{
public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~zerotwosequenceSampler() {}

<<<<<<< HEAD
private:
    zerotwosequenceSampler() {SamplingType = "02sequence" ;}
    zerotwosequenceSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;
};


=======

private:
    zerotwosequenceSampler() {SamplingType = "02sequence" ;}
    zerotwosequenceSampler(const vector<string>& SamplerParams) {}
    friend class SamplerPrototype;
};

>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
#endif // SAMPLER_H
