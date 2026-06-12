/*
  ==============================================================================

    Saturator.cpp
    Created: 1 Jun 2026 2:26:11am
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "Saturator.h"
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>


void Saturator::update()
{
    params.preGainSmoother->setTargetValue(params.preGain->get());
    bypass = params.bypass;
}

void Saturator::smoothen()
{
    preGain = params.preGainSmoother->getNextValue();
}

void Saturator::processBlock(juce::dsp::AudioBlock<float> &input)
{
    if(bypass) return;
    
    for(size_t channel=0; channel<input.getNumChannels(); channel++)
    {
        auto* channelPtr = input.getChannelPointer(channel);
        
        for(size_t sample=0; sample<input.getNumSamples(); sample++)
        {
            smoothen();
            channelPtr[sample] = std::tanh(preGain * channelPtr[sample]);
        }
    }
}
