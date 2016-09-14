#include <Analyzer.h>
#include <PointAnalyzer.h>
#include <cmdlnparser.h>
#include <sampler.h>
#include <iostream>
#include <fstream>
#include <common.h>

using namespace std;
using namespace eea;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			PointAnalyzer to visualize points that are written directly in EPS format.
////////////////////////////////////////////////////////////////////////////////////////////////////////////

///
/// \brief PointAnalyzer::~PointAnalyzer
///

PointAnalyzer::~PointAnalyzer(){}

Analyzer* PointAnalyzer::createAnalyzer(Sampler *s, const vector<string> &AnalyzerParams, const vector<string> &IntegString){
    return new PointAnalyzer(s, AnalyzerParams, IntegString);
}

PointAnalyzer::PointAnalyzer(Sampler* s, const vector<string>& AnalyzerParams, const vector<string> &IntegString) {

    AnalyzerType = "pts";
    _sampler = s;
    CLParser::FindMultiArgs<int>(-1, _nSamples, AnalyzerParams, NSampStr) ;
    _nTrials = CLParser::FindArgument<int>(AnalyzerParams, nTrialsStr) ;

}

void PointAnalyzer::RunAnalysis(string &prefix){

    for(int j = 0; j <_nSamples.size();j++){

        const int n(_nSamples[j]) ;
        _sampler->MTSample(_pts, n);

        ///
        ///Uncomment this function if your samples are going out of the domain range
        ///
        //_sampler->toroidalWrapping(_pts);

        std::stringstream ss;
        ss << prefix << "-" << _sampler->GetType() << "-n" << n << ".txt";
        string filename = ss.str();
        IO::WriteEPS(filename, _pts);
    }
}

