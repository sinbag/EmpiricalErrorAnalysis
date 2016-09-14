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

class PBRTIntegrand : public Integrand {

public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
    virtual double operator () (const Point2d& p, const string &SamplerType) const ;
    virtual ~PBRTIntegrand() ;

protected:
    PBRTIntegrand() { RefVal = 0.0; IntegrandType = "Pbrt" ; }
    PBRTIntegrand(const vector<string>& IntegParams);
    friend class IntegrandPrototype;

private:
    double _crop[4];
    std::string _pathexec, _pathscene, _pathpyscript, _imgname;
    static const string CropStr ; // = "--crop"
    static const string PbrtExecPathStr ; // = "--epath"
    static const string PbrtScenePathStr ; // = "-- spath"
    static const string PythonScriptPathStr ; // = "--pypath"
    static const string ExrImgNameStr ; // = "--img"

    Analyzer* _analyzer;
};

#endif
