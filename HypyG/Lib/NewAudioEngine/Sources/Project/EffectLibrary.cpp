/*
 * Baund Studio Engine
 *
 * EffectLibrary.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "EffectLibrary.hpp"
#include "Sources/Effect/Effect.hpp"

namespace Baund::StudioEngine {
    EffectLibrary effectLibrary = [] {
        EffectLibrary effectLibrary = { };
        effectLibrary.add<Tone::Bright>();
        effectLibrary.add<Tone::Stuffy>();
        effectLibrary.add<Tone::Thin>();
        effectLibrary.add<Tone::Light>();
        effectLibrary.add<Distortion::FourBit>();
        effectLibrary.add<Distortion::LowBit>();
        effectLibrary.add<Delay::HalfNote>();
        effectLibrary.add<Delay::QuarterNote>();
        effectLibrary.add<Delay::DottedEighthNote>();
        effectLibrary.add<Delay::EighthNote>();
        effectLibrary.add<Reverb::Shorter>();
        effectLibrary.add<Reverb::Medium>();
        effectLibrary.add<Reverb::Longer>();
        return effectLibrary;
    }();
}
