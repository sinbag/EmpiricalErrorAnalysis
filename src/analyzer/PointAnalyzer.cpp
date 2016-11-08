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

Analyzer* PointAnalyzer::createAnalyzer(Sampler *s, Integrand* I, const vector<string>& AnalyzerParams){
    return new PointAnalyzer(s, AnalyzerParams);
}

PointAnalyzer::PointAnalyzer(Sampler* s, const vector<string>& AnalyzerParams) {

    AnalyzerType = "pts";
    _sampler = s;
    CLParser::FindMultiArgs<int>(-1, _nSamples, AnalyzerParams, NSampStr) ;
    _nTrials = CLParser::FindArgument<int>(AnalyzerParams, nTrialsStr) ;
<<<<<<< HEAD

=======
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
}

void PointAnalyzer::RunAnalysis(string &prefix){

    for(int j = 0; j <_nSamples.size();j++){

<<<<<<< HEAD
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
=======
        for(int trial = 1; trial <= _nTrials; trial++){

            const int n(_nSamples[j]) ;
            _sampler->MTSample(_pts, n);

            ///
            ///Uncomment this function if your samples are going out of the domain range
            ///
            //_sampler->toroidalWrapping(_pts);

            ///
            /// Add index of each file with trailing zeros
            ///
            //##########################################################
            std::stringstream ss;
            ss.str(std::string());
            ss << trial;
            std::string s1 = ss.str();
            paddedzerosN(s1, _nTrials);
            //##########################################################
            ss.str(std::string());
            ss << prefix << "-" << _sampler->GetType() << "-n" << n << "-" << s1 << ".txt";
            string filename = ss.str();
            IO::WriteEPS(filename, _pts);
        }
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
    }
}

