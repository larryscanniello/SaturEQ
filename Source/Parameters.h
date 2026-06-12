//
//  Parameters.h
//  SaturEQ
//
//  Created by Larry Scanniello on 5/15/26.
//

#pragma once

#include <JuceHeader.h>
#include "ParamDeclarations.h"

class Parameters
{
public:
    
    struct EQ
    {
        EQ(juce::AudioProcessorValueTreeState &aptvs);
        
        struct Band
        {
            juce::AudioParameterFloat* fc;
            juce::AudioParameterFloat* gainInDB;
            juce::AudioParameterFloat* Q;
            juce::AudioParameterBool* bypass;
            
            juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>* fcSmoother;
            juce::LinearSmoothedValue<float>* gainInDBSmoother;
            juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>* QSmoother;
        };
        
        Band getParamsForBand(size_t bandNum)
        {
            return params[bandNum];
        };
        
        private:
            std::vector<Band> params;
    };

    struct Saturation
    {
        Saturation(juce::AudioProcessorValueTreeState &apvts);
        
        struct Band {
            juce::AudioParameterBool* bypass;
            juce::AudioParameterFloat* preGain;
            
            juce::LinearSmoothedValue<float>* preGainSmoother;
            
            juce::AudioParameterFloat* freqBandStart;
        };
        
        Band getParamsForBand(size_t bandNum)
        {
            return params[bandNum];
        }
        
        size_t getNumBands()
        {
            params.size();
        }
        
        
        private:
            std::vector<Band> params;
            
    };
    
    
    Parameters(juce::AudioProcessorValueTreeState& apvts,juce::dsp::ProcessSpec &spec);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;
    void setDefaults() noexcept;
    
    float freq = 200.0f;
    
    EQ eqParams;
    Saturation saturationParams;
    
};
