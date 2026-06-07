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
    
    struct EQParams
    {
        std::vector<juce::AudioParameterFloat*> freqParams;
        std::vector<juce::AudioParameterFloat*> eqGainInDBParams;
        std::vector<juce::AudioParameterFloat*> eqQParams;
        std::vector<juce::AudioParameterBool*> eqBypassParams;
        
        std::vector<juce::LinearSmoothedValue<float>> eqFreqSmoothers;
        std::vector<juce::LinearSmoothedValue<float>> eqGainInDBSmoothers;
        std::vector<juce::LinearSmoothedValue<float>> eqQSmoothers;
    };
    
    struct SaturationParams
    {
        std::vector<juce::AudioParameterBool*> saturationBypassParams;
        std::vector<juce::AudioParameterFloat*> saturationPreGainParams;
        
        std::vector<juce::LinearSmoothedValue<float>> saturationPreGainSmoothers;
    };
    
    EQParams eqParams;
    SaturationParams saturationParams;
    
};
