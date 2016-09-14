//
//  PointAnalyzer.h
//  Pointset
//
//  Created by Gurpreet Singh Bagga on 14/08/16.
//
//

#ifndef Pointset_PointAnalyzer_h
#define Pointset_PointAnalyzer_h

#include <Analyzer.h>
#include <sampler.h>

class PointAnalyzer : public Analyzer{

public:
     void RunAnalysis(string& prefix);
     Analyzer* createAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);
    ~PointAnalyzer();
private:
    PointAnalyzer() { AnalyzerType = "pts" ;}
    PointAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

    friend class AnalyzerPrototype;

};



#endif
