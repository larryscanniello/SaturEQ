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
        LowPass lp;
        HighPass hp;
        Filter lpf{0,0,0,lp};
        Filter hpf{0,0,0,hp};
        std::pair<Filter,Filter> pair{lpf,hpf};
        filters.emplace_back(pair);
    }
    deriveFiltersFromFrequencies();
}


void LinkwitzRileyManager::deriveFiltersFromFrequencies()
{
    for(int i=0; i<params.getNumSplits(); i++){
        float fc = params.getFreqToSplitAt(i);
        std::pair<Filter,Filter>& pair = filters[i];
        pair.first.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
        pair.second.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
    }
}

void LinkwitzRileyManager::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    for(auto& pair : filters)
    {
        pair.first.prepareToPlay(spec);
        pair.second.prepareToPlay(spec);
    }
    deriveFiltersFromFrequencies();
    bands.prepareToPlay(spec);
}



void LinkwitzRileyManager::sumSignal(juce::dsp::AudioBlock<float>& output)
{
    for(auto i=1; i<params.getNumSplits()+1; i++)
    {
        juce::dsp::AudioBlock<float> block = bands.getBlock(i);
        bands.getBlock(0).add(block);
    }
    
    output.copyFrom(bands.getBlock(0).getSubBlock(0,output.getNumSamples()));
}


const std::vector<juce::dsp::AudioBlock<float>>& LinkwitzRileyManager::splitSignal(juce::dsp::AudioBlock<float> &input)
{
    auto n = params.getNumSplits();
    
    if(n==0)
    {
        juce::dsp::AudioBlock<float> internalBand{bands.getBlock(0).getSubBlock(0,input.getNumSamples())};
        internalBand.copyFrom(input);
        return bands.getBlocks();
    };

    filters[0].first.processBlock(input, bands.getBlock(0).getSubBlock(0, input.getNumSamples()));
    
    for(int i=0; i<n-1;i++){
        juce::dsp::AudioBlock<float> block{bands.getBlock(i+1).getSubBlock(0, input.getNumSamples())};
        filters[i].second.processBlock(input, block);
        filters[i+1].first.processBlock(block);
    }
    
    filters[n-1].second.processBlock(input, bands.getBlock(n).getSubBlock(0, input.getNumSamples()));
    
    return bands.getBlocks();
}
