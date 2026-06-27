/*
  ==============================================================================

    FreqBandManager.cpp
    Created: 29 May 2026 4:19:11pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "LinkwitzRileyManager.h"
#include "Filter.h"
#include "HighPass.h"
#include "LowPass.h"
#include <vector>

const int NUM_STAGES = 2;

void LinkwitzRileyManager::addSplit(float freq)
{
    //Inserts into vector and keeps freqs sorted
    frequencies.insert(upper_bound(frequencies.begin(), frequencies.end(), freq), freq);
    deriveFiltersFromFrequencies();
}

void LinkwitzRileyManager::removeSplit(int splitNum){
    
    if(frequencies.size()<=0) return;
    frequencies.erase(frequencies.begin() + splitNum);
    deriveFiltersFromFrequencies();
}

void LinkwitzRileyManager::deriveFiltersFromFrequencies()
{
    filters.clear();
    
    for(int i=0; i<frequencies.size(); i++){
        LowPass lpStrategy;
        HighPass hpStrategy;
        float fc = *params.getParamsForBand(i).freqBandStart;
        Filter lp{fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f,lpStrategy};
        Filter hp{fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f,hpStrategy};
        std::pair pair{lp,hp};
        filters.emplace_back(pair);
    }
}

void LinkwitzRileyManager::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    bands.resize(params.size(),spec);
}

juce::dsp::AudioBlock<float> LinkwitzRileyManager::sumSignal(juce::dsp::AudioBlock<float> &output)
{
    
    output.clear();
    
    for(auto i=1; i<bands.size(); i++)
    {
        juce::dsp::AudioBlock<float> block = bands.getBlock(i);
        bands.getBlock(0).add(block);
    }
    
    return bands.getBlock(0);
}


const std::vector<juce::dsp::AudioBlock<float>> LinkwitzRileyManager::splitSignal(juce::dsp::AudioBlock<float> &input)
{
    
    if(filters.size()==0)
    {
        juce::dsp::AudioBlock<float> internalBand{bands.getBlock(0)};
        internalBand.copyFrom(input);
        return bands.getBlocks();
    };

    filters[0].first.processBlock(input, bands.getPointers()[0]->block);
    
    auto n = filters.size();
    
    for(int i=0; i<n-1;i++){
        filters[i].second.processBlock(input, bands.getPointers()[i+1]->block);
        filters[i+1].first.processBlock(bands.getPointers()[i+1]->block, bands.getPointers()[i+1]->block);
    }
    
    filters[n-1].second.processBlock(input, bands.getPointers()[n-1]->block);
    
    return bands.getBlocks();
}
