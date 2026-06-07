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

class Filter
{
protected:
    virtual ~Filter() = default;
    std::vector<std::vector<float>> x;
    std::vector<std::vector<float>> y;
    
    int curr = 0;
    
    std::vector<float> a;
    std::vector<float> b;

    
public:
    
    Filter(size_t numChannels, float centerFreq,float Q, float dB, size_t sampleRate){
        x.resize(numChannels);
        y.resize(numChannels);
        resetDelayLines();
        updateCoefficients(centerFreq, Q, dB, sampleRate);
    }
    
    
    
    
    void resetDelayLines();
    virtual void updateCoefficients(float centerFreq, float Q, float dB,size_t sampleRate);
    
    void putSample(float sample,int channel);
    float getSample(int channel) {return y[channel][curr];};
    void processBlock(juce::dsp::AudioBlock<float> &input, juce::dsp::AudioBlock<float> &output);
    void processBlock(juce::dsp::AudioBlock<float> &buf);
};
