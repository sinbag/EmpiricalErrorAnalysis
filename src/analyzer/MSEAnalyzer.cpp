#include "MSEAnalyzer.h"
#include <cmdlnparser.h>
#include <integrand.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>

using namespace std;

///////////////////////////////////////////////
// MSEAnalyzer class
// requires (pointers to) sampler and integrand objects
// and the portion of the commandline meant for the MSEAnalyzer
//
// It uses the sampler and the integrand to estimate the estimator's
// convergence rate and the y-intercept of the convergence plot (log-log)
///////////////////////////////////////////////

// expected format for the analysis section is
// -A --nsamps n1 n2 n3 n4 ... --nreps r --atype a
//
// n1 n2 n3 n4 are integers eg. 10 100 500 1000 to be used as sample counts
// r is an integer specifying the number of n1-sample estimates to be averaged for computing
//    error at n1 (equal to number of n2-sample estimates to be averaged for error at n2, etc.)
// a is a string that can be "mse" and is used to output the MSE at n1, n2 ...
//
// convergence rate is the slope of the best-fit line to
//  log([n1 n2 n3 n4]) vs mse estimated at those sample counts points.
//


///
/// \brief MSEAnalyzer::createAnalyzer
/// \param s
/// \param AnalyzerParams
/// \param IntegString
/// \return
///
/// MSEAnalyzer should only be used if the Reference Value (RefVal) of the integrand is known
///

Analyzer* MSEAnalyzer::createAnalyzer(Sampler *s, Integrand* I, const vector<string>& AnalyzerParams){
    return new MSEAnalyzer(s, I, AnalyzerParams);
}

MSEAnalyzer::~MSEAnalyzer(){
}

MSEAnalyzer::MSEAnalyzer(Sampler* s, Integrand *I, const vector<string>& AnalyzerParams)
    : _integrand(I){

    AnalyzerType = "mse";
    _sampler = s;
    CLParser::FindMultiArgs<int>(-1, _nSamples, AnalyzerParams, NSampStr) ;
    _nTrials = CLParser::FindArgument<int>(AnalyzerParams, nTrialsStr) ;
}

namespace // some functions to compute simple statistics of vector<double>
{
    inline double Mean(const vector<double>& v)
    {
        double m(0);
        for(int i(0); i<v.size(); i++) m+=v[i] ;
        return m/v.size() ;
    }

    inline double Var(const vector<double>& v)
    {
        double m(Mean(v));
        double var(0);
        for(int i(0); i<v.size(); i++) var+=(v[i]-m)*(v[i]-m) ;
        ///
        /// Subtraction by 1 to apply Bessel's correction
        ///
        return var/(v.size()-1.0) ;
    }

    inline void LogLogLinearFit(const vector<int>& x, const vector<double>& y, double& m, double& b)
    {
        double Xsum(0), X2sum(0), Ysum(0), XYsum(0) ;
        for(int i(0); i<y.size(); i++)
        {
            const double lx (log(static_cast<double>(x[i]))) ;
            const double ly (log(y[i])) ;

            Xsum += lx ;
            Ysum += ly ;
            X2sum += lx*lx ;
            XYsum += lx*ly ;
        }

        const int n(x.size()) ;
        m = (n*XYsum - Xsum*Ysum) / (n*X2sum - Xsum*Xsum)  ;
        b=(X2sum*Ysum-Xsum*XYsum)/(X2sum*n-Xsum*Xsum);
    }
}

// Main routine.
//
void MSEAnalyzer::RunAnalysis(string& prefix)
{

    //########################################################################################################
    ///
    /// We output two files:
    /// prefix-xxx-matlab.txt contains variance data horizontally to plot directly from matlab
    /// prefix-mean.txt contains the mean value for each N as reference
    ///
    std::stringstream ss;

    ss.str(std::string());
    ss << prefix << "-mean-" << _integrand->GetType() << "-" << _sampler->GetType() << ".txt";
    std::ofstream ofsmean(ss.str().c_str(), std::ofstream::app) ;

    ss.str(std::string());
    ss << prefix << "-mse-" << _integrand->GetType() << "-" << _sampler->GetType() << ".txt";
    std::ofstream ofsmse(ss.str().c_str(), std::ofstream::app) ;

    ofsmean << std::fixed << std::setprecision(15);
    ofsmse << std::fixed << std::setprecision(15);
    //########################################################################################################

    const double Iref (_integrand->ReferenceValue()) ;

    _avgM.resize(_nSamples.size()) ;
    _MSE.resize(_nSamples.size()) ;

    for (int i=0; i<_nSamples.size(); i++)
    {
        _avgM[i]=0;
        _MSE[i]=0 ;
    }

    for (int i=0; i<_nSamples.size(); i++)
    {
        const int n(_nSamples[i]) ;
        vector<double> ms(_nTrials,0) ;

        for (int r=0; r<_nTrials; r++)
        {
            ///Uncomment this line to monitor the progress
            //fprintf(stderr, "\r %d / %d : %d", trial, _nTrials, n);

            vector<Point2d> S;
            _sampler->Sample(S, n) ;

            vector<double> res ;
            _integrand->MultipointEval(res, S) ;
            double m = Mean(res);
            ms[r] = m ;
            _avgM[i] += m ;
            _MSE[i] += (Iref-m)*(Iref-m) ;
        }
        _avgM[i] /= float(_nTrials) ;
        _MSE[i] /= float(_nTrials) ;

       ofsmean << n << " "<< _avgM[i] << std::endl;
       ofsmse << n << " "<< _MSE[i] << std::endl;

    }
    LogLogLinearFit(_nSamples, _MSE, _convRate, _YIntError);
    std::cerr << std::endl;
}


