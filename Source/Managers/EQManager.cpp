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

void EQManager::update()
{
    for(Filter& filter: filters)
    {
        filter.update();
    }
}

void EQManager::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    updateFilters(eqParams.size(),spec);
    for(auto& filter : filters)
    {
        filter.prepareToPlay(spec);
    }
}

void EQManager::updateFilters(size_t numBands, juce::dsp::ProcessSpec spec)
{
    jassert(numBands >= 2);
    filters.clear();
    HighPass hpStrategy;
    filters.emplace_back(eqParams.getParamsForBand(0),hpStrategy);
    for(auto i=1; i<numBands-1; i++)
    {
        Peaking peakingStrategy;
        filters.emplace_back(eqParams.getParamsForBand(i),peakingStrategy);
    }
    LowPass lpStrategy;
    filters.emplace_back(eqParams.getParamsForBand(numBands-1),lpStrategy);
}
