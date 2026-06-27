/*
  ==============================================================================

    SaturationManager.cpp
    Created: 8 Jun 2026 1:30:32am
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "SaturationManager.h"


void SaturationManager::update()
{
    for(auto &saturator : saturators)
    {
        saturator.update();
    }
}

void SaturationManager::processBands(std::vector<juce::dsp::AudioBlock<float>> blocks)
{
    jassert(saturators.size() == blocks.size());
    
    for(auto i=0; i<blocks.size(); i++)
    {
        saturators[i].processBlock(blocks[i]);
    }
}

void SaturationManager::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    saturators.clear();
    for(auto i=0; i<params.size();i++)
    {
        saturators.emplace_back(params.getParamsForBand(i),spec);
    }
}
