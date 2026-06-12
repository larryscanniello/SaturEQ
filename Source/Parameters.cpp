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

Parameters::EQ::EQ(juce::AudioProcessorValueTreeState &apvts)
{
    castParameter(apvts,ParamDeclarations::EQFreq0::Name,params[0].fc);
    castParameter(apvts,ParamDeclarations::EQFreq1::Name,params[1].fc);
    castParameter(apvts,ParamDeclarations::EQFreq0::Name,params[2].fc);
    castParameter(apvts,ParamDeclarations::EQFreq1::Name,params[3].fc);
    castParameter(apvts,ParamDeclarations::EQFreq0::Name,params[4].fc);
    castParameter(apvts,ParamDeclarations::EQFreq1::Name,params[5].fc);

    castParameter(apvts,ParamDeclarations::EQQ0::Name,params[0].Q);
    castParameter(apvts,ParamDeclarations::EQQ1::Name,params[1].Q);
    castParameter(apvts,ParamDeclarations::EQQ2::Name,params[2].Q);
    castParameter(apvts,ParamDeclarations::EQQ3::Name,params[3].Q);
    castParameter(apvts,ParamDeclarations::EQQ4::Name,params[4].Q);
    castParameter(apvts,ParamDeclarations::EQQ5::Name,params[5].Q);
    
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        castParameter(apvts,ParamDeclarations::EQGain::Name,params[i].gainInDB);
        castParameter(apvts,ParamDeclarations::EQBypass::Name,params[i].gainInDB);
    }
}

Parameters::Saturation::Saturation(juce::AudioProcessorValueTreeState &apvts)
{
    params.resize(ParamDeclarations::SATURATION_NUM_BANDS);
    for(auto i=0; i<ParamDeclarations::SATURATION_NUM_BANDS; i++)
    {
        castParameter(apvts, ParamDeclarations::SaturationPreGain::Name, params[i].preGain);
        castParameter(apvts, ParamDeclarations::SaturationBypass::Name, params[i].bypass);
    }
}

Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts) : eqParams(apvts), saturationParams(apvts) {}
    

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02f;
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        EQ::Band band = eqParams.getParamsForBand(i);
        band.fcSmoother->reset(sampleRate,duration);
        band.QSmoother->reset(sampleRate, duration);
        band.gainInDBSmoother->reset(sampleRate, duration);
    }
    for(auto i=0; i<ParamDeclarations::SATURATION_NUM_BANDS; i++)
    {
        Saturation::Band band = saturationParams.getParamsForBand(i);
        band.preGainSmoother->reset(sampleRate,duration);
    }
}


void Parameters::reset() noexcept
{
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        EQ::Band band = eqParams.getParamsForBand(i);
        band.fcSmoother->setCurrentAndTargetValue(band.fc->get());
        band.QSmoother->setCurrentAndTargetValue(band.Q->get());
        band.gainInDBSmoother->setCurrentAndTargetValue(band.gainInDB->get());
    }
    
    for(auto i=0; i<ParamDeclarations::SATURATION_NUM_BANDS;i++)
    {
        Saturation::Band band = saturationParams.getParamsForBand(i);
        band.preGainSmoother->setCurrentAndTargetValue(band.preGain->get());
    }
}

void Parameters::update() noexcept
{
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        EQ::Band band = eqParams.getParamsForBand(i);
        band.fcSmoother->setTargetValue(band.fc->get());
        band.QSmoother->setTargetValue(band.Q->get());
        band.gainInDBSmoother->setTargetValue(band.gainInDB->get());
    }
    
    for(auto i=0; i<ParamDeclarations::SATURATION_NUM_BANDS;i++)
    {
        Saturation::Band band = saturationParams.getParamsForBand(i);
        band.preGainSmoother->setTargetValue(band.preGain->get());
    }
 }
 


#include "Parameters.h"

