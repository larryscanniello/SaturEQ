/*
  ==============================================================================

    Saturator.h
    Created: 1 Jun 2026 2:26:11am
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include "../Parameters/Parameters.h"

class Saturator {

public:
    Saturator(Parameters::Saturation::Band params, juce::dsp::ProcessSpec spec) : params(params) {}
    
    void processBlock(juce::dsp::AudioBlock<float>& input);
    
    void update();
    
    void smoothen();
    
    juce::dsp::ProcessSpec spec;
    
private:
    Parameters::Saturation::Band params;
    
    float preGain;
    
    bool bypass;
};
