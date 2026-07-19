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
constexpr size_t ROW_GAP = 12;

SaturEQAudioProcessorEditor::SaturEQAudioProcessorEditor (SaturEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    eqFreqs.add(std::make_unique<RotaryKnob>("Freq",audioProcessor.apvts,ParamDeclarations::EQFreq0::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Freq",audioProcessor.apvts,ParamDeclarations::EQFreq1::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Freq",audioProcessor.apvts,ParamDeclarations::EQFreq2::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Freq",audioProcessor.apvts,ParamDeclarations::EQFreq3::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Freq",audioProcessor.apvts,ParamDeclarations::EQFreq4::ID));
    eqFreqs.add(std::make_unique<RotaryKnob>("Freq",audioProcessor.apvts,ParamDeclarations::EQFreq5::ID));

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
        auto* eqBypassButton = eqBypasses.add(std::make_unique<juce::ImageButton>("Bypass"));
        eqBypassAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            audioProcessor.apvts, std::string(ParamDeclarations::EQBypass::ID)+suffix, *eqBypassButton));
    }

    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS+1; i++)
    {
        std::string suffix = std::to_string(i);
        saturationPreGains.add(std::make_unique<RotaryKnob>("PreGain",audioProcessor.apvts,std::string(ParamDeclarations::SaturationPreGain::ID) + suffix));
        if(i<ParamDeclarations::SATURATION_MAX_SPLITS)
           startFrequencies.add(std::make_unique<RotaryKnob>("Split Freq",audioProcessor.apvts,std::string(ParamDeclarations::SaturationSplitFreq::ID)+suffix));
        auto* saturationBypassButton = saturationBypasses.add(std::make_unique<juce::ImageButton>("Bypass"));
        saturationBypassAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            audioProcessor.apvts, std::string(ParamDeclarations::SaturationBypass::ID) + suffix, *saturationBypassButton));
    }
    
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS+1; i++)
    {
        saturationBands.add(std::make_unique<juce::Component>());
        saturationBands[i]->addAndMakeVisible(saturationPreGains[i]);
        if(i>0) saturationBands[i]->addAndMakeVisible(startFrequencies[i-1]);
        saturationGroup.addAndMakeVisible(saturationBands[i]);
    }
    
    saturationGroup.setText("Saturation");
    saturationGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(saturationGroup);
    
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        eqBands.add(std::make_unique<juce::Component>());
        eqBands[i]->addAndMakeVisible(eqFreqs[i]);
        eqBands[i]->addAndMakeVisible(eqQs[i]);
        if(i>0 && i<ParamDeclarations::EQ_NUM_BANDS-1)
            eqBands[i]->addAndMakeVisible(eqGains[i]);
        eqGroup.addAndMakeVisible(eqBands[i]);
    }
    eqGroup.setText("EQ");
    eqGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
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
    const int H    = eqFreqs[0]->getHeight();
    const int PAD  = (int(SCREEN_HEIGHT) - 5*H - 3*int(ROW_GAP)) / 4;
    const int eqH  = 2*PAD + 3*H + 2*int(ROW_GAP);
    const int satH = 2*PAD + 2*H + 1*int(ROW_GAP);

    eqGroup.setBounds(0,0,SCREEN_WIDTH,eqH);
    const size_t factor = SCREEN_WIDTH/ParamDeclarations::EQ_NUM_BANDS;

    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        eqBands[i]->setBounds(factor*i, 0, factor, eqH);
        int groupWidth = eqBands[i]->getWidth();

        auto area = eqBands[i]->getLocalBounds();
        area.removeFromTop(PAD);
        auto r0 = area.removeFromTop(eqFreqs[i]->getHeight());
        eqFreqs[i]->setCentrePosition(groupWidth/2, r0.getCentreY());
        area.removeFromTop(ROW_GAP);
        auto r1 = area.removeFromTop(eqQs[i]->getHeight());
        eqQs[i]->setCentrePosition(groupWidth/2, r1.getCentreY());
        area.removeFromTop(ROW_GAP);
        auto r2 = area.removeFromTop(eqGains[i]->getHeight());
        eqGains[i]->setCentrePosition(groupWidth/2, r2.getCentreY());
    }

    saturationGroup.setBounds(0,eqH,SCREEN_WIDTH,satH);
    const size_t satFactor = SCREEN_WIDTH/(ParamDeclarations::SATURATION_MAX_SPLITS+1);

    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS+1; i++)
    {
        saturationBands[i]->setBounds(satFactor*i,0, satFactor, satH);
        int groupWidth = saturationBands[i]->getWidth();

        auto area = saturationBands[i]->getLocalBounds();
        area.removeFromTop(PAD);
        auto r0 = area.removeFromTop(saturationPreGains[i]->getHeight()); // start-freq row
        if(i>0) startFrequencies[i-1]->setCentrePosition(groupWidth/2, r0.getCentreY());
        area.removeFromTop(ROW_GAP);
        auto r1 = area.removeFromTop(saturationPreGains[i]->getHeight());
        saturationPreGains[i]->setCentrePosition(groupWidth/2, r1.getCentreY());
    }
    
    
}
