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

    Integrand* i1;

    if(clarg.IntegSection().size() > 3){
        // create integrand object from the -I section of command line
        // implemented as a virtual constructor
        // treat this as a call to the new operator, and delete the object i1 responsibly
        i1 = IntegrandPrototype::Generate(clarg.IntegSection()) ;
    }
    else{
        i1 = 0;
    }

    // create analyzer object from the -A section of command line
    // implemented as a virtual constructor
    Analyzer* a = AnalyzerPrototype::Generate(s1, i1, clarg.AnalSection());

    string ofname(clarg.OutFile()) ;
    a->RunAnalysis(ofname);


    delete a;

   //delete the object i1 responsibly
    if(clarg.IntegSection().size() > 3)
        delete i1;

    delete s1;

    return 0;
}

