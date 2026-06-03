/*
  ==============================================================================

    LinkwitzRiley.cpp
    Created: 27 May 2026 12:18:30am
    Author:  Larry Scanniello

  ==============================================================================
*/

#include "SecondOrderButterworth.h"
#include "Filter.h"
#include <vector>
#include <cmath>



void SecondOrderButterworth::updateCoefficients(float fc, float _Q, float _dB)
{
    /*
        To derive these coefficients, use the Audio EQ Cookbook's formula for high/low pass
        Set Q = sqrt(2)/2 to get Butterworth coefficients
        Then bilinear transform
        The filters need to be cascaded with themselves to get Linkwitz-Riley filters
        The LR filters have the property that you can you separate audio into two by frequency,
        add them back together, and the frequency response is flat, and they have the same phase response
     */
    
    float pi = juce::MathConstants<float>::pi;
    float Q = juce::MathConstants<float>::sqrt2/2.0f;
    
    float theta = 2.0f * pi * fc / sampleRate;
    float c = std::tan(theta * 0.5f);
    float cSq = c * c;
    float normalize = Q + cSq * Q + c;
    float invNormalize = 1.0f/normalize;
    float numFactor = cSq * Q;
    a[0] = 1.0f;
    a[1] =  (2.0f * Q - 2.0f * numFactor) * invNormalize;
    a[2] = (Q + numFactor - c) * invNormalize;
    if(lowPass){
        b[0] = numFactor * invNormalize;
        b[1] = 2.0f * b[0];
        b[2] = b[0];
    }else{
        b[0] = Q * invNormalize;
        b[1] = -2.0f * b[0];
        b[2] = b[0];
    }
}




