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
    
    std::vector<size_t> curr;
    
    Parameters::EQ::Band* params;
    
    CoefficientStrategy strategy;
    
    bool bypass;
    
    void smoothen();

    bool useParams;
    
    std::vector<float> a;
    std::vector<float> b;
    
    std::optional<float> fc;
    std::optional<float> Q;
    std::optional<float> gainInDB;
    
    juce::dsp::ProcessSpec spec;
    
public:
    
    Filter(Parameters::EQ::Band& p,CoefficientStrategy strategy);
    
    Filter(float fc,float Q,float gainInDB,CoefficientStrategy strategy);
    
    virtual ~Filter() = default;
    
    void update();
    
    void updateCoefficients(float fc, float Q, float gainInDB)
    {
        std::visit([&](auto& s) {
              s.updateCoefficients(spec.sampleRate, fc, Q, gainInDB, a, b);
          }, strategy);
        this->fc = fc;
        this->Q = Q;
        this->gainInDB = gainInDB;
    }
    
    void setCoefficientStrategy(CoefficientStrategy s)
    {
        strategy = s;
    }
    
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    
    void resetDelayLines();
    
    void putSample(float sample,int channel);
    
    float getSample(int channel) {return y[channel][curr[channel]];};
    
    void processBlock(juce::dsp::AudioBlock<float> input, juce::dsp::AudioBlock<float> output);
    
    void processBlock(juce::dsp::AudioBlock<float> buf);
};
