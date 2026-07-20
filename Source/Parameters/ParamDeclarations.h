/*
  ==============================================================================

    ParamDeclarations.h
    Created: 5 Jun 2026 8:44:24pm
    Author:  Larry Scanniello

  ==============================================================================
*/

// The following code is a derivative work of the code from the ZLEqualizer,
// which is licensed AGPLv3. This code therefore is also licensed under the terms
// of the AGPL, verison 3.

#pragma once

namespace ParamDeclarations {

inline constexpr auto VERSION_HINT = 1;
inline constexpr auto EQ_NUM_BANDS = 6;
inline constexpr auto SATURATION_MAX_SPLITS = 5;
inline constexpr float SATURATION_DEFAULT_SPLIT_FREQS[] = {200.f, 500.f, 1000.f, 3000.f, 8000.f};
inline constexpr float EQ_DEFAULT_FREQS[] = {75.0f,100.0f,250.0f,1040.0f,2500.0f,7500.0f};
inline constexpr float EQ_DEFAULT_QS[] = {1.0f,0.6f,0.3f,0.41f,0.20f,1.0f};
    
enum FilterStatus {
    off, on
};

template <class T>
    class FloatParameters {
    public:
        static std::unique_ptr<juce::AudioParameterFloat> get(const bool automate = true) {
            auto attributes = juce::AudioParameterFloatAttributes().withAutomatable(automate).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(T::ID, VERSION_HINT),
                                                               T::Name, T::Range, T::DefaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterFloat> get(const std::string& suffix) {
            auto attributes = juce::AudioParameterFloatAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                               T::Name + suffix, T::Range, T::DefaultV, attributes);
        }
        
        static std::unique_ptr<juce::AudioParameterFloat> get(const std::string& suffix, const float defaultVal) {
            auto attributes = juce::AudioParameterFloatAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                               T::Name + suffix, T::Range, defaultVal, attributes);
        }

        static std::unique_ptr<juce::AudioParameterFloat> get(const std::string& suffix, const bool meta,
                                                              const bool automate) {
            auto attributes = juce::AudioParameterFloatAttributes().withAutomatable(automate).withLabel(T::Name).
                                                                    withMeta(meta);
            return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                               T::Name + suffix, T::Range, T::DefaultV, attributes);
        }

        inline static float convertTo01(const float x) {
            return T::kRange.convertTo0to1(x);
        }
    };

template <class T>
    class IntParameters {
    public:
        static std::unique_ptr<juce::AudioParameterInt> get(const bool automate = true) {
            auto attributes = juce::AudioParameterIntAttributes().withAutomatable(automate).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterInt>(juce::ParameterID(T::ID, VERSION_HINT),
                                                             T::Name, T::Min, T::Max, T::DefaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterInt> get(const std::string& suffix) {
            auto attributes = juce::AudioParameterIntAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterInt>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                             T::Name + suffix, T::Min, T::Max, T::DefaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterInt> get(const std::string& suffix, const int defaultVal) {
            auto attributes = juce::AudioParameterIntAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterInt>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                             T::Name + suffix, T::Min, T::Max, defaultVal, attributes);
        }

        static std::unique_ptr<juce::AudioParameterInt> get(const std::string& suffix, const bool meta,
                                                             const bool automate) {
            auto attributes = juce::AudioParameterIntAttributes().withAutomatable(automate).withLabel(T::Name).
                                                                  withMeta(meta);
            return std::make_unique<juce::AudioParameterInt>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                             T::Name + suffix, T::Min, T::Max, T::DefaultV, attributes);
        }

        inline static float convertTo01(const int x) {
            return static_cast<float>(x - T::Min) / static_cast<float>(T::Max - T::Min);
        }
    };

    // bool
    template <class T>
    class BoolParameters {
    public:
        static std::unique_ptr<juce::AudioParameterBool> get(const bool automate = true) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(automate).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID, VERSION_HINT),
                                                              T::Name, T::DefaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterBool> get(const std::string& suffix) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                              T::Name + suffix, T::DefaultV, attributes);
        }
        
        static std::unique_ptr<juce::AudioParameterBool> get(const std::string& suffix, bool defaultVal) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                              T::Name + suffix, defaultVal, attributes);
        }

        static std::unique_ptr<juce::AudioParameterBool> get(const std::string& suffix, const bool meta,
                                                             const bool automate) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(automate).withLabel(T::Name).
                                                                   withMeta(meta);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                              T::Name + suffix, T::DefaultV, attributes);
        }

        inline static float convertTo01(const bool x) {
            return x ? 1.f : 0.f;
        }
    };

    // choice
    template <class T>
    class ChoiceParameters {
    public:
        static std::unique_ptr<juce::AudioParameterChoice> get(const bool automate = true) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(automate).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(T::ID, VERSION_HINT),
                                                                T::Name, T::Choices, T::DefaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterChoice> get(const std::string& suffix) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                                T::Name + suffix, T::Choices, T::DefaultV,
                                                                attributes);
        }
        
        static std::unique_ptr<juce::AudioParameterChoice> get(const std::string& suffix,juce::StringArray defaultVal) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(true).withLabel(T::Name);
            return std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                                T::Name + suffix, T::Choices, defaultVal,
                                                                attributes);
        }


        static std::unique_ptr<juce::AudioParameterChoice> get(const std::string& suffix, const bool meta,
                                                               const bool automate) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(automate).withLabel(T::Name).
                                                                     withMeta(meta);
            return std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(T::ID + suffix, VERSION_HINT),
                                                                T::Name + suffix, T::Choices, T::DefaultV,
                                                                attributes);
        }

        inline static float convertTo01(const int x) {
            return static_cast<float>(x) / static_cast<float>(T::kChoices.size() - 1);
        }
    };

template <typename FloatType>
    inline juce::NormalisableRange<FloatType> getLogMidRange(
        const FloatType x_min, const FloatType x_max, const FloatType x_mid, const FloatType x_interval) {
        const FloatType rng1{std::log(x_mid / x_min) * FloatType(2)};
        const FloatType rng2{std::log(x_max / x_mid) * FloatType(2)};
        auto result_range = juce::NormalisableRange<FloatType>{
            x_min, x_max,
            [=](FloatType, FloatType, const FloatType v) {
                return v < FloatType(.5) ? std::exp(v * rng1) * x_min : std::exp((v - FloatType(.5)) * rng2) * x_mid;
            },
            [=](FloatType, FloatType, const FloatType v) {
                return v < x_mid ? std::log(v / x_min) / rng1 : FloatType(.5) + std::log(v / x_mid) / rng2;
            },
            [=](FloatType, FloatType, const FloatType v) {
                const FloatType x = x_min + x_interval * std::round((v - x_min) / x_interval);
                return x <= x_min ? x_min : (x >= x_max ? x_max : x);
            }
        };
        result_range.interval = x_interval;
        return result_range;
    }

    template <typename FloatType>
    inline juce::NormalisableRange<FloatType> getLogMidRangeShift(
        const FloatType x_min, const FloatType x_max, const FloatType x_mid,
        const FloatType x_interval, const FloatType shift) {
        const auto range = getLogMidRange<FloatType>(x_min, x_max, x_mid, x_interval);
        auto result_range = juce::NormalisableRange<FloatType>{
            x_min + shift, x_max + shift,
            [=](FloatType, FloatType, const FloatType v) {
                return range.convertFrom0to1(v) + shift;
            },
            [=](FloatType, FloatType, const FloatType v) {
                return range.convertTo0to1(v - shift);
            },
            [=](FloatType, FloatType, const FloatType v) {
                return range.snapToLegalValue(v - shift) + shift;
            }
        };
        result_range.interval = x_interval;
        return result_range;
    }

    template <typename FloatType>
    inline juce::NormalisableRange<FloatType> getSymmetricLogMidRangeShift(
        const FloatType x_min, const FloatType x_max, const FloatType x_mid,
        const FloatType x_interval, const FloatType shift) {
        const auto range = getLogMidRangeShift<FloatType>(x_min, x_max, x_mid, x_interval, shift);
        auto result_range = juce::NormalisableRange<FloatType>{
            -(x_max + shift), x_max + shift,
            [=](FloatType, FloatType, const FloatType v) {
                if (v > FloatType(0.5)) {
                    return range.convertFrom0to1(v * FloatType(2) - FloatType(1));
                } else {
                    return -range.convertFrom0to1(FloatType(1) - v * FloatType(2));
                }
            },
            [=](FloatType, FloatType, const FloatType v) {
                if (v > FloatType(0)) {
                    return range.convertTo0to1(v) * FloatType(0.5) + FloatType(0.5);
                } else {
                    return FloatType(0.5) - range.convertTo0to1(-v) * FloatType(0.5);
                }
            },
            [=](FloatType, FloatType, const FloatType v) {
                if (v > FloatType(0)) {
                    return range.snapToLegalValue(v);
                } else {
                    return -range.snapToLegalValue(-v);
                }
            }
        };
        result_range.interval = x_interval;
        return result_range;
    }

    template <typename FloatType>
    inline juce::NormalisableRange<FloatType> getLinearMidRange(
        const FloatType x_min, const FloatType x_max, const FloatType x_mid, const FloatType x_interval) {
        auto result_range = juce::NormalisableRange<FloatType>{
            x_min, x_max,
            [=](FloatType, FloatType, const FloatType v) {
                return v < FloatType(.5)
                    ? FloatType(2) * v * (x_mid - x_min) + x_min
                    : FloatType(2) * (v - FloatType(0.5)) * (x_max - x_mid) + x_mid;
            },
            [=](FloatType, FloatType, const FloatType v) {
                return v < x_mid
                    ? FloatType(.5) * (v - x_min) / (x_mid - x_min)
                    : FloatType(.5) + FloatType(.5) * (v - x_mid) / (x_max - x_mid);
            },
            [=](FloatType, FloatType, const FloatType v) {
                const FloatType x = x_min + x_interval * std::round((v - x_min) / x_interval);
                return x <= x_min ? x_min : (x >= x_max ? x_max : x);
            }
        };
        result_range.interval = x_interval;
        return result_range;
    }

class EQFreq0 : public FloatParameters<EQFreq0> {
    public:
        static constexpr auto ID = "freq0";
        static constexpr auto Name = "Freq0";
        inline static const auto Range = getLogMidRange(20.f, 20000.f, 75.0f, 0.1f);
        static constexpr auto DefaultV = 75.0f;
    };

class EQFreq1 : public FloatParameters<EQFreq1> {
    public:
        static constexpr auto ID = "freq1";
        static constexpr auto Name = "Freq1";
        inline static const auto Range = getLogMidRange(20.f, 20000.f, 100.f, 0.1f);
        static constexpr auto DefaultV = 100.f;
    };

class EQFreq2 : public FloatParameters<EQFreq2> {
    public:
        static constexpr auto ID = "freq2";
        static constexpr auto Name = "Freq2";
        inline static const auto Range = getLogMidRange(20.f, 20000.f, 250.f, 0.1f);
        static constexpr auto DefaultV = 250.f;
    };

class EQFreq3 : public FloatParameters<EQFreq3> {
    public:
        static constexpr auto ID = "freq3";
        static constexpr auto Name = "Freq3";
        inline static const auto Range = getLogMidRange(20.f, 20000.f, 1040.0f, 0.1f);
        static constexpr auto DefaultV = 1040.0f;
    };

class EQFreq4 : public FloatParameters<EQFreq4> {
    public:
        static constexpr auto ID = "freq4";
        static constexpr auto Name = "Freq4";
        inline static const auto Range = getLogMidRange(20.f, 20000.f, 2500.f, 0.1f);
        static constexpr auto DefaultV = 2500.f;
    };

class EQFreq5 : public FloatParameters<EQFreq5> {
    public:
        static constexpr auto ID = "freq5";
        static constexpr auto Name = "Freq5";
        inline static const auto Range = getLogMidRange(20.f, 20000.f, 7500.f, 0.1f);
        static constexpr auto DefaultV = 7500.f;
    };

class EQGain : public FloatParameters<EQGain> {
    public:
        static constexpr auto ID = "gain";
        static constexpr auto Name = "Gain";
        inline static const auto Range = juce::NormalisableRange<float>(-30, 30, .01f);
        static constexpr auto DefaultV = 0.f;
    };

class EQQ0 : public FloatParameters<EQQ0> {
    public:
        static constexpr auto ID = "q0";
        static constexpr auto Name = "Q0";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 1.0f, 0.001f);
        static constexpr auto DefaultV = 1.0f;
};

class EQQ1 : public FloatParameters<EQQ1> {
    public:
        static constexpr auto ID = "q1";
        static constexpr auto Name = "Q1";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 0.6f, 0.001f);
        static constexpr auto DefaultV = 0.6f;
};

class EQQ2 : public FloatParameters<EQQ2> {
    public:
        static constexpr auto ID = "q2";
        static constexpr auto Name = "Q2";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 0.3f, 0.001f);
        static constexpr auto DefaultV = 0.3f;
};

class EQQ3 : public FloatParameters<EQQ3> {
    public:
        static constexpr auto ID = "q3";
        static constexpr auto Name = "Q3";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 0.41f, 0.001f);
        static constexpr auto DefaultV = 0.41f;
};

class EQQ4 : public FloatParameters<EQQ4> {
    public:
        static constexpr auto ID = "q4";
        static constexpr auto Name = "Q4";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 0.20f, 0.001f);
        static constexpr auto DefaultV = 0.20f;
};

class EQQ5 : public FloatParameters<EQQ5> {
    public:
        static constexpr auto ID = "q5";
        static constexpr auto Name = "Q5";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 1.0f, 0.001f);
        static constexpr auto DefaultV = 1.0f;
};


class EQBypass : public BoolParameters<EQBypass> {
public:
    static constexpr auto ID = "eqbp";
    static constexpr auto Name = "EQ Bypass";
    static constexpr auto DefaultV = false;
};

class SaturationPreGain : public FloatParameters<SaturationPreGain> {
    public:
        static constexpr auto ID = "s_pg";
        static constexpr auto Name = "Saturation Pre Gain";
        inline static const auto Range = getLogMidRange(0.025f, 25.f, 1.0f, 0.001f);
        static constexpr auto DefaultV = 1.0f;
};

class SaturationBypass : public BoolParameters<SaturationBypass> {
public:
    static constexpr auto ID = "s_bp";
    static constexpr auto Name = "Saturation Band Bypass";
    static constexpr auto DefaultV = false;
};

class SaturationSplitFreq : public FloatParameters<SaturationSplitFreq> {
public:
    static constexpr auto ID = "s_sf";
    static constexpr auto Name = "Sat Split Freq";
    inline static const auto Range = getLogMidRange(20.f, 20000.f, 1000.f, 0.1f);
    static constexpr auto DefaultV = 1000.f;
};

class NumSplits : public IntParameters<NumSplits> {
public:
    static constexpr auto ID = "nsplts";
    static constexpr auto Name = "Number of active splits";
    inline static const auto Min = 0;
    inline static const auto Max = ParamDeclarations::SATURATION_MAX_SPLITS;
    static constexpr auto DefaultV = 3;
};



static inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(){
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(
    EQFreq0::get(true),EQFreq1::get(true),EQFreq2::get(true),
    EQFreq3::get(true),EQFreq4::get(true),EQFreq5::get(true),
    EQQ0::get(true),EQQ1::get(true),EQQ2::get(true),
    EQQ3::get(true),EQQ4::get(true),EQQ5::get(true)
               );
    for(size_t i=0; i< EQ_NUM_BANDS; i++)
    {
        const auto suffix = std::to_string(i);
        layout.add(
                   EQGain::get(suffix),
                   EQBypass::get(suffix));
        }
    for(size_t i=0; i< SATURATION_MAX_SPLITS + 1; i++)
    {
        const auto suffix = std::to_string(i);
        layout.add(
                   SaturationPreGain::get(suffix),
                   SaturationBypass::get(suffix));
    }
    for(size_t i=0; i < SATURATION_MAX_SPLITS; i++)
    {
        const auto suffix = std::to_string(i);
        layout.add(SaturationSplitFreq::get(suffix, SATURATION_DEFAULT_SPLIT_FREQS[i]));
    }
    layout.add(NumSplits::get());
    
    return layout;
};




}
