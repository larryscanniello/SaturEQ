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
    EQManager(juce::dsp::ProcessSpec Parameters::EQParams eqparams)
    : spec, eqParams(eqparams)
    {
        initializeFilterCoefficients(sr, numBands, numChannels, eqparams);
    };
    
    void update();
    
    void processBlock(juce::dsp::AudioBlock<float>& buffer);
    
    void initializeFilterCoefficients(Parameters::EQ eqparams);
    
private:
    
    juce::dsp::ProcessSpec spec;
    
    void changeNumBands(size_t numBands);
    
    size_t sampleRate;
    size_t numChannels;
    
    Parameters::EQParams eqParams;

    std::vector<Filter> filters;
    
};
