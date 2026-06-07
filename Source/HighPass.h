/*
  ==============================================================================

    HighPass.h
    Created: 3 Jun 2026 10:21:29pm
    Author:  Larry Scanniello

  ==============================================================================
*/

#pragma once

class HighPass : public Filter::CoefficientStrategy {
    
public:
    
    void updateCoefficients(size_t sampleRate, float fc, float Q,[[maybe_unused]] float gainInDB,std::vector<float> &a, std::vector<float> &b)
    {
        float omega_0 = 2.0f * PI * fc / sampleRate;
        float alpha = std::sin(omega_0) / (2.0f * Q);
        b[0] = (1 + std::cos(omega_0)) * 0.5f;
        b[1] = -(1+std::cos(omega_0));
        b[2] = (1 + std::cos(omega_0)) * 0.5f;
        a[0] = 1 + alpha;
        a[1] = -2 * std::cos(omega_0);
        a[2] = 1 - alpha;
    }
    
};
