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
#include "Parameters.h"
#include <vector>
#include <juce_dsp/juce_dsp.h>
#include "Filter.h"

class LinkwitzRileyManager {
    
public:
    LinkwitzRileyManager(Parameters::Saturation& params)
    : params(params)
    {
        for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS + 1; i++)
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
    
    const std::vector<juce::dsp::AudioBlock<float>> splitSignal(juce::dsp::AudioBlock<float> &input);
    
    juce::dsp::AudioBlock<float> sumSignal(juce::dsp::AudioBlock<float> &output);
    
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    
    void deriveFiltersFromFrequencies();
    
private:
    
    Parameters::Saturation& params;
    
    std::vector<std::pair<Filter,Filter>> filters;
    
    /*
        This struct holds the buffers for the split audio in memory
        One Band represents a buffer for a saturation band
     */
    struct Band{
        Band(size_t numChannels,size_t numSamples)
        {
            buffer.setSize((int) numChannels,(int) numSamples);
        }
        
        void prepare(size_t numChannels,size_t numSamples)
        {
            buffer.setSize((int) numChannels,(int) numSamples);
        }
        
        juce::dsp::AudioBlock<float> getSamples(int numSamples)
        {
            return juce::dsp::AudioBlock<float> (buffer).getSubBlock(0, numSamples);
        }
    
        juce::AudioBuffer<float> buffer;
        juce::dsp::AudioBlock<float> block;
    };
    
    /*
        An instance of this struct manages the collection of all of the bands
        It has a vector of AudioBlocks as well as a vector of unique pointers, each with elements corresponding to a Band instance
        The split method in this class needs to return a vector of AudioBlocks, which need to be preallocated
    */
    struct Bands{
        Bands() = default;
        
        Bands(size_t numBands,juce::dsp::ProcessSpec spec)
        {
            for(auto i=0; i<numBands; i++)
            {
                ptrs.emplace_back(std::make_unique<Band>(spec.numChannels,spec.maximumBlockSize));
            }
        }
        void resize(size_t numBands,juce::dsp::ProcessSpec spec)
        {
            ptrs.clear();
            for (auto i = 0; i<numBands; i++)
                  ptrs.emplace_back(std::make_unique<Band>(spec.numChannels,spec.maximumBlockSize));
            blocks.clear();
            for(auto i=0; i<numBands;i++)
            {
                juce::dsp::AudioBlock<float> block{ptrs[i]->buffer};
                blocks.emplace_back(block);
            }
            
        }
        
        juce::dsp::AudioBlock<float> getBlock(size_t blockNum)
        {
            return blocks[blockNum];
        }
        size_t size()
        {
            return blocks.size();
        }
        const std::vector<juce::dsp::AudioBlock<float>> getBlocks()
        {
            return blocks;
        }
        const std::vector<std::unique_ptr<Band>>& getPointers()
        {
            return ptrs;
        }
    private:
        std::vector<juce::dsp::AudioBlock<float>> blocks; //need this pre-allocated to return without allocating memory
        std::vector<std::unique_ptr<Band>> ptrs;
    };
    
    Bands bands;
    
    
    
};
