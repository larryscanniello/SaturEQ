/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters/ParamDeclarations.h"
#include <vector>
#include "UI/RotaryKnob.h"
#include "UI/LookAndFeel.h"

//==============================================================================
/**
*/
class SaturEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SaturEQAudioProcessorEditor (SaturEQAudioProcessor&);
    ~SaturEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SaturEQAudioProcessor& audioProcessor;
    
    juce::OwnedArray<RotaryKnob> eqGains;
    juce::OwnedArray<RotaryKnob> eqQs;
    juce::OwnedArray<RotaryKnob> eqFreqs;
    juce::OwnedArray<juce::ImageButton> eqBypass;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBypassAttachments;
    juce::OwnedArray<juce::Label> eqBypassLabels;

    juce::OwnedArray<RotaryKnob> saturationPreGains;
    juce::OwnedArray<juce::ImageButton> saturationBypass;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> saturationBypassAttachments;
    juce::OwnedArray<juce::Label> saturationBypassLabels;
    
    juce::GroupComponent eqGroup;
    
    MainLookAndFeel mainLF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturEQAudioProcessorEditor)
};
