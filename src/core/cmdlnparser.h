#ifndef __CMDLNPARSER_H__
#define __CMDLNPARSER_H__

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>



using std::string;
using std::vector;
using std::stringstream ;
using std::invalid_argument ;

///////////////////////////////////////////////////////////////////////////////////////
// constants used as labels for specifying command line arguments
namespace CLArg
{
    const string SamplerSecStr("-S") ; // Sampling section
    const string IntegSecStr("-I") ;   // Integrand section
    const string AnalSecStr("-A") ;    // Analyzer section
    const string GenSecStr("-G") ;     // General section

    const string AnalyzerType("--atype");
    const string SamplerType("--stype") ;
    const string IntegrandType("--itype") ;
    const string OutFile("--ofile") ;
} ;

///////////////////////////////////////////////////////////////////////////////////////
// Very basic Parser that splits the input command line string into a vector of strings
// Using the constants defined in CLArg, it separate the above vector into four vectors of strings
// according to the sampling, integrand, analysis and general section labels
//
// FindSwitch takes a string and tests if it is present in a given vector of strings
//            eg. FindSwitch(vec, "-A") returns true if an analysis section is found
// FindArgument takes a string and returns the argument immediately succeeding the given string in the vector
//            eg. if vec contains strings {"-S", "--stype", "Random"}
//                     FindArgument<string>(vec, "--stype")  returns "Random"
// FindMultiArgs returns multiple arguments after the test string until it finds a string of type "-*"
//            eg. if vec contains strings {"-I", "--itype", "QuadPix" --points .1 .2 .3 .4}
//                     FindMultiArgs<double>(vec, "--points")  returns a
//  			vector<double> containing {.1, .2, .3, .4}
//
// Important: Requires the command line to contain strings in the order -S ... -I ... -A ... -G ...
//
///////////////////////////////////////////////////////////////////////////////////////
class CLParser
{
    public:
    CLParser(int argc, char* argv[]) ;
    void IdentifySections() ;
    void ParseUnit() ;

    const vector<string>& SamplerSection() {return _samplerUnit;}
    const vector<string>& IntegSection() {return _integUnit;}
    const vector<string>& AnalSection() {return _analUnit;}
    const vector<string>& GenSection() {return _genUnit;}

    string OutFile() const {return FindArgument<string>(_genUnit, CLArg::OutFile);}
    //void generate_full_path(std::string &pwdir);

    static bool FindSwitch(const vector<string>& args, const string& switchStr) ;

    template<typename T>
    static T  FindArgument(const vector<string>& args, const string& argStr) ;

    template<typename T>
    static bool FindMultiArgs(int nargs, vector<T>& argOut, const vector<string>& args, const string& MultiArgsStr) ;
private:
    const int _argc ;
    char** _argv ;

    string _argvStr, _execMode ;
    vector<string> _samplerUnit, _analUnit, _integUnit, _genUnit ;

    //std::string _homedir, _datadir, _graphsdir, _imagesdir;

} ;


///////////////////////////////////////////////////////////////////////////////////////
//		Implementation of templatized members
///////////////////////////////////////////////////////////////////////////////////////
// FindArgument takes a string and returns the argument immediately succeeding the given string in the vector
//            eg. if vec contains strings {"-S", "--stype", "Random"}
//                     FindArgument<string>(vec, "--stype")  returns "Random"
template<typename T>
T CLParser::FindArgument(const vector<string>& args, const string& argStr)
{
    int found (args.size()) ;
    for (int i(0); i<args.size(); i++)
    {
        if (args[i]==argStr)
        {
            found = i ;
            break ;
        }
    }

    if (found<(args.size()-1))
    {
        T ret ;
        stringstream ss (args[found+1]) ;
        ss >> ret ;
        return ret ;
    }
    throw(invalid_argument("Did you forget to specify " + argStr + "?")) ;
    return T() ;
}

// FindMultiArgs returns multiple arguments after the test string until it finds a string of type "-*"
//            eg. if vec contains strings {"-I", "--itype", "QuadPix" --points .1 .2 .3 .4}
//                     FindMultiArgs<double>(vec, "--points")  returns a
//  			vector<double> containing {.1, .2, .3, .4}
template<typename T>
bool CLParser::FindMultiArgs(int nargs, vector<T>& argOut, const vector<string>& args, const string& MultiArgsStr)
{
    int found (args.size()) ;
    for (int i(0); i<args.size(); i++)
    {
        if (args[i]==MultiArgsStr)
        {
            found = i ;
            break ;
        }
    }

    if (nargs>0)
    {
        if (found<(args.size()-nargs))
        {
            argOut.resize(nargs) ;
            for (int i(0); i<nargs; i++)
            {
                T ret ;
                stringstream ss (args[found+i+1]) ;
                ss >> ret ;
                argOut[i] = ret;
            }
            return true ;
        }
        else if (found<args.size())
        {
            stringstream ss ;
            ss << nargs ;
            string nstr (ss.str()) ;

            throw(invalid_argument("Did you forget to specify enough parameters after " + MultiArgsStr + "?: Expecting " + nstr)) ;
        }
        else
            throw (invalid_argument("Did you forget to specify " + MultiArgsStr + "?\n"));
    }

    if (nargs<0)
    {
        int i(found+1) ;
        bool readingArgs(true) ;
        while(readingArgs && i<args.size())
        {
            if (args[i][0] != '-')
            {
                T ret ;
                stringstream ss (args[i]) ;
                ss >> ret ;
                argOut.push_back(ret);
            }
            else
                readingArgs=false;
            i++ ;
        }
        if (argOut.size())
            return true;
        else
            throw(invalid_argument("Did you forget to specify parameters after " + MultiArgsStr + "?: Expecting at least one since function is called with -ve #args")) ;
    }

    return  false ;
}

#endif // __CMDLNPARSER_H__
