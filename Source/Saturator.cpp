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

void Saturator::processBlock(juce::dsp::AudioBlock<float> &input)
{
    for(size_t channel=0; channel<input.getNumChannels(); channel++)
    {
        auto* channelPtr = input.getChannelPointer(channel);
        
        for(size_t sample=0; sample<input.getNumSamples(); sample++)
        {
            channelPtr[sample] = std::tanh(channelPtr[sample]);
        }
        
    }
}
