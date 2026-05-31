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
    
    float sampleRate;
    
    
public:
    Filter(float sr, int inputs) : sampleRate(sr){
        x.resize(inputs);
        y.resize(inputs);
        resetDelayLines();
    };
    
    Filter(float sr, int inputs, float centerFreq,float Q, float dB){
        x.resize(inputs);
        y.resize(inputs);
        resetDelayLines();
        updateCoefficients(centerFreq, Q, dB);
    }
    
    void setSampleRate(int sr) { sampleRate = sr; };
    
    void resetDelayLines();
    virtual void updateCoefficients(float centerFreq, float Q, float dB);
    
    void putSample(float sample,int channel);
    float getSample(int channel) {return y[channel][curr];};
    void processBlock(juce::AudioBuffer<float> &input, juce:: AudioBuffer<float> &output);
};
