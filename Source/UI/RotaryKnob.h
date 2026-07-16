/*
  ==============================================================================

    RotaryKnob.h
    Created: 11 Jul 2026 9:44:37pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text,
               juce::AudioProcessorValueTreeState& apvts,
               const juce::ParameterID& parameterID,
               bool drawFromMiddle = false);

    ~RotaryKnob() override;

    void resized() override;

    juce::Slider slider;
    juce::Label label;

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};

