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
    EQManager(size_t sr, size_t numBands, size_t numChannels, Parameters::EQParams eqparams)
    : sampleRate(sr), numChannels(numChannels), eqParams(eqparams)
    {
        initializeFilterCoefficients(sr, numBands, numChannels, eqparams);
    };
    
    void processBlock(juce::dsp::AudioBlock<float>& buffer);
    
    void initializeFilterCoefficients(size_t sr, size_t numBands, size_t numChannels, Parameters::EQParams eqparams);
    
private:
    
    void changeNumBands(size_t numBands);
    
    size_t sampleRate;
    size_t numChannels;
    
    Parameters::EQParams eqParams;

    std::vector<Filter> filters;
    
};
