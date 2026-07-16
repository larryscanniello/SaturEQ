# SaturEQ

All-in-one multiband saturator and EQ made with C++/JUCE.

## Features

- Multiple band EQ supporting low pass, high pass, and peaking filters (courtesy of the Robert Bristow-Johnson Audio EQ cookbook)
- Multiband tanh saturation, splitting the signal with Linkwitz-Riley crossovers
- Ability to swap filter strategies in real-time, and also dynamically add and reduce splits in real-time

## DSP Flow

                 Input
                   │
                   ▼
           2× Oversampling
                   │
                   ▼
      Linkwitz–Riley Crossover
          ┌────┼────┐
          ▼    ▼    ▼
        Low   Mid  High
          │    │    │
          ▼    ▼    ▼
      Saturation (per band)
          │    │    │
          └────┼────┘
               ▼
         Band Summation
               │
               ▼
              EQ
               │
               ▼
         2× Downsampling
               │
               ▼
             Output

## Built With

- JUCE
- C++
- Projucer
- Xcode

## Future Improvements

Right now, the DSP is complete, and the plugin is functional audio-wise. The parameters are wired in with APVTS. The user interface is currently being built.

## Acknowledgements

I used code from:

[The Audio Programmer / Matthijs Hollemans Delay](https://github.com/TheAudioProgrammer/BeginnerBookAudioProgramming)

[ZLEqualizer](https://github.com/ZL-Audio/ZLEqualizer)


