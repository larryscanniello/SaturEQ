//
//  Parameters.h
//  SaturEQ
//
//  Created by Larry Scanniello on 5/15/26.
//

#pragma once

#include <JuceHeader.h>

const juce::ParameterID freqParamID {"freq", 1};

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;
    
    float freq = 200.0f;
private:
    juce::AudioParameterFloat* freqParam;
    juce::LinearSmoothedValue<float> freqSmoother;
};
