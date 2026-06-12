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
        EQ(juce::AudioProcessorValueTreeState &apvts, size_t numBands) : apvts(apvts)
        {
            params.resize(numBands);
        }
        
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
        
        void resize(size_t numBands)
        {
            params.resize(numBands);
        }
        
        private:
            std::vector<Band> params;
            juce::AudioProcessorValueTreeState& apvts;
    };

    struct Saturation
    {
        Saturation(juce::AudioProcessorValueTreeState &apvts, size_t numBands) : apvts(apvts)
        {
            params.resize(numBands);
        }
        
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
        
        void resize(size_t numBands)
        {
            params.resize(numBands);
        }
        
        private:
            std::vector<Band> params;
            juce::AudioProcessorValueTreeState& apvts;
            
    };
    
    
    
    
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void setDefaults() noexcept;
    
    juce::dsp::ProcessSpec spec;
    
    float freq = 200.0f;
    
    EQ eqParams;
    Saturation saturationParams;
    
};
