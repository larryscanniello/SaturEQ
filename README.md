# SaturEQ

All-in-one multiband saturator and EQ made with C++/JUCE.

<img width="800" height="600" alt="image" src="https://github.com/user-attachments/assets/7b428fcf-3ccb-4baa-ab7f-e2a2758c000b" />


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
      Linkwitz–Riley Crossover (adjustable number of bands)
          ┌────┼────┐
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

The user interface is simple. A more advanced user interface might be built in the future. The saturators may also be upgraded later.

## Acknowledgements

I used code from:

[The Audio Programmer / Matthijs Hollemans Delay](https://github.com/TheAudioProgrammer/BeginnerBookAudioProgramming)

[ZLEqualizer](https://github.com/ZL-Audio/ZLEqualizer)


