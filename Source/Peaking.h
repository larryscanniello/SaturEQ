/*
  ==============================================================================

    Peaking.h
    Created: 3 Jun 2026 12:45:13am
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once

const float PI = juce::MathConstants<float>::pi;

class Peaking : public Filter
{
public:
    Peaking(size_t numChannels, float centerFreq,float Q, float dB, size_t sampleRate)
    : Filter(numChannels, centerFreq, Q, dB, sampleRate){};
    
    void updateCoefficients(float fc, float Q, float dB,size_t sampleRate)
    {
        float omega_0 = 2.0f * PI * fc / sampleRate;
        float alpha = std::sin(omega_0) / (2.0f * Q);
        float A = std::pow(10.0f,dB/40.0f);
        b[0] = 1 + alpha * A;
        b[1] = -2 * std::cos(omega_0);
        b[2] = 1 - alpha * A;
        a[0] = 1 + alpha/A;
        a[1] = -2 * std::cos(omega_0);
        a[2] = 1 - alpha/A;
    }
};
