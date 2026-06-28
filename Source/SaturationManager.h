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
#include "LinkwitzRileyManager.h"

class SaturationManager {
    
public:
    SaturationManager(Parameters::Saturation& params) : params(params), lrManager(params) {};
    
    void processBands(std::vector<juce::dsp::AudioBlock<float>> blocks);
    
    void update();
    
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    
    const std::vector<juce::dsp::AudioBlock<float>> splitSignal(juce::dsp::AudioBlock<float> &input)
    {
        return lrManager.splitSignal(input);
    }
    
    juce::dsp::AudioBlock<float> sumSignal(juce::dsp::AudioBlock<float> &output)
    {
        return lrManager.sumSignal(output);
    }
    
    void addSplit(float freq);
    
    void removeSplit(int splitNum);
    
private:
    std::vector<Saturator> saturators;
    
    Parameters::Saturation& params;
    
    LinkwitzRileyManager lrManager;
    
};
