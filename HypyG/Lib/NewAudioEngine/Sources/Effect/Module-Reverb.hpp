/*
 * Baund Studio Engine
 *
 * Module-Reverb.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Module_Reverb_hpp
#define BaundStudioEngine_Sources_Effect_Module_Reverb_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Unit-CombFilter.hpp"
#include "Unit-AllpassFilter.hpp"

namespace Baund::StudioEngine::Module {
    class Reverb final: public BaseObject {
    public:
        using Event       = double;
        using SampleValue = AudioBuffer::SampleValue;
        
        ~Reverb() override = default;
        Reverb();
        Reverb(Reverb const&) = default;
        auto operator=(Reverb const&) -> Reverb& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setLength(Event const&) -> void;
        auto setDry   (Event const&) -> void;
        auto setWet   (Event const&) -> void;
        auto getLength() const -> Event;
        auto getDry   () const -> Event;
        auto getWet   () const -> Event;
        
        auto processingL(SampleValue const&) -> SampleValue;
        auto processingR(SampleValue const&) -> SampleValue;

    private:
        struct MemberVariables {
            Event dry    = 1.0;
            Event wet    = 0.0;
            Event length = 1/3;
            Unit::CombFilter combL1, combL2, combL3, combL4, combR1, combR2, combR3, combR4;
            Unit::AllpassFilter allpassL1, allpassL2, allpassR1, allpassR2;
            bool active = false;
        } m;
    };
    
    inline Reverb::Reverb() {
        auto& audioDevice = deviceManager.getAudioDevice();
        auto  srate       = 44100;
        if (audioDevice.isValid()) srate = audioDevice.getSampleRate();
        //auto srate = deviceManager.getAudioDevice().getSampleRate();
        m.combL1.setDelay(std::floor(0.100*srate));
        m.combL2.setDelay(std::floor(0.115*srate));
        m.combL3.setDelay(std::floor(0.120*srate));
        m.combL4.setDelay(std::floor(0.135*srate));
        m.combR1.setDelay(std::floor(0.105*srate));
        m.combR2.setDelay(std::floor(0.110*srate));
        m.combR3.setDelay(std::floor(0.125*srate));
        m.combR4.setDelay(std::floor(0.130*srate));
        m.allpassL1.setDelay(std::floor(0.025*srate));
        m.allpassL2.setDelay(std::floor(0.010*srate));
        m.allpassR1.setDelay(std::floor(0.025*srate));
        m.allpassR2.setDelay(std::floor(0.010*srate));
        setLength(0.0);
        m.active = true;
    }
    inline auto Reverb::setLength(Event const& in) -> void {
        m.active = false;
        constexpr auto minLength = 1/3;
        constexpr auto maxLength = 1;
        m.length = (in*(1-minLength))+minLength;
        m.combL1.setFeedback(0.80*m.length);
        m.combL2.setFeedback(0.85*m.length);
        m.combL3.setFeedback(0.81*m.length);
        m.combL4.setFeedback(0.84*m.length);
        m.combR1.setFeedback(0.81*m.length);
        m.combR2.setFeedback(0.84*m.length);
        m.combR3.setFeedback(0.83*m.length);
        m.combR4.setFeedback(0.82*m.length);
        m.allpassL1.setGain(0.85*m.length);
        m.allpassL2.setGain(0.85*m.length);
        m.allpassR1.setGain(0.85*m.length);
        m.allpassR2.setGain(0.85*m.length);
        m.active = true;
    }
    inline void Reverb::reset() {
        m = MemberVariables();
    }
    inline bool Reverb::isValid() const {
        return true;
    }
    inline auto Reverb::setDry(Event const& in) -> void {
        m.dry = in;
    }
    inline auto Reverb::setWet(Event const& in) -> void {
        m.wet = in;
    }
    inline auto Reverb::getLength() const -> Event {
        return m.length;
    }
    inline auto Reverb::getDry() const -> Event {
        return m.dry;
    }
    inline auto Reverb::getWet() const -> Event {
        return m.wet;
    }
    inline auto Reverb::processingL(SampleValue const& in) -> SampleValue {
        if (m.active) {
            auto c1 = m.combL1.processing(in);
            auto c2 = m.combL2.processing(in);
            auto c3 = m.combL3.processing(in);
            auto c4 = m.combL4.processing(in);
            auto c  = (+c1-c2+c3-c4)/4;
            auto a1 = m.allpassL1.processing(c)/2;
            auto a2 = m.allpassL2.processing(a1);
            auto d = in * m.dry;
            auto w = a2 * m.wet;
            return d + w;
        } else return in;
    }
    inline auto Reverb::processingR(SampleValue const& in) -> SampleValue {
        if (m.active) {
            auto c1 = m.combR1.processing(in);
            auto c2 = m.combR2.processing(in);
            auto c3 = m.combR3.processing(in);
            auto c4 = m.combR4.processing(in);
            auto c  = (-c1+c2-c3+c4)/4;
            auto a1 = m.allpassR1.processing(c)/2;
            auto a2 = m.allpassR2.processing(a1);
            auto d = in * m.dry;
            auto w = a2 * m.wet;
            return d + w;
        } else return in;
    }
}

#endif//BaundStudioEngine_Sources_Effect_Module_Reverb_hpp
