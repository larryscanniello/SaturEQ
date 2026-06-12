/*
  ==============================================================================

    SaturationManager.h
    Created: 8 Jun 2026 1:30:32am
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include "Parameters.h"
#include "Saturator.h"
#include <vector>

class SaturationManager {
    
    SaturationManager(juce::dsp::ProcessSpec &spec, Parameters::Saturation params) : spec(spec), params(params)
    {};
    
    juce::dsp::ProcessSpec spec;
    
    void processBands(std::vector<juce::dsp::AudioBlock<float>> blocks);
    
    void update();
    
private:
    std::vector<Saturator> saturators;
    
    Parameters::Saturation params;
    
};
