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
    
public:
    SaturationManager(Parameters::Saturation& params) : params(params) {};
    
    void processBands(std::vector<juce::dsp::AudioBlock<float>> blocks);
    
    void update();
    
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    
private:
    std::vector<Saturator> saturators;
    
    Parameters::Saturation& params;
    
};
