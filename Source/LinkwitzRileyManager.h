/*
  ==============================================================================

    FreqBandManager.h
    Created: 29 May 2026 4:19:11pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once
#include "LowPass.h"
#include "HighPass.h"
#include "Paramaters.h"
#include <vector>
#include <juce_dsp/juce_dsp.h>

class LinkwitzRileyManager {
    
public:
    LinkwitzRileyManager(Parameters params)
    {
        bands.resize(0,0,0);
        this.params = params;
    }
    
    void addSplit(float freq);
    
    void removeSplit(int splitNum);
    
    std::vector<juce::dsp::AudioBlock<float>>& splitSignal(juce::dsp::AudioBlock<float> &input);
    
    juce::dsp::AudioBlock<float>& sumSignal(juce::dsp::AudioBlock<float> &output);
    
    std::vector<float> getSplitFrequencies() {return frequencies;};
    
    int getSampleRate(){ return sampleRate;}
    
    void setSampleRate(int sr);
    
    void prepare(size_t channelSize, size_t numBands, size_t samplesPerBlock);
    
private:
    
    Params params;
    
    int sampleRate;
    
    int numChannels;
    
    std::vector<std::pair<LowPass,HighPass>> filters;
    
    template<typename type>
    struct Band{
        Band(size_t numChannels,size_t numSamples)
        {
            
            buffer.resize(numChannels,numSamples);
        }
        
        void prepare(int numChannels,int numSamples)
        {
            buffer.resize(numChannels,numSamples);
        }
        
        juce::dsp::AudioBlock<type> getSamples(int numSamples)
        {
            return juce::dsp::AudioBlock<type> (buffer).getSubBlock(0, numSamples);
        }
        
        juce::AudioBuffer<type> buffer;
        juce::dsp::AudioBlock<type> block;
    };
    
    struct Bands{
        std::vector<juce::dsp::AudioBlock<float>> blocks; //need this pre-allocated to return without allocating memory
        std::vector<std::unique_ptr<Band<float>>> ptrs;
        void resize(size_t numBands,size_t numChannels, size_t samplesPerBlock)
        {
            ptrs.resize(numBands,std::make_unique<Band<float>>(numChannels,samplesPerBlock));
            blocks.resize(numBands);
            for(auto i=0; i<numBands;i++)
            {
                blocks[i] = ptrs[i]->block;
            }
            
        }
    };
    
    Bands bands;

    std::vector<float> frequencies;
    
    void deriveFiltersFromFrequencies();
    
};
