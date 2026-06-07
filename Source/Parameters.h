//
//  Parameters.h
//  SaturEQ
//
//  Created by Larry Scanniello on 5/15/26.
//

#pragma once

#include <JuceHeader.h>

const auto NUM_EQ_BANDS = 6;
const auto NUM_SATURATION_BANDS = 6;

namespace Parameters
{

struct EQParams
{
    struct FilterParams
    {
        juce::AudioParameterFloat* fc;
        juce::AudioParameterFloat* gainInDB;
        juce::AudioParameterFloat* Q;
        juce::AudioParameterBool* bypass;
        
        juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>* fcSmoother;
        juce::LinearSmoothedValue<float>* gainInDBSmoother;
        juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>* QSmoother;
    };
    
    
    std::vector<juce::AudioParameterFloat*> freqParams;
    std::vector<juce::AudioParameterFloat*> gainInDBParams;
    std::vector<juce::AudioParameterFloat*> QParams;
    std::vector<juce::AudioParameterBool*> bypassParams;
    
    std::vector<juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>*> freqSmoothers;
    std::vector<juce::LinearSmoothedValue<float>*> gainInDBSmoothers;
    std::vector<juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>*> QSmoothers;
    
    FilterParams getFilterParams(size_t filterNum)
    {
        size_t m = filterNum;
        FilterParams filterparams;
        filterparams.fc = freqParams[m];
        filterparams.gainInDB = gainInDBParams[m];
        filterparams.Q = QParams[m];
        filterparams.bypass = bypassParams[m];
        filterparams.fcSmoother = freqSmoothers[m];
        filterparams.gainInDBSmoother = gainInDBSmoothers[m];
        filterparams.QSmoother = QSmoothers[m];
    };
};

struct SaturationParams
{
    std::vector<juce::AudioParameterBool*> saturationBypassParams;
    std::vector<juce::AudioParameterFloat*> saturationPreGainParams;
    
    std::vector<juce::LinearSmoothedValue<float>> saturationPreGainSmoothers;
};

class Parameters
{
    public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;
    void setDefaults() noexcept;
    
    float freq = 200.0f;
    private:
    
    std::vector<float> saturationPreGains;
    std::vector<bool> saturationBypasses;
    
    EQParams eqParams;
    SaturationParams saturationParams;
    
};

};
