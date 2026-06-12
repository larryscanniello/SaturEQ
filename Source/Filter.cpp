/*
  ==============================================================================

    Filter.cpp
    Created: 28 May 2026 6:59:59pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "Filter.h"
#include <juce_dsp/juce_dsp.h>

void Filter::update()
{
    params.fcSmoother->setTargetValue(params.fc->get());
    params.QSmoother->setTargetValue(params.Q->get());
    params.gainInDBSmoother->setTargetValue(params.gainInDB->get());
    bypass = params.bypass->get();
}

void Filter::smoothen()
{
    if(params.fcSmoother->isSmoothing() || params.QSmoother->isSmoothing() || params.gainInDBSmoother->isSmoothing())
    {
        float fc = params.fcSmoother->getNextValue();
        float Q = params.QSmoother->getNextValue();
        float gainInDB = params.gainInDBSmoother->getNextValue();
        strategy.updateCoefficients(sampleRate,fc,Q,gainInDB,a,b);
    }
}

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

void Filter::processBlock(juce::dsp::AudioBlock<float> &buf)
{
    for(auto channel=0; channel<buf.getNumChannels();channel++)
    {
        auto channelData = buf.getChannelPointer(channel);
        for(auto sample=0; sample<buf.getNumSamples();sample++)
        {
            smoothen();
            putSample(channelData[sample], channel);
            channelData[sample] = getSample(channel);
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
