/*
  ==============================================================================

    FreqBandManager.cpp
    Created: 29 May 2026 4:19:11pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "LinkwitzRileyManager.h"
#include "SecondOrderButterworth.h"
#include "Filter.h"
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
    for(int i=0; i< frequencies.size(); i++){
        filters[i][0] = SecondOrderButterworth(sampleRate, numChannels, frequencies[i], 0,0, true);
        filters[i][1] = SecondOrderButterworth(sampleRate, numChannels, frequencies[i], 0,0, false);
    }
}

void LinkwitzRileyManager::prepare(size_t channelSize,size_t numBands,size_t samplesPerBlock)
{
    bands.resize(numBands,channelSize,samplesPerBlock);
}

juce::dsp::AudioBlock<float>& LinkwitzRileyManager::sumSignal(juce::dsp::AudioBlock<float> &output)
{
    jassert(output.getNumSamples()==bands.ptrs[0]->buffer.getNumSamples());
    jassert(output.getNumChannels()==bands.ptrs[0]->buffer.getNumChannels());
    
    output.clear();
    
    for(auto band=1; band<bands.blocks.size(); band++)
    {
        bands.blocks[0].add(bands.blocks[band]);
    }
    
    return bands.blocks[0];
}


std::vector<juce::dsp::AudioBlock<float>>& LinkwitzRileyManager::splitSignal(juce::dsp::AudioBlock<float> &input)
{
    jassert(filters.size()>=0);
    
    if(filters.size()<=0)
    {
        juce::dsp::AudioBlock<float> internalBand(bands.ptrs[0]->buffer);
        internalBand.copyFrom(input);
        return bands.blocks;
    };
    
    filters[0][0].processBlock(input, bands.ptrs[0]->block);
    
    auto n = filters.size();
    
    for(int i=0; i<n-1;i++){
        filters[i][1].processBlock(input, bands.ptrs[i+1]->block);
        filters[i+1][0].processBlock(bands.ptrs[i+1]->block, bands.ptrs[i+1]->block);
    }
    
    filters[n-1][1].processBlock(input, bands.ptrs[n-1]->block);
    
    return bands.blocks;
}

