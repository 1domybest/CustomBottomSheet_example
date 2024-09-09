/*
 * Baund Studio Engine
 *
 * Unit-AllpassFilter.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Unit_AllpassFilter_hpp
#define BaundStudioEngine_Sources_Effect_Unit_AllpassFilter_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Sources/Effect/Unit-SmoothValue.hpp"

namespace Baund::StudioEngine::Unit {
    class AllpassFilter final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Frames      = AudioBuffer::Frames;
        using Gain        = double;
        using Buffer      = std::vector<SampleValue>;
        
        ~AllpassFilter() override        = default;
        AllpassFilter()                  = default;
        AllpassFilter(AllpassFilter const&);
        auto operator=(AllpassFilter const&) -> AllpassFilter&;
        
        void reset  ()       override;
        bool isValid() const override;

        auto processing(SampleValue const&) -> SampleValue;
        
        auto setDelay(Frames const&) -> void;
        auto setGain (Gain   const&) -> void;
        auto getDelay() const -> Frames;
        auto getGain () const -> Gain;
        
    private:
        struct NonCopyables {
            std::mutex mutex;
        } n;
        struct MemberVariables {
            Buffer buffer;
            Frames position = 0;
            Frames delay    = 0;
            Gain   gain     = 0.0;
            SmoothValue<64> smoothGain  = 0.0;
            SmoothValue<64> smoothInput = 0.0;
            bool   active   = false;
        } m;
    };
    
    inline AllpassFilter::AllpassFilter(AllpassFilter const& in)
    : m(in.m) {
    }
    inline auto AllpassFilter::operator=(AllpassFilter const& in) -> AllpassFilter& {
        this->m = in.m;
        return *this;
    }
    inline void AllpassFilter::reset() {
        m = MemberVariables();
    }
    inline bool AllpassFilter::isValid() const {
        return true;
    }
    inline auto AllpassFilter::setDelay(Frames const& in) -> void {
        std::unique_lock<std::mutex> lock(n.mutex);
        m.active = false;
        m.position = 0;
        m.delay    = in;
        m.buffer.resize(in);
        std::fill(m.buffer.begin(),m.buffer.end(),0.0);
        m.active = true;
    }
    inline auto AllpassFilter::setGain(Gain const& in) -> void {
        m.gain = in;
    }
    inline auto AllpassFilter::getDelay() const -> Frames {
        return m.delay;
    }
    inline auto AllpassFilter::getGain() const -> Gain {
        return m.gain;
    }
    inline auto AllpassFilter::processing(SampleValue const& in) -> SampleValue {
        if (m.active) {
            if (std::isnan(m.buffer[m.position])) m.buffer[m.position] = 0.0;
            auto smoothGain = m.smoothGain.processing(m.gain);
            auto out = (-smoothGain*in) + m.buffer[m.position];
            m.buffer[m.position] = in * m.smoothInput.processing(1.0);
            //auto current = m.buffer[m.position];
            //auto delayed = in - (m.gain*current);
            //auto out     = (m.gain*delayed) + current;
            //m.buffer[m.position] = delayed;
            //auto old = m.buffer[m.position];
            //auto out = in + -m.gain * old;
            //m.buffer[m.position] = in + m.gain * old;
            m.position += 1;
            (m.position >= m.delay) ? m.position = 0 : NULL;
            return out;
            //return m.buffer[m.position];
        } else return in;
    }
}

#endif//BaundStudioEngine_Sources_Effect_Unit_AllpassFilter_hpp
