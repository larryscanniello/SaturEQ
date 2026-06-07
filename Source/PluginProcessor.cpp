/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Filter.h"
#include "LinkwitzRileyManager.h"
#include "Saturator.h"
#include "ParamDeclarations.h"
#include "Peaking.h"
#include "Parameters.h"
#include <cmath>

//==============================================================================
SaturEQAudioProcessor::SaturEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        params(apvts)
#endif
{
    lrManager = std::make_unique<LinkwitzRileyManager>();
    filters.resize(Params::EQ_BAND_NUM);
    
    filters[0] = std::make_unique<HighPass>();
    filters[1] = std::make_unique<Peaking>();
    filters[2] = std::make_unique<Peaking>();
    filters[3] = std::make_unique<Peaking>();
    filters[4] = std::make_unique<Peaking>();
    filters[5] = std::make_unique<LowPass>();
}

SaturEQAudioProcessor::~SaturEQAudioProcessor()
{
}

//==============================================================================
const juce::String SaturEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SaturEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SaturEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SaturEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SaturEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SaturEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SaturEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SaturEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SaturEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void SaturEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SaturEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    params.prepareToPlay(sampleRate);
    params.reset();
    
    oversampler.initProcessing(samplesPerBlock);
    
    upsampled.clear();
}

void SaturEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaturEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SaturEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    params.update();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    upsampled = oversampler.processSamplesUp(buffer);
    
    bandblocks = lrManager->splitSignal(upsampled);
    
    saturator.processBands(bandblocks);
    
    juce::dsp::AudioBlock<float> summed = lrManager -> sumSignal(upsampled);
    
    for(auto &filter : filters)
    {
        filter.processBlock(summed);
    }
    
    juce::dsp::AudioBlock<float> bufferblock = juce::dsp::AudioBlock<float>(buffer);
    
    oversampler.processSamplesDown(bufferblock);
    
}

//==============================================================================
bool SaturEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SaturEQAudioProcessor::createEditor()
{
    return new SaturEQAudioProcessorEditor (*this);
}

//==============================================================================
void SaturEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SaturEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaturEQAudioProcessor();
}
