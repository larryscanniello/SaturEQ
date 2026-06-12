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

void EQManager::initializeFilterCoefficients(size_t sr, size_t numChannels, size_t numBands, Parameters::EQ eqparams)
{
    jassert(numBands >= 2);
        
    filters.clear();
    
    filters.reserve(numBands);
    
    HighPass hpStrategy;
    
    filters.emplace_back(numChannels,sr, eqparams.getFilterParams(0),hpStrategy);
    
    for(auto i=1; i<filters.size()-1; i++)
    {
        Peaking peakingStrategy;
        filters.emplace_back(numChannels,sr,eqparams.getFilterParams(i),peakingStrategy);
    }
    
    LowPass lpStrategy;
    
    filters.emplace_back(numChannels,sr,eqparams.getFilterParams(filters.size()-1),lpStrategy);
}
