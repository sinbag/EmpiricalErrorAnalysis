#ifndef MSEANALYZER_H
#define MSEANALYZER_H

#include "Analyzer.h"
#include "./../integrand/integrand.h"
#include <fstream>

class MSEAnalyzer : public Analyzer
{
public:
    virtual void RunAnalysis(string& prefix);
    virtual Analyzer* createAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);
    ~MSEAnalyzer();
private:
    MSEAnalyzer() { AnalyzerType = "mse" ;}
    MSEAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

    Integrand* _integrand;

    vector<double> _avgM, _avgV, _MSE ;

    double _convRate, _YIntError ;

    friend class AnalyzerPrototype;
};

#endif // MSEANALYZER_H
