#include <iostream>
#include <fstream>

#include <cmdlnparser.h>
#include <sampler.h>
#include <integrand.h>
#include <Analyzer.h>

int main(int argc, char* argv[]){

    srand48(time(NULL));
    CLParser clarg(argc, argv) ;
    clarg.IdentifySections();

    // create sampler object from the -S section of command line
    // implemented as a virtual constructor
    // treat this as a call to the new operator, and delete the object s1 responsibly
    Sampler* s1 = SamplerPrototype::Generate(clarg.SamplerSection());

    // create analyzer object from the -A section of command line
    // implemented as a virtual constructor
    Analyzer* a = AnalyzerPrototype::Generate(s1, clarg.AnalSection(), clarg.IntegSection());

    string ofname(clarg.OutFile()) ;
    a->RunAnalysis(ofname);


    delete s1;
    delete a;

    return 0;
}

