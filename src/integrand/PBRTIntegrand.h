//
//  PBRTIntegrand.h
//  eea
//
//  Created by Gurpreet Singh Bagga on 10/08/16.
//
//

#ifndef eea_PBRTIntegrand_h
#define eea_PBRTIntegrand_h


#include "integrand.h"
#include <Analyzer.h>
#include <string>
using std::string;

class PBRTIntegrand : public Integrand {

public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
    virtual double operator () (const Point2d& p) const ;
    virtual ~PBRTIntegrand() ;

protected:
    PBRTIntegrand() { RefVal = 0.0; IntegrandType = "Pbrt" ; }
    PBRTIntegrand(const vector<string>& IntegParams);
    friend class IntegrandPrototype;

private:

//    int _nspp;
//    static const string NsppStr; // "--nspp"

    std::string _pbrtSampler;
    static const string PbrtSamplerStr; // "--pbrtstype"

    int _ReferenceNspp;
    static const string RefNsppStr; // "--refnspp"

    double _crop[4];
    std::string _pathexec, _pathscene, _pathpyscript, _imageName;
    static const string CropStr ; // = "--crop"
    static const string PbrtExecPathStr ; // = "--epath"
    static const string PbrtScenePathStr ; // = "-- spath"
    static const string PythonScriptPathStr ; // = "--pypath"
    static const string ExrImgNameStr ; // = "--img"

    Analyzer* _analyzer;

    double computePBRTIntegral(std::string imageName, int NSPP, std::string samplerName) const;
};

#endif
