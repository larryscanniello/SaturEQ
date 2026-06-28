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

Parameters::EQ::EQ(juce::AudioProcessorValueTreeState &apvts) : apvts(apvts)
{
    params.resize(ParamDeclarations::EQ_NUM_BANDS);
    
    castParameter(apvts,ParamDeclarations::EQFreq0::ID,params[0].fc);
    castParameter(apvts,ParamDeclarations::EQFreq1::ID,params[1].fc);
    castParameter(apvts,ParamDeclarations::EQFreq2::ID,params[2].fc);
    castParameter(apvts,ParamDeclarations::EQFreq3::ID,params[3].fc);
    castParameter(apvts,ParamDeclarations::EQFreq4::ID,params[4].fc);
    castParameter(apvts,ParamDeclarations::EQFreq5::ID,params[5].fc);

    castParameter(apvts,ParamDeclarations::EQQ0::ID,params[0].Q);
    castParameter(apvts,ParamDeclarations::EQQ1::ID,params[1].Q);
    castParameter(apvts,ParamDeclarations::EQQ2::ID,params[2].Q);
    castParameter(apvts,ParamDeclarations::EQQ3::ID,params[3].Q);
    castParameter(apvts,ParamDeclarations::EQQ4::ID,params[4].Q);
    castParameter(apvts,ParamDeclarations::EQQ5::ID,params[5].Q);

    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        const auto suffix = std::to_string(i);
        castParameter(apvts,ParamDeclarations::EQGain::ID   + suffix,params[i].gainInDB);
        castParameter(apvts,ParamDeclarations::EQBypass::ID + suffix,params[i].bypass);
    }
}

Parameters::Saturation::Saturation(juce::AudioProcessorValueTreeState &apvts) : apvts(apvts)
{
    params.resize(ParamDeclarations::SATURATION_MAX_SPLITS + 1);
    
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS + 1; i++)
    {
        const auto suffix = std::to_string(i);
        castParameter(apvts, ParamDeclarations::SaturationPreGain::ID + suffix, params[i].preGain);
        castParameter(apvts, ParamDeclarations::SaturationBypass::ID  + suffix, params[i].bypass);
    }
    
    freqsToSplitAt.resize(ParamDeclarations::SATURATION_MAX_SPLITS);
    
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS; i++)
    {
        const auto suffix = std::to_string(i);
        castParameter(apvts, ParamDeclarations::SaturationSplitFreq::ID + suffix, freqsToSplitAt[i]);
    }

    castParameter(apvts, ParamDeclarations::NumSplits::ID, numSplits);
}

Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts) : eqParams(apvts), saturationParams(apvts)
{
    
}


void Parameters::prepareToPlay(juce::dsp::processSpec spec) noexcept
{
    double duration = 0.02f;
    
    double sampleRate = spec.sampleRate;
    for(auto i=0; i<ParamDeclarations::EQ_NUM_BANDS; i++)
    {
        EQ::Band band = eqParams.getParamsForBand(i);
        band.fcSmoother->reset(sampleRate,duration);
        band.QSmoother->reset(sampleRate, duration);
        band.gainInDBSmoother->reset(sampleRate, duration);
    }
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS + 1; i++)
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
    
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS + 1;i++)
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
    
    for(auto i=0; i<ParamDeclarations::SATURATION_MAX_SPLITS + 1;i++)
    {
        Saturation::Band band = saturationParams.getParamsForBand(i);
        band.preGainSmoother->setTargetValue(band.preGain->get());
    }
 }
 


#include "Parameters.h"

