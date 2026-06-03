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

class Saturator {

public:
    void processBlock(juce::dsp::AudioBlock<float>& input);
    
    
};
