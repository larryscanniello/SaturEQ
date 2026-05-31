/*
  ==============================================================================

    FreqBandManager.h
    Created: 29 May 2026 4:19:11pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include "SecondOrderButterworth.h"
#include <vector>

class LinkwitzRileyManager {
    
public:
    void addSplit(float freq);
    void removeSplit(int splitNum);
    
    void splitSignal(
                     juce::AudioBuffer<float> &buf,
                     std::vector<juce::AudioBuffer<float>> &output,
                    );
    
    std::vector<float> getSplitFrequencies() {return frequencies;};
    
    int getSampleRate(){ return sampleRate;}
    void setSampleRate(int sr);
    
    
    
private:
    int sampleRate;
    int numChannels;
    std::vector<std::vector<SecondOrderButterworth>> filters; //This is size ((num bands * 2) - 2) by 2
    std::vector<float> frequencies;
    void deriveFiltersFromFrequencies();
};
