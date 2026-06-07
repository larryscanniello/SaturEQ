/*
  ==============================================================================

    Filter.h
    Created: 27 May 2026 12:18:58am
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include <vector>
#include <JuceHeader.h>
#include "Parameters.h"

class Filter
{
    
public:
    class CoefficientStrategy {
    public:
        virtual ~CoefficientStrategy() = default;
        virtual void updateCoefficients(size_t sampleRate, float fc, float Q, float gainInDB, std::vector<float>& a, std::vector<float>& b);
    };
    
protected:
    virtual ~Filter() = default;
    
    std::vector<std::vector<float>> x;
    std::vector<std::vector<float>> y;
    
    int curr = 0;
    
    std::vector<float> a;
    std::vector<float> b;
    
    size_t sampleRate;
    
    Parameters::EQParams::FilterParams params;
    
    CoefficientStrategy strategy;
    
public:
    
    
    
    Filter(size_t numChannels,
           size_t sampleRate,
           Parameters::EQParams::FilterParams p,
           CoefficientStrategy s
           )
    : sampleRate(sampleRate), params(p), strategy(s)
    {
        x.resize(numChannels);
        y.resize(numChannels);
        strategy.updateCoefficients(sampleRate,*p.fc,*p.Q,*p.gainInDB,a,b);
    }
    
    void setCoefficientStrategy(CoefficientStrategy s)
    {
        strategy = s;
    }
    
    void resetDelayLines();
    
    void putSample(float sample,int channel);
    float getSample(int channel) {return y[channel][curr];};
    void processBlock(juce::dsp::AudioBlock<float> &input, juce::dsp::AudioBlock<float> &output);
    void processBlock(juce::dsp::AudioBlock<float> &buf);
};
