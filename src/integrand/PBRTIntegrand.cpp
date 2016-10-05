//
//  PBRTIntegrand.cpp
//  eea
//
//  Created by Gurpreet Singh Bagga on 10/08/16.
//
//

#include <iostream>
#include <PBRTIntegrand.h>
#include <cmdlnparser.h>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h> //to use getcwd(cwd, sizeof(cwd));
#include <write-exr.h>
#include <read-image.h>
#include <common.h>

const string PBRTIntegrand::PbrtSamplerStr = "--pbrtstype";
const string PBRTIntegrand::RefNsppStr = "--refnspp";
//const string PBRTIntegrand::NsppStr = "--nspp";

const string PBRTIntegrand::CropStr = "--crop" ;
const string PBRTIntegrand::PbrtExecPathStr = "--epath" ;
const string PBRTIntegrand::PbrtScenePathStr = "--spath" ;
const string PBRTIntegrand::PythonScriptPathStr = "--pypath" ;
const string PBRTIntegrand::ExrImgNameStr = "--img" ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				PBRT Integrator to render a given pixel
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand *PBRTIntegrand::GenIntegrand(const vector<string>& IntegParams)
{
    return new PBRTIntegrand(IntegParams) ;
}

/////////////////////////////////////////////
// Constructor:
/////////////////////////////////////////////
PBRTIntegrand::PBRTIntegrand(const vector<string>& IntegParams)
{
    IntegrandType = "Pbrt";
    RefVal = 0.0;
    _pathexec = CLParser::FindArgument<std::string>(IntegParams, PbrtExecPathStr) ;
    _pathscene = CLParser::FindArgument<std::string>(IntegParams, PbrtScenePathStr) ;
    _pathpyscript = CLParser::FindArgument<std::string>(IntegParams, PythonScriptPathStr) ;
    _imageName = CLParser::FindArgument<std::string>(IntegParams, ExrImgNameStr) ;
    _pbrtSampler = CLParser::FindArgument<std::string>(IntegParams, PbrtSamplerStr) ;
    _ReferenceNspp = CLParser::FindArgument<int>(IntegParams, RefNsppStr) ;

    std::vector<double> MultiArgs;
    CLParser::FindMultiArgs(4, MultiArgs, IntegParams, CropStr) ;

    for(int i = 0; i < 4; i++)
        _crop[i] = MultiArgs[i];

    std::cerr << "Computing PBRT reference image using " << _ReferenceNspp << " samples..." << std::endl;
    RefVal = computePBRTIntegral("reference.exr", _ReferenceNspp, "halton");
    std::cerr << "reference image computed!!!" << std::endl;
}


/////////////////////////////////////////////////////////////
//` Evaluation returns the pixel radiance computed from the
// PBRT that is on your system.
// User need to modify their PBRT version to render only the
// given input pixel (_xpixel, _ypixel) and
// to make PBRT return a value for that pixel instead
// of an image. Use the following flags:
// --pixel : pixel coordinates to be rendered from the PBRT code
// --epath : path to your PBRT executable on your machine
// --spath : path to your PBRT Scene file (.pbrt)
// --pypath: path to the python script to modify .pbrt file according to
// command line arguments provided

// NOTE: argument Point2d p is used to get the number of samples
// in the operator call below.
// PBRTIntegrand uses the samplers directly from the PBRT code.
/////////////////////////////////////////////////////////////
/// \brief PBRTIntegrand::computePBRTIntegral
/// \param imageName
/// \param NSPP
/// \param samplerName
/// \return
///
double PBRTIntegrand::computePBRTIntegral(std::string imageName, int NSPP, std::string samplerName) const{

    std::stringstream ss;

    /// Python script to update the crop window size in the pbrt scene file (.pbrt) with
    /// the values provided from the command line.
    /// Passing arguments to the python script
    /// Provide N x1 x2 y1 y2 from the command line to fill _crop[4]
    ss << "python " << _pathpyscript << " " << _pathscene << " " << samplerName  << " "
       << NSPP << " " << _crop[0] << " " << _crop[1] << " " << _crop[2] << " " <<  _crop[3];


    /// Running python script to change the crop window size of the PBRT Scene File
    std::system(ss.str().c_str());

    /// reinitialize ss stringstream
    ss.str(std::string());

    /// prepare stringstream to call the pbrt with the scenefile
    ss << _pathexec << " " << _pathscene << " --outfile " <<  imageName << " --quiet";

    /// Call PBRT
    std::system(ss.str().c_str());

    /// Read the image generated from PBRT
    int width =0, height =0;
    float *pixels;
    if(!read_exr_rgb(imageName, pixels, width, height)){
        std::cerr << "PBRTIntegrand: Couldn't load the pbrt-eea.exr file !!!" << std::endl;
        std::cerr << "aborting..." << std::endl;
        exit(-1);
    }

    ///
    ///Uncomment to verify that *pixels carry the correct image;
    ///
    //    write_exr_rgb("test.exr", pixels, width, height);

    ///
    /// Average the image over all the pixels to return the output value
    /// There are four channels RGBA, we don't consider the A channel
    ///
    double integral = 0.0;
    for(int i=0; i< 3 * width * height; i++){
        integral += pixels[i];
    }

    integral /= float(3.0 * width *height);

    return integral;
}


double PBRTIntegrand::operator () (const Point2d& p) const
{
    /// For PBRTIntegrand the p argument contains the number of samples information
    //    int N = _nspp;

    double integral = computePBRTIntegral(_imageName, p.x, _pbrtSampler);
    return integral;
}

PBRTIntegrand::~PBRTIntegrand()
{}

