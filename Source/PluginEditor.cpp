/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters/ParamDeclarations.h"

//==============================================================================

constexpr size_t SCREEN_WIDTH =  800; //og 400
constexpr size_t SCREEN_HEIGHT = 600; //og 300;

SaturEQAudioProcessorEditor::SaturEQAudioProcessorEditor (SaturEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    eqFreqs.add(std::make_unique<RotaryKnob>("Frequency",audioProcessor.apvts,ParamDeclarations::EQFreq0::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Frequency",audioProcessor.apvts,ParamDeclarations::EQFreq1::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Frequency",audioProcessor.apvts,ParamDeclarations::EQFreq2::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Frequency",audioProcessor.apvts,ParamDeclarations::EQFreq3::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Frequency",audioProcessor.apvts,ParamDeclarations::EQFreq4::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Frequency",audioProcessor.apvts,ParamDeclarations::EQFreq5::ID));

    eqQs.add(std::make_unique<RotaryKnob>("Q",audioProcessor.apvts,ParamDeclarations::EQQ0::ID));
    eqQs.add(std::make_unique<RotaryKnob>("Q",audioProcessor.apvts,ParamDeclarations::EQQ1::ID));
    eqQs.add(std::make_unique<RotaryKnob>("Q",audioProcessor.apvts,ParamDeclarations::EQQ2::ID));
    eqQs.add(std::make_unique<RotaryKnob>("Q",audioProcessor.apvts,ParamDeclarations::EQQ3::ID));
    eqQs.add(std::make_unique<RotaryKnob>("Q",audioProcessor.apvts,ParamDeclarations::EQQ4::ID));
    eqQs.add(std::make_unique<RotaryKnob>("Q",audioProcessor.apvts,ParamDeclarations::EQQ5::ID));

    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        std::string suffix = std::to_string(i);
        eqGains.add(std::make_unique<RotaryKnob>("Gain",audioProcessor.apvts,std::string(ParamDeclarations::EQGain::ID)+suffix));
        auto* eqBypassButton = eqBypass.add(std::make_unique<juce::ImageButton>("Bypass"));
        eqBypassAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            audioProcessor.apvts, std::string(ParamDeclarations::EQBypass::ID)+suffix, *eqBypassButton));
    }

    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS+1; i++)
    {
        std::string suffix = std::to_string(i);
        saturationPreGains.add(std::make_unique<RotaryKnob>("PreGain",audioProcessor.apvts,std::string(ParamDeclarations::SaturationPreGain::ID) + suffix));
        auto* saturationBypassButton = saturationBypass.add(std::make_unique<juce::ImageButton>("Bypass"));
        saturationBypassAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            audioProcessor.apvts, std::string(ParamDeclarations::SaturationBypass::ID) + suffix, *saturationBypassButton));
    }
    
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        eqGroup.addAndMakeVisible(eqFreqs[i]);
        eqGroup.addAndMakeVisible(eqQs[i]);
        eqGroup.addAndMakeVisible(eqGains[i]);
    }
    addAndMakeVisible(eqGroup);
    
    setSize (SCREEN_WIDTH, SCREEN_HEIGHT);
    
    setLookAndFeel(&mainLF);
}

SaturEQAudioProcessorEditor::~SaturEQAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SaturEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);
}

void SaturEQAudioProcessorEditor::resized()
{
    eqGroup.setBounds(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    const size_t factor = SCREEN_WIDTH/(ParamDeclarations::EQ_NUM_BANDS+1);

    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        
        eqFreqs[i]->setCentrePosition(SCREEN_WIDTH - factor*(i+1),SCREEN_HEIGHT/4);
        eqQs[i]->setCentrePosition(SCREEN_WIDTH - factor*(i+1), SCREEN_HEIGHT/2);
        eqGains[i]->setCentrePosition(SCREEN_WIDTH - factor*(i+1), 3*SCREEN_HEIGHT/4);
    }
    
}
