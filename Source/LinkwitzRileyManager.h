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
    LinkwitzRileyManager(Parameters::Saturation& params);
    
    const std::vector<juce::dsp::AudioBlock<float>> splitSignal(juce::dsp::AudioBlock<float> &input);
    
    juce::dsp::AudioBlock<float> sumSignal();
    
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
        void prepareToPlay(juce::dsp::ProcessSpec spec)
        {
            buffer.setSize(spec.numChannels,spec.maximumBlockSize);
        }
        juce::dsp::AudioBlock<float> getBlock()
        {
            juce::dsp::AudioBlock<float> block{buffer};
            return block;
        }
    
        juce::AudioBuffer<float> buffer;
    };
    
    /*
        An instance of this struct manages the collection of all of the bands
        It has a vector of AudioBlocks as well as a vector of unique pointers, each with elements corresponding to a Band instance
        The split method in this class needs to return a vector of AudioBlocks, which need to be preallocated
    */
    struct Bands{
        Bands()
        {
            for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS+1; i++)
            {
                ptrs.emplace_back(std::make_unique<Band>());
            }
        }
        juce::dsp::AudioBlock<float>& getBlock(size_t blockNum)
        {
            return blocks[blockNum];
        }
        size_t size()
        {
            return blocks.size();
        }
        void prepareToPlay(juce::dsp::ProcessSpec spec)
          {
              for (auto& ptr : ptrs)
                  ptr->prepareToPlay(spec);
              blocks.clear();
              for (auto& ptr : ptrs)
                  blocks.emplace_back(juce::dsp::AudioBlock<float>{ptr->buffer});
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
