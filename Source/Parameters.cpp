//
//  Parameters.cpp
//  SaturEQ
//
//  Created by Larry Scanniello on 5/15/26.
//

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts,freqParamID,freqParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        freqParamID,
        "Frequency",
        juce::NormalisableRange<float>{20.0f,20000.0f},
        200.0f)
       );
    
    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    
}

void Parameters::reset() noexcept
{

}

void Parameters::update() noexcept
{
    freq = freqParam->get();
}


#include "Parameters.h"
