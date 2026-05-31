/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Filter.h"
#include "LinkwitzRiley.h"
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
    
    int upsampleMultiplier = std::ceil(HIGH_SR_THRESHOLD / sampleRate);
    
    highSRBuffer.setSize(getTotalNumInputChannels(),samplesPerBlock * upsampleMultiplier);
    
    
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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    resampler.resample(buffer,highSRBuffer);
    
    lrManager.splitSignal(highSRBuffer,bandsHighSR);

    for(int channel=0; channel< totalNumInputChannels; channel++){
        auto* channelData = buffer.getWritePointer(channel);
        
        highSRBuffer[channel].clear();
        
        for(int band=0;band<BANDS;band++){
                        
            saturator.processBlock(bandsHighSR[channel][band]);
            
            filters[band].processBlock(bandsHighSR[channel][band]);
            
            for(int sample=0; sample<highSRBuffer[channel].size();sample++){
                highSRBuffer[channel][sample] += bandsHighSR[channel][band][sample];
            }
        }
    }
    
    for(int channel=0; channel<totalNumInputChannels; channel++){
        resampler.resample(highSRBuffer,buffer.getWritePointer(channel));
    }
    
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
