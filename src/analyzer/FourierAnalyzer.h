#ifndef FOURIERANALYZER_H
#define FOURIERANALYZER_H

#include <complex>
#include <Analyzer.h>
#include <sampler.h>

class FourierAnalyzer : public Analyzer{

public:
     void RunAnalysis(std::string &prefix);
     Analyzer* createAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

private:
    FourierAnalyzer() { AnalyzerType = "fourier" ;}
    ~FourierAnalyzer();
    FourierAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

    void continuous_fourier_spectrum();
    void discrete_fourier_spectrum(const double *mydata);

    void power_fourier_spectrum();

    void compute_radial_mean_powerspectrum(const std::string& filename);


    static const string trialStepStr; // --tstep
    int _trialStepOut;

    static const string freqStepStr; // --wstep
    float _frequencyStep;

    int _xRes, _yRes;
    float* _powerSpectrum;
    std::complex<float>* _complexSpectrum;

    friend class AnalyzerPrototype;
};

#endif // FOURIERANALYZER_H
