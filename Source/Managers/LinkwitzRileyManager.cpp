/*
  ==============================================================================

    FreqBandManager.cpp
    Created: 29 May 2026 4:19:11pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "LinkwitzRileyManager.h"
#include "../DSP/Filters/Filter.h"
#include "../DSP/Filters/HighPass.h"
#include "../DSP/Filters/LowPass.h"
#include <vector>


LinkwitzRileyManager::LinkwitzRileyManager(Parameters::Saturation& params)
: params(params)
{
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS; i++)
    {
        //Initialize with placeholder values, will be updated on prepareToPlay
        LowPass lp;
        HighPass hp;
        Filter lpf{800,1,0,lp};
        Filter hpf{800,1,0,hp};
        std::pair<Filter,Filter> pair1{lpf,lpf};
        std::pair<Filter,Filter> pair2{hpf,hpf};
        std::pair<std::pair<Filter,Filter>,std::pair<Filter,Filter>> pairOfPairs{pair1,pair2};
        filters.emplace_back(pairOfPairs);
    }
    deriveFiltersFromFrequencies();
}


void LinkwitzRileyManager::deriveFiltersFromFrequencies()
{
    for(int i=0; i<params.getNumSplits(); i++){
        float fc = params.getFreqToSplitAt(i);
        std::pair<std::pair<Filter,Filter>,std::pair<Filter,Filter>>& highAndLowPass = filters[i];
        highAndLowPass.first.first.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
        highAndLowPass.first.second.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
        highAndLowPass.second.first.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
        highAndLowPass.second.second.updateCoefficients(fc,juce::MathConstants<float>::sqrt2 * 0.5f,0.0f);
    }
}

void LinkwitzRileyManager::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    for(auto& pair : filters)
    {
        pair.first.first.prepareToPlay(spec);
        pair.first.second.prepareToPlay(spec);
        pair.second.first.prepareToPlay(spec);
        pair.second.second.prepareToPlay(spec);
    }
    deriveFiltersFromFrequencies();
    bands.prepareToPlay(spec);
}



void LinkwitzRileyManager::sumSignals(juce::dsp::AudioBlock<float>& output)
{
    for(auto i=1; i<params.getNumSplits()+1; i++)
    {
        juce::dsp::AudioBlock<float> block = bands.getBlock(i);
        bands.getBlock(0).add(block);
    }
    
    jassert(bands.getBlock(0).getNumSamples() == output.getNumSamples());
    
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

    filters[0].first.first.processBlock(input, bands.getBlock(0).getSubBlock(0, input.getNumSamples()));
    filters[0].first.second.processBlock(input, bands.getBlock(0).getSubBlock(0, input.getNumSamples()));
    
    for(int i=0; i<n-1;i++){
        juce::dsp::AudioBlock<float> block{bands.getBlock(i+1).getSubBlock(0, input.getNumSamples())};
        filters[i].second.first.processBlock(input, block);
        filters[i].second.second.processBlock(input, block);
        filters[i+1].first.first.processBlock(block);
        filters[i+1].first.second.processBlock(block);
    }
    
    jassert(bands.getBlock(n).getNumSamples() == input.getNumSamples());
    
    filters[n-1].second.first.processBlock(input, bands.getBlock(n).getSubBlock(0, input.getNumSamples()));
    filters[n-1].second.second.processBlock(input, bands.getBlock(n).getSubBlock(0, input.getNumSamples()));
    
    return bands.getBlocks();
}
