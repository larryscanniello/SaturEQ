/*
  ==============================================================================

    Filter.cpp
    Created: 28 May 2026 6:59:59pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "Filter.h"
#include <juce_dsp/juce_dsp.h>

Filter::Filter(Parameters::EQ::Band p,CoefficientStrategy strategy)
:  params(p), strategy(strategy), useParams(true)
{
    std::visit([&](auto& s) {
        a.resize(s.aSize);
        b.resize(s.bSize);
    },strategy);
    
    std::visit([&](auto& s) {
          s.updateCoefficients(spec.sampleRate, *p.fc, *p.Q, *p.gainInDB, a, b);
      }, strategy);
}

Filter::Filter(float fc,
       float Q,
       float gainInDB,
       CoefficientStrategy strategy)
: strategy(strategy), useParams(false)
{
    std::visit([&](auto& s) {
        a.resize(s.aSize);
        b.resize(s.bSize);
    },strategy);
    
    std::visit([&](auto& s) {
        s.updateCoefficients(spec.sampleRate,fc,Q,gainInDB,a,b);
    },strategy);
}

void Filter::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    this->spec = spec;
    juce::uint32 numChannels = spec.numChannels;
    x.resize(numChannels);
    y.resize(numChannels);
    for(auto i=0; i<numChannels; i++)
    {
        std::visit([&](auto& s){
            x[i].resize(s.aSize);
            y[i].resize(s.bSize);
        },strategy);
    }
    resetDelayLines();
}

void Filter::update()
{
    params.fcSmoother.setTargetValue(params.fc->get());
    params.QSmoother.setTargetValue(params.Q->get());
    params.gainInDBSmoother.setTargetValue(params.gainInDB->get());
    bypass = params.bypass->get();
}

void Filter::smoothen()
{
    if(params.fcSmoother.isSmoothing() || params.QSmoother.isSmoothing() || params.gainInDBSmoother.isSmoothing())
    {
        float fc = params.fcSmoother.getNextValue();
        float Q = params.QSmoother.getNextValue();
        float gainInDB = params.gainInDBSmoother.getNextValue();
        std::visit([&](auto& s) {
            s.updateCoefficients(spec.sampleRate,fc,Q,gainInDB,a,b);
        },strategy);
    }
}

void Filter::putSample(float sample,int channel){
      std::vector<float>& xChannel = x[channel];
      std::vector<float>& yChannel = y[channel];
      curr = (curr + 1) % xChannel.size();
      xChannel[curr] = sample;
      float processedSample = 0;
      for(int i=0; i<b.size();i++){
          processedSample += b[i] * xChannel[(curr-i+xChannel.size()) % xChannel.size()];
      }
      for(int i=1; i<a.size();i++){
          processedSample -= a[i] * yChannel[(curr-i+yChannel.size()) % yChannel.size()];
      }
      processedSample /= a[0];
      yChannel[curr] = processedSample;
  }


void Filter::processBlock(juce::dsp::AudioBlock<float> input, juce::dsp::AudioBlock<float> output)
{
    jassert(input.getNumChannels() == output.getNumChannels());
    jassert(input.getNumSamples() == output.getNumSamples());
    
    if(useParams) update();
    
    for(auto channel=0; channel<output.getNumChannels(); channel++)
    {
        
        auto inputData = input.getChannelPointer(channel);
        auto outputData = output.getChannelPointer(channel);
        for(auto sample=0; sample<input.getNumSamples();sample++)
        {
            if(useParams) smoothen();
            putSample(inputData[sample], channel);
            outputData[sample] = getSample(channel);
        }
    }
}

void Filter::processBlock(juce::dsp::AudioBlock<float> buf)
{
    if(useParams) update();
    
    for(auto channel=0; channel<buf.getNumChannels();channel++)
    {
        auto channelData = buf.getChannelPointer(channel);
        for(auto sample=0; sample<buf.getNumSamples();sample++)
        {
            if(useParams) smoothen();
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
