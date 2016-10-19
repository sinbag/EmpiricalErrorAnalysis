#ifndef VARIANCEANALYZER_H
#define VARIANCEANALYZER_H

#include <Analyzer.h>
#include <integrand.h>
#include <fstream>

class VarianceAnalyzer : public Analyzer{

public:
      void RunAnalysis(string& prefix);
      Analyzer* createAnalyzer(Sampler *s, Integrand* I, const vector<string>& AnalyzerParams) ;
    ~VarianceAnalyzer();
private:
    VarianceAnalyzer() { AnalyzerType = "var" ; }
    VarianceAnalyzer(Sampler *s, Integrand* I, const vector<string>& AnalyzerParams);

    Integrand* _integrand;

    vector<double> _avgM, _avgV, _MSE ;

    friend class AnalyzerPrototype;
};

#endif // VARIANCEANALYZER_H
