/*
  ==============================================================================

    Peaking.h
    Created: 3 Jun 2026 12:45:13am
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once

class Peaking
{
public:
    
    static constexpr float PI = juce::MathConstants<float>::pi;
    
    void updateCoefficients(size_t sampleRate, float fc, float Q, float gainInDB,std::vector<float> &a, std::vector<float> &b)
    {
        float omega_0 = 2.0f * PI * fc / sampleRate;
        float alpha = std::sin(omega_0) / (2.0f * Q);
        float A = std::pow(10.0f,gainInDB/40.0f);
        b[0] = 1 + alpha * A;
        b[1] = -2 * std::cos(omega_0);
        b[2] = 1 - alpha * A;
        a[0] = 1 + alpha/A;
        a[1] = -2 * std::cos(omega_0);
        a[2] = 1 - alpha/A;
    }
};
