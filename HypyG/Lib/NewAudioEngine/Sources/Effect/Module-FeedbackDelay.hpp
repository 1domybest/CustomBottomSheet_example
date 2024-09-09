/*
 * Baund Studio Engine
 *
 * Module-FeedbackDelay.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Module_FeedbackDelay_hpp
#define BaundStudioEngine_Sources_Effect_Module_FeedbackDelay_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Unit-CombFilter.hpp"

namespace Baund::StudioEngine::Module {
    class FeedbackDelay final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Frames      = AudioBuffer::Frames;
        using Event       = double;
\
        ~FeedbackDelay() override           = default;
        FeedbackDelay()                     = default;
        FeedbackDelay(FeedbackDelay const&) = default;
        
        auto operator=(FeedbackDelay const&) -> FeedbackDelay& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setDelay   (Frames const&) -> void;
        auto setFeedback(Event  const&) -> void;
        auto setDryGain (Event  const&) -> void;
        auto setWetGain (Event  const&) -> void;
        auto getDelay   () const -> Frames;
        auto getFeedback() const -> Event;
        auto getDryGain () const -> Event;
        auto getWetGain () const -> Event;
        
        auto processing(SampleValue const&) -> SampleValue;
        
    private:
        struct MemberVariables {
            Event dryGain = 1.0;
            Event wetGain = 0.0;
            Unit::CombFilter combFilter;
            bool active = false;
        } m;
    };
    
    inline void FeedbackDelay::reset() {
        m = MemberVariables();
    }
    inline bool FeedbackDelay::isValid() const {
        return true;
    }
    inline auto FeedbackDelay::setDelay(Frames const& in) -> void {
        m.active = false;
        m.combFilter.setDelay(in);
        m.active = true;
    }
    inline auto FeedbackDelay::setFeedback(Event const& in) -> void {
        m.combFilter.setFeedback(in);
    }
    inline auto FeedbackDelay::setDryGain(Event const& in) -> void {
        m.dryGain = in;
    }
    inline auto FeedbackDelay::setWetGain(Event const& in) -> void {
        m.wetGain = in;
    }
    inline auto FeedbackDelay::getDelay() const -> Frames {
        return m.combFilter.getDelay();
    }
    inline auto FeedbackDelay::getFeedback() const -> Event {
        return m.combFilter.getFeedback();
    }
    inline auto FeedbackDelay::getDryGain() const -> Event {
        return m.dryGain;
    }
    inline auto FeedbackDelay::getWetGain() const -> Event {
        return m.wetGain;
    }
    inline auto FeedbackDelay::processing(SampleValue const& in) -> SampleValue {
        if (m.active) {
            auto dry = in;
            auto wet = m.combFilter.processing(in);
            return (dry * m.dryGain) + (wet * m.wetGain);
        } else return in;
    }
}

#endif//BaundStudioEngine_Sources_Effect_Module_FeedbackDelay_hpp
