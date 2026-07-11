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
#include "../../Parameters/Parameters.h"
#include "HighPass.h"
#include "LowPass.h"
#include "Peaking.h"

class Filter
{
    
public:
    using CoefficientStrategy = std::variant<LowPass, HighPass, Peaking>;
    
protected:
    
    
    std::vector<std::vector<float>> x;
    std::vector<std::vector<float>> y;
    
    int curr = 0;
    
    std::vector<float> a;
    std::vector<float> b;
    
    Parameters::EQ::Band params;
    
    CoefficientStrategy strategy;
    
    bool bypass;
    
    void smoothen();
    
    juce::dsp::ProcessSpec spec;
    
    bool useParams;
    
public:
    virtual ~Filter() = default;
    
    void update();
    
    void updateCoefficients(float fc, float Q, float gainInDB)
    {
        std::visit([&](auto& s) {
              s.updateCoefficients(spec.sampleRate, fc, Q, gainInDB, a, b);
          }, strategy);
    }

    Filter(Parameters::EQ::Band p,
           CoefficientStrategy strategy)
    :  params(p), strategy(strategy), useParams(true)
    {
        std::visit([&](auto& s) {
            a.resize(s.aSize);
            b.resize(s.bSize);
        },strategy);
        
        std::visit([&](auto& s) {
              s.updateCoefficients(spec.sampleRate, *p.fc, *p.Q, *p.gainInDB, a, b);
          }, strategy);
    }
    
    Filter(float fc,
           float Q,
           float gainInDB,
           CoefficientStrategy strategy)
    : strategy(strategy), useParams(false)
    {
        std::visit([&](auto& s) {
            a.resize(s.aSize);
            b.resize(s.bSize);
        },strategy);
        
        std::visit([&](auto& s) {
            s.updateCoefficients(spec.sampleRate,fc,Q,gainInDB,a,b);
        },strategy);
        
        
    }
    
    void setCoefficientStrategy(CoefficientStrategy s)
    {
        strategy = s;
    }
    
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    
    void resetDelayLines();
    
    void putSample(float sample,int channel);
    float getSample(int channel) {return y[channel][curr];};
    void processBlock(juce::dsp::AudioBlock<float> input, juce::dsp::AudioBlock<float> output);
    void processBlock(juce::dsp::AudioBlock<float> buf);
};
