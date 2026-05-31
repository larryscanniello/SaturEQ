/*
  ==============================================================================

    Filter.cpp
    Created: 28 May 2026 6:59:59pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "Filter.h"

void Filter::putSample(float sample,int channel){
    std::vector<float>& xChannel = x[channel];
    std::vector<float>& yChannel = y[channel];
    curr = (curr + 1) % x.size();
    xChannel[curr] = sample;
    float processedSample = 0;
    unsigned long xSize = xChannel.size();
    for(int i=0; i<xSize;i++){
        processedSample -= a[i] * xChannel[(curr-i+xSize) % xSize];
    }
    unsigned long ySize = yChannel.size();
    for(int i=0; i<ySize;i++){
        processedSample += b[i] * yChannel[(curr-i+ySize) % ySize];
    }
    yChannel[curr] = sample;
}


void Filter::processBlock(juce::AudioBuffer<float> &input, juce:: AudioBuffer<float> &output)
{
    for(int channel = 0; channel<input.getNumChannels();channel++){
        auto channelData = input.getReadPointer(channel);
        auto writePtr = output.getWritePointer(channel);
        
        for(int sample=0; sample<input.getNumSamples();sample++){
            putSample(channelData[sample],channel);
            writePtr[sample] = getSample(channel);
        }
    }
}

void Filter::resetDelayLines(){
    for(auto &vec : x){
        for(int i=0; i<vec.size(); i++){
            vec[i] = 0;
        }
    }
    for(auto &vec : y){
        for(int i=0; i<vec.size(); i++){
            vec[i] = 0;
        }
    }
}
