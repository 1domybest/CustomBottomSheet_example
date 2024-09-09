/*
 * Baund Studio Engine
 *
 * Effect-Delay.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Effect_Delay_hpp
#define BaundStudioEngine_Sources_Effect_Effect_Delay_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Module-FeedbackDelay.hpp"

namespace Baund::StudioEngine::Delay {
    class HalfNote final: public Effect {
        static inline auto category = std::string("Delay");
        static inline auto name     = std::string("1/2");
        static inline auto key      = std::string("delay-1/2");
        
    public:
        using Beats = double;
        
        ~HalfNote() override      = default;
        HalfNote();
        HalfNote(HalfNote const&) = default;
        
        auto operator=(HalfNote const&) -> HalfNote& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        Category const& getCategory() const override;
        Name     const& getName    () const override;
        Key      const& getKey     () const override;
        Effect*         create     () const override;
        
        Name const& getParameterName(Index const&) const override;
        
    private:
        void processing_off   (AudioBuffer const&) override;
        void processing_bypass(AudioBuffer const&) override;
        void processing_on    (AudioBuffer const&) override;
        
        struct MemberVariables {
            Event amount = 0.5;
            Beats delay  = 0.0;
            Tempo tempo  = 120.0;
            Module::FeedbackDelay feedbackDelayL;
            Module::FeedbackDelay feedbackDelayR;
        } m;
    };
    class QuarterNote final: public Effect {
        static inline auto category = std::string("Delay");
        static inline auto name     = std::string("1/4");
        static inline auto key      = std::string("delay-1/4");
        
    public:
        using Beats = double;
        
        ~QuarterNote() override         = default;
        QuarterNote();
        QuarterNote(QuarterNote const&) = default;
        
        auto operator=(QuarterNote const&) -> QuarterNote& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        Category const& getCategory() const override;
        Name     const& getName    () const override;
        Key      const& getKey     () const override;
        Effect*         create     () const override;
        
        Name const& getParameterName(Index const&) const override;
        
    private:
        void processing_off   (AudioBuffer const&) override;
        void processing_bypass(AudioBuffer const&) override;
        void processing_on    (AudioBuffer const&) override;
        
        struct MemberVariables {
            Event amount = 0.5;
            Beats delay  = 0.0;
            Tempo tempo  = 120.0;
            Module::FeedbackDelay feedbackDelayL;
            Module::FeedbackDelay feedbackDelayR;
        } m;
    };
    class DottedEighthNote final: public Effect {
        static inline auto category = std::string("Delay");
        static inline auto name     = std::string("1/6");
        static inline auto key      = std::string("delay-1/6");
        
    public:
        using Beats = double;
        
        ~DottedEighthNote() override              = default;
        DottedEighthNote();
        DottedEighthNote(DottedEighthNote const&) = default;
        
        auto operator=(DottedEighthNote const&) -> DottedEighthNote& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        Category const& getCategory() const override;
        Name     const& getName    () const override;
        Key      const& getKey     () const override;
        Effect*         create     () const override;
        
        Name const& getParameterName(Index const&) const override;
        
    private:
        void processing_off   (AudioBuffer const&) override;
        void processing_bypass(AudioBuffer const&) override;
        void processing_on    (AudioBuffer const&) override;
        
        struct MemberVariables {
            Event amount = 0.5;
            Beats delay  = 0.0;
            Tempo tempo  = 120.0;
            Module::FeedbackDelay feedbackDelayL;
            Module::FeedbackDelay feedbackDelayR;
        } m;
    };
    class EighthNote final: public Effect {
        static inline auto category = std::string("Delay");
        static inline auto name     = std::string("1/8");
        static inline auto key      = std::string("delay-1/8");
        
    public:
        using Beats = double;
        
        ~EighthNote() override        = default;
        EighthNote();
        EighthNote(EighthNote const&) = default;
        
        auto operator=(EighthNote const&) -> EighthNote& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        Category const& getCategory() const override;
        Name     const& getName    () const override;
        Key      const& getKey     () const override;
        Effect*         create     () const override;
        
        Name const& getParameterName(Index const&) const override;
        
    private:
        void processing_off   (AudioBuffer const&) override;
        void processing_bypass(AudioBuffer const&) override;
        void processing_on    (AudioBuffer const&) override;
        
        struct MemberVariables {
            Event amount = 0.5;
            Beats delay  = 0.0;
            Tempo tempo  = 120.0;
            Module::FeedbackDelay feedbackDelayL;
            Module::FeedbackDelay feedbackDelayR;
        } m;
    };

    inline HalfNote::HalfNote() {
        constexpr auto delayBeat = 2.0;
        m.delay = delayBeat;
        auto srate = deviceManager.getAudioDevice().getSampleRate();
        m.tempo = getTempo();
        auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
        m.feedbackDelayL.setDelay(frames);
        m.feedbackDelayR.setDelay(frames);
        m.feedbackDelayL.setFeedback(0.5);
        m.feedbackDelayR.setFeedback(0.5);
        m.feedbackDelayL.setDryGain(1.0);
        m.feedbackDelayR.setDryGain(1.0);
        getParameters()[0] = m.amount;
        m.feedbackDelayL.setWetGain(m.amount / 4);
        m.feedbackDelayR.setWetGain(m.amount / 4);
    }
    inline void HalfNote::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool HalfNote::isValid() const {
        return Effect::isValid();
    }
    inline HalfNote::Category const& HalfNote::getCategory() const { return category; }
    inline HalfNote::Name     const& HalfNote::getName    () const { return name; }
    inline HalfNote::Key      const& HalfNote::getKey     () const { return key; }
    inline Effect*                   HalfNote::create     () const { return new HalfNote; }
    inline Effect::Name       const& HalfNote::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void HalfNote::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void HalfNote::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.feedbackDelayL.processing(0.0f);
            m.feedbackDelayR.processing(0.0f);
        }
    }
    inline void HalfNote::processing_on(AudioBuffer const& in) {
        if (m.delay == 0.0 or m.tempo != getTempo()) {
            constexpr auto delayBeat = 2.0;
            m.delay = delayBeat;
            auto srate = deviceManager.getAudioDevice().getSampleRate();
            m.tempo = getTempo();
            auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
            m.feedbackDelayL.setDelay(frames);
            m.feedbackDelayR.setDelay(frames);
            m.feedbackDelayL.setFeedback(0.5);
            m.feedbackDelayR.setFeedback(0.5);
            m.feedbackDelayL.setDryGain(1.0);
            m.feedbackDelayR.setDryGain(1.0);
        }
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.feedbackDelayL.setWetGain(m.amount / 4);
            m.feedbackDelayR.setWetGain(m.amount / 4);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.feedbackDelayL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.feedbackDelayR.processing(in[i][1]);
            }
        }
    }

    inline QuarterNote::QuarterNote() {
        constexpr auto delayBeat = 1.0;
        m.delay = delayBeat;
        auto srate = deviceManager.getAudioDevice().getSampleRate();
        m.tempo = getTempo();
        auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
        m.feedbackDelayL.setDelay(frames);
        m.feedbackDelayR.setDelay(frames);
        m.feedbackDelayL.setFeedback(0.5);
        m.feedbackDelayR.setFeedback(0.5);
        m.feedbackDelayL.setDryGain(1.0);
        m.feedbackDelayR.setDryGain(1.0);
        getParameters()[0] = m.amount;
        m.feedbackDelayL.setWetGain(m.amount / 4);
        m.feedbackDelayR.setWetGain(m.amount / 4);
    }
    inline void QuarterNote::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool QuarterNote::isValid() const {
        return Effect::isValid();
    }
    inline QuarterNote::Category const& QuarterNote::getCategory() const { return category; }
    inline QuarterNote::Name     const& QuarterNote::getName    () const { return name; }
    inline QuarterNote::Key      const& QuarterNote::getKey     () const { return key; }
    inline Effect*                      QuarterNote::create     () const { return new QuarterNote; }
    inline Effect::Name          const& QuarterNote::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void QuarterNote::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void QuarterNote::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.feedbackDelayL.processing(0.0f);
            m.feedbackDelayR.processing(0.0f);
        }
    }
    inline void QuarterNote::processing_on(AudioBuffer const& in) {
        if (m.delay == 0.0 or m.tempo != getTempo()) {
            constexpr auto delayBeat = 1.0;
            m.delay = delayBeat;
            auto srate = deviceManager.getAudioDevice().getSampleRate();
            m.tempo = getTempo();
            auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
            m.feedbackDelayL.setDelay(frames);
            m.feedbackDelayR.setDelay(frames);
            m.feedbackDelayL.setFeedback(0.5);
            m.feedbackDelayR.setFeedback(0.5);
            m.feedbackDelayL.setDryGain(1.0);
            m.feedbackDelayR.setDryGain(1.0);
        }
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.feedbackDelayL.setWetGain(m.amount / 4);
            m.feedbackDelayR.setWetGain(m.amount / 4);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.feedbackDelayL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.feedbackDelayR.processing(in[i][1]);
            }
        }
    }

    inline DottedEighthNote::DottedEighthNote() {
        constexpr auto delayBeat = 0.75;
        m.delay = delayBeat;
        auto srate = deviceManager.getAudioDevice().getSampleRate();
        m.tempo = getTempo();
        auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
        m.feedbackDelayL.setDelay(frames);
        m.feedbackDelayR.setDelay(frames);
        m.feedbackDelayL.setFeedback(0.5);
        m.feedbackDelayR.setFeedback(0.5);
        m.feedbackDelayL.setDryGain(1.0);
        m.feedbackDelayR.setDryGain(1.0);
        getParameters()[0] = m.amount;
        m.feedbackDelayL.setWetGain(m.amount / 4);
        m.feedbackDelayR.setWetGain(m.amount / 4);
    }
    inline void DottedEighthNote::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool DottedEighthNote::isValid() const {
        return Effect::isValid();
    }
    inline DottedEighthNote::Category const& DottedEighthNote::getCategory() const { return category; }
    inline DottedEighthNote::Name     const& DottedEighthNote::getName    () const { return name; }
    inline DottedEighthNote::Key      const& DottedEighthNote::getKey     () const { return key; }
    inline Effect*                           DottedEighthNote::create     () const { return new DottedEighthNote; }
    inline Effect::Name               const& DottedEighthNote::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void DottedEighthNote::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void DottedEighthNote::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.feedbackDelayL.processing(0.0f);
            m.feedbackDelayR.processing(0.0f);
        }
    }
    inline void DottedEighthNote::processing_on(AudioBuffer const& in) {
        if (m.delay == 0.0 or m.tempo != getTempo()) {
            constexpr auto delayBeat = 0.75;
            m.delay = delayBeat;
            auto srate = deviceManager.getAudioDevice().getSampleRate();
            m.tempo = getTempo();
            auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
            m.feedbackDelayL.setDelay(frames);
            m.feedbackDelayR.setDelay(frames);
            m.feedbackDelayL.setFeedback(0.5);
            m.feedbackDelayR.setFeedback(0.5);
            m.feedbackDelayL.setDryGain(1.0);
            m.feedbackDelayR.setDryGain(1.0);
        }
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.feedbackDelayL.setWetGain(m.amount / 4);
            m.feedbackDelayR.setWetGain(m.amount / 4);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.feedbackDelayL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.feedbackDelayR.processing(in[i][1]);
            }
        }
    }

    inline EighthNote::EighthNote() {
        constexpr auto delayBeat = 0.5;
        m.delay = delayBeat;
        auto srate = deviceManager.getAudioDevice().getSampleRate();
        m.tempo = getTempo();
        auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
        m.feedbackDelayL.setDelay(frames);
        m.feedbackDelayR.setDelay(frames);
        m.feedbackDelayL.setFeedback(0.5);
        m.feedbackDelayR.setFeedback(0.5);
        m.feedbackDelayL.setDryGain(1.0);
        m.feedbackDelayR.setDryGain(1.0);
        getParameters()[0] = m.amount;
        m.feedbackDelayL.setWetGain(m.amount / 4);
        m.feedbackDelayR.setWetGain(m.amount / 4);
    }
    inline void EighthNote::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool EighthNote::isValid() const {
        return Effect::isValid();
    }
    inline EighthNote::Category const& EighthNote::getCategory() const { return category; }
    inline EighthNote::Name     const& EighthNote::getName    () const { return name; }
    inline EighthNote::Key      const& EighthNote::getKey     () const { return key; }
    inline Effect*                     EighthNote::create     () const { return new EighthNote; }
    inline Effect::Name         const& EighthNote::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void EighthNote::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void EighthNote::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.feedbackDelayL.processing(0.0f);
            m.feedbackDelayR.processing(0.0f);
        }
    }
    inline void EighthNote::processing_on(AudioBuffer const& in) {
        if (m.delay == 0.0 or m.tempo != getTempo()) {
            constexpr auto delayBeat = 0.5;
            m.delay = delayBeat;
            auto srate = deviceManager.getAudioDevice().getSampleRate();
            m.tempo = getTempo();
            auto frames = std::ceil(srate * (60.0/m.tempo) * delayBeat);
            m.feedbackDelayL.setDelay(frames);
            m.feedbackDelayR.setDelay(frames);
            m.feedbackDelayL.setFeedback(0.5);
            m.feedbackDelayR.setFeedback(0.5);
            m.feedbackDelayL.setDryGain(1.0);
            m.feedbackDelayR.setDryGain(1.0);
        }
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.feedbackDelayL.setWetGain(m.amount / 4);
            m.feedbackDelayR.setWetGain(m.amount / 4);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.feedbackDelayL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.feedbackDelayR.processing(in[i][1]);
            }
        }
    }
}

#endif//BaundStudioEngine_Sources_Effect_Effect_Delay_hpp
