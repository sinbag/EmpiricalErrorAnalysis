#include <Analyzer.h>
#include <PointAnalyzer.h>
#include <FourierAnalyzer.h>
#include <VarianceAnalyzer.h>
#include <MSEAnalyzer.h>
#include <cmdlnparser.h>

#include <iostream>
#include <fstream>

#include <sstream>
#include <sys/stat.h>

using namespace eea;

const string Analyzer::NSampStr = "--nsamps" ;
const string Analyzer::nTrialsStr = "--nreps" ;

map<string, Analyzer*> AnalyzerPrototype::exemplars ;
AnalyzerPrototype a1;

Analyzer::~Analyzer(){}

/////////////////////////////////////////////
// When implementing a new type of analyzer,
//  say MyNewAnalyzer
// add an extra line to "MODIFY THIS" block
/////////////////////////////////////////////
AnalyzerPrototype::AnalyzerPrototype()
{
    vector<Analyzer*> vs ;
    vs.push_back(new PointAnalyzer());
    vs.push_back(new FourierAnalyzer());
    vs.push_back(new VarianceAnalyzer());
    vs.push_back(new MSEAnalyzer());
    // vs.push_back(new MyNewAnalyzer()); // add a line like this

    for (int i(0); i<vs.size(); i++)
    exemplars[vs[i]->GetType()] = vs[i] ;
}

Analyzer* AnalyzerPrototype::Generate(Sampler* s, Integrand *I, const vector<string>& AnalyzerString)
{
    string type = CLParser::FindArgument<string>(AnalyzerString, CLArg::AnalyzerType) ;

    std::cerr << "Analyzer type: " << type << std::endl;

    map<string, Analyzer*>::iterator it = exemplars.find(type) ;
    if (it==exemplars.end()) throw invalid_argument("Unknown analyzer type") ;
    return it->second->createAnalyzer(s, I, AnalyzerString) ;
}


