/*
  ==============================================================================

    EQManager.cpp
    Created: 6 Jun 2026 8:30:49pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "EQManager.h"


void EQManager::processBlock(juce::dsp::AudioBlock<float>& buffer)
{
    for(auto& filter: filters)
    {
        filter.processBlock(buffer);
    }
}


