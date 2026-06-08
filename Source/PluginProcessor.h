/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "Filter.h"
#include "LinkwitzRileyManager.h"
#include "Saturator.h"
#include <vector>

//==============================================================================
/**
*/

const int BANDS = 4;
const int HIGH_SR_THRESHOLD = 176400;

class SaturEQAudioProcessor  : public juce::AudioProcessor
{
    public:
    //==============================================================================
    SaturEQAudioProcessor();
    ~SaturEQAudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts {
        *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };
    
    Parameters params;
    
    size_t resampleFrequency = 196000.0f;
    
    juce::dsp::ProcessSpec specs;
    
    std::unique_ptr<LinkwitzRileyManager> lrManager = std::make_unique<LinkwitzRileyManager();
    
    std::unique_ptr<EQManager> eqManager = std::make_unique
    
    juce::dsp::Oversampling<float> oversampler{ (size_t) getTotalNumOutputChannels(), 1, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple, false, true};
    
    Saturator saturator;
    
    juce::dsp::AudioBlock<float> upsampled;
    
    std::vector<juce::dsp::AudioBlock<float>> bandblocks;
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturEQAudioProcessor)
};
