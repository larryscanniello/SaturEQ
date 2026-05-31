/*
  ==============================================================================

    LinkwitzRiley.h
    Created: 27 May 2026 12:18:30am
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include "./Filter.h"
#include <JuceHeader.h>
#include <vector>

class SecondOrderButterworth : public Filter
{
private:
    bool lowPass;
    
    std::vector<std::vector<float>> secondPassx;
    std::vector<std::vector<float>> secondPassy;
    
public:
    
    SecondOrderButterworth(float sr, int inputs, bool lowPass)
    : Filter(sr,inputs), lowPass(lowPass) {}
    
    SecondOrderButterworth(float sr, int inputs, float freq, float Q, float dB, bool lowPass)
    : Filter(sr, inputs, freq, Q, dB), lowPass(lowPass) {}
    
    void updateCoefficients(float centerFreq, float Q, float dB);
    
    
    
    void putSample(float sample,int channel);
    
    void processBlock(juce::AudioBuffer<float> &input, juce:: AudioBuffer<float> &output);
    
};
