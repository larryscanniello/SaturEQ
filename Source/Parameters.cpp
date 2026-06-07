//
//  Parameters.cpp
//  SaturEQ
//
//  Created by Larry Scanniello on 5/15/26.
//

#include "Parameters.h"
#include "ParamDeclarations.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::String& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id));
    jassert(destination);
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    
    castParameter(apvts,ParamDeclarations::EQFreq0::Name,eqFreqParams[0]);
    castParameter(apvts,ParamDeclarations::EQFreq1::Name,eqFreqParams[1]);
    castParameter(apvts,ParamDeclarations::EQFreq2::Name,eqFreqParams[2]);
    castParameter(apvts,ParamDeclarations::EQFreq3::Name,eqFreqParams[3]);
    castParameter(apvts,ParamDeclarations::EQFreq4::Name,eqFreqParams[4]);
    castParameter(apvts,ParamDeclarations::EQFreq5::Name,eqFreqParams[5]);
    
    castParameter(apvts,ParamDeclarations::EQQ0::Name,eqQParams[0]);
    castParameter(apvts,ParamDeclarations::EQQ1::Name,eqQParams[1]);
    castParameter(apvts,ParamDeclarations::EQQ2::Name,eqQParams[2]);
    castParameter(apvts,ParamDeclarations::EQQ3::Name,eqQParams[3]);
    castParameter(apvts,ParamDeclarations::EQQ4::Name,eqQParams[4]);
    castParameter(apvts,ParamDeclarations::EQQ5::Name,eqQParams[5]);
    
    for(size_t i=0; i<ParamDeclarations::EQ_BAND_NUM; i++)
    {
        castParameter(apvts,ParamDeclarations::EQGain::Name,eqGainInDBParams[i]);
        castParameter(apvts,ParamDeclarations::EQBypass::Name,eqBypassParams[i]);
    }
    
    for(size_t i=0; i<ParamDeclarations::SATURATION_BAND_NUM; i++)
    {
        castParameter(apvts,ParamDeclarations::SaturationPreGain::Name,saturationPreGainParams[i]);
        castParameter(apvts,ParamDeclarations::SaturationBypass::Name,saturationBypassParams[i]);
    }
    setDefaults();
}
    

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02f;
    for(auto i=0; i<ParamDeclarations::EQ_BAND_NUM; i++)
    {
        eqFreqSmoothers[i].reset(sampleRate,duration);
        eqQSmoothers[i].reset(sampleRate,duration);
        eqFreqSmoothers[i].reset(sampleRate,duration);
        eqQSmoothers[i].reset(sampleRate,duration);
    }
    for(auto i=0; i<ParamDeclarations::SATURATION_BAND_NUM; i++)
    {
        saturationPreGainSmoothers[i].reset(sampleRate,duration);
    }
}

void Parameters::reset() noexcept
{
    eqFreqs = ParamDeclarations::EQ_DEFAULT_FREQS;
    eqQs = ParamDeclarations::EQ_DEFAULT_QS;
    eqGainsInDB = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    eqBypasses = {false,false,false,false,false,false};
    
    for(auto i=0; i<ParamDeclarations::EQ_BAND_NUM; i++)
    {
        eqFreqSmoothers[i].setCurrentAndTargetValue(eqFreqParams[i]->get());
        eqQSmoothers[i].setCurrentAndTargetValue(eqQParams[i]->get());
        eqGainInDBSmoothers[i].setCurrentAndTargetValue(juce::Decibels::decibelsToGain(eqGainsInDB[i]));
    }
    
    saturationPreGains = {0.0f,0.0f,0.0f,0.0f};
    saturationBypasses = {false,false,false,false};
    
    for(auto i=0; i<ParamDeclarations::SATURATION_BAND_NUM;i++)
    {
        saturationPreGainSmoothers[i].setCurrentAndTargetValue(saturationPreGainParams[i]->get());
    }

}

void Parameters::update() noexcept
{
    for(auto i=0; i<ParamDeclarations::EQ_BAND_NUM; i++)
    {
        eqFreqSmoothers[i].setTargetValue(eqFreqParams[i]->get());
        eqQSmoothers[i].setTargetValue(eqQParams[i]->get());
        eqGainInDBSmoothers[i].setTargetValue(juce::Decibels::decibelsToGain(eqGainInDBParams[i]->get()));
        eqBypasses[i] = eqBypassParams[i]->get();
    }
    
    for(auto i=0; i<ParamDeclarations::SATURATION_BAND_NUM; i++)
    {
        saturationPreGainSmoothers[i].setTargetValue(saturationPreGainParams[i]->get());
        saturationBypasses[i] = saturationBypassParams[i]->get();
    }
 }

void Parameters::smoothen() noexcept
{
    for(auto i=0; i<ParamDeclarations::EQ_BAND_NUM; i++)
    {
        eqFreqs[i] = eqFreqSmoothers[i].getNextValue();
        eqGainsInDB[i] = eqGainInDBSmoothers[i].getNextValue();
        eqQs[i] = eqQSmoothers[i].getNextValue();
    }
    
    for(auto i=0; i<ParamDeclarations::SATURATION_BAND_NUM; i++)
    {
        saturationPreGains[i] = saturationPreGainSmoothers[i].getNextValue();
    }
}


#include "Parameters.h"
