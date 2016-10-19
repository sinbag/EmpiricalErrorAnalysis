#ifndef MSEANALYZER_H
#define MSEANALYZER_H

#include "Analyzer.h"
#include "./../integrand/integrand.h"
#include <fstream>

class MSEAnalyzer : public Analyzer
{
public:
    virtual void RunAnalysis(string& prefix);
    virtual Analyzer* createAnalyzer(Sampler *s, Integrand* I, const vector<string>& AnalyzerParams) ;
    ~MSEAnalyzer();
private:
    MSEAnalyzer() { AnalyzerType = "mse" ;}
    MSEAnalyzer(Sampler *s, Integrand* I, const vector<string>& AnalyzerParams);

    Integrand* _integrand;

    vector<double> _avgM, _avgV, _MSE ;

    double _convRate, _YIntError ;

    friend class AnalyzerPrototype;
};

#endif // MSEANALYZER_H
