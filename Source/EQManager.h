/*
  ==============================================================================

    EQManager.h
    Created: 6 Jun 2026 8:30:49pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "ParamDeclarations.h"
#include "Filter.h"
#include "HighPass.h"
#include "LowPass.h"
#include "Peaking.h"

class EQManager {
    
public:
    EQManager(Parameters::EQ eqParams) : eqParams(eqParams) {};
    
    void update();
    
    void processBlock(juce::dsp::AudioBlock<float>& buffer);
    
    void updateFilters(size_t numBands, juce::dsp::ProcessSpec spec);
    
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    
private:
    
    juce::dsp::ProcessSpec spec;
    
    void changeNumBands(size_t numBands);
    
    size_t sampleRate;
    size_t numChannels;
    
    Parameters::EQ eqParams;

    std::vector<Filter> filters;
    
};
