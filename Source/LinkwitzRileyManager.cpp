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

void LinkwitzRileyManager::setSampleRate(int sr)
{
    int oldsr = sampleRate;
    float ratio = float(sr)/float(oldsr);
    for(int band=0;band<filters.size();band++){
        for(int stage=0; stage < 2; stage++){
            filters[band][stage].setSampleRate(sr);
        }
    }
}

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

void LinkwitzRileyManager::splitSignal(
                                              juce::AudioBuffer<float> &buf,
                                              std::vector<juce::AudioBuffer<float>> &output
                                       )
{
    jassert(filters.size()>=0);
    if(filters.size()<=0) return;
    
    filters[0][0].processBlock(buf, output[0]);
    
    long n = filters.size();
    
    for(int i=0; i<n-1;i++){
        filters[i][1].processBlock(buf, output[i+1]);
        filters[i+1][0].processBlock(output[i+1], output[i+1]);
    }
    
    filters[n-1][1].processBlock(buf, output[n-1]);
    
}

