/*
  ==============================================================================

    EQManager.h
    Created: 6 Jun 2026 8:30:49pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "Filter.h"


class EQManager {
    
public:
    EQManager(size_t sr, size_t numBands)
    {
        sampleRate = sr;
        changeNumBands(numBands);
    }
    
    void processBlock(juce::dsp::AudioBlock<float>& buffer);
    
    
    
private:
    
    void changeNumBands(size_t numBands);
    
    size_t sampleRate;
    
    std::vector<float> eqFreqs;
    std::vector<float> eqGainsInDB;
    std::vector<float> eqQs;
    std::vector<bool> eqBypasses;

    std::vector<Filter> filters;
    
};
