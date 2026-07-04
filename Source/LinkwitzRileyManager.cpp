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


LinkwitzRileyManager::LinkwitzRileyManager(Parameters::Saturation& params)
: params(params)
{
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS; i++)
    {
        //Initialize with placeholder values, will be updated on prepareToPlay
        HighPass hp;
        LowPass lp;
        Filter hpf{0,0,0,hp};
        Filter lpf{0,0,0,lp};
        std::pair<Filter,Filter> pair{hpf,lpf};
        filters.emplace_back(pair);
    }
    deriveFiltersFromFrequencies();
}


void LinkwitzRileyManager::deriveFiltersFromFrequencies()
{
    for(int i=0; i<params.getNumSplits(); i++){
        float fc = params.getFreqToSplitAt(i);
        std::pair<Filter,Filter> pair = filters[i];
        pair.first.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
        pair.second.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
        filters[i] = pair;
    }
}

void LinkwitzRileyManager::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    deriveFiltersFromFrequencies();
    bands.prepareToPlay(spec);
    for(auto& pair : filters)
    {
        pair.first.prepareToPlay(spec);
        pair.second.prepareToPlay(spec);
    }
}



juce::dsp::AudioBlock<float> LinkwitzRileyManager::sumSignal()
{
    for(auto i=1; i<bands.size(); i++)
    {
        juce::dsp::AudioBlock<float> block = bands.getBlock(i);
        bands.getBlock(0).add(block);
    }
    
    return bands.getBlock(0);
}


const std::vector<juce::dsp::AudioBlock<float>> LinkwitzRileyManager::splitSignal(juce::dsp::AudioBlock<float> &input)
{
    if(params.getNumSplits()==0)
    {
        juce::dsp::AudioBlock<float> internalBand{bands.getBlock(0)};
        internalBand.copyFrom(input);
        return bands.getBlocks();
    };

    filters[0].first.processBlock(input, bands.getBlock(0));
    
    auto n = params.getNumSplits();
    
    for(int i=0; i<n-1;i++){
        filters[i].second.processBlock(input, bands.getBlock(i+1));
        filters[i+1].first.processBlock(bands.getBlock(i+1), bands.getBlock(i+1));
    }
    
    filters[n-1].second.processBlock(input, bands.getBlock(n-1));
    
    return bands.getBlocks();
}
