#ifndef __VarianceAnalyzerH__
#define __VarianceAnalyzerH__

#include <iostream>
#include <vector>
#include <string>
#include <point2d.h>
#include <sampler.h>


using std::string ;
using std::cout ;
using std::endl ;
using std::vector ;

using namespace eea;

class Analyzer;

class AnalyzerPrototype{
public:
    AnalyzerPrototype() ;

    static Analyzer* Generate(Sampler *s, const vector<string>& AnalyzerString, const vector<string>& IntegString) ;

private:
    static map<string, Analyzer*> exemplars;

};

///////////////////////////////////////////////
// Abstract base class for analyzers
///////////////////////////////////////////////
class Analyzer{
public:
    virtual void RunAnalysis(string& prefix) = 0;
    //virtual void WriteFile(string& filename) const = 0;
    virtual Analyzer* createAnalyzer(Sampler *s, const vector<string>& SamplerParams, const vector<string>& IntegString) = 0 ;
    virtual string GetType() const {return AnalyzerType; }
    void create_folders(std::string homedir, std::string &data, std::string &images, std::string &graphs);
    virtual ~Analyzer();

protected:
    static const string nTrialsStr; // --nreps
    int _nTrials;

    static const string NSampStr; // --nsamps
    vector<int> _nSamples ; // secondary estimator sample sizes

    std::vector<Point2d> _pts;

    Sampler* _sampler ;

    string AnalyzerType;
};

#endif //__AnalyzerH__
