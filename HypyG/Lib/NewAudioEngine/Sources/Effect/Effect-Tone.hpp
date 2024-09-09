/*
 * Baund Studio Engine
 *
 * Effect-Tone.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Effect_Tone_hpp
#define BaundStudioEngine_Sources_Effect_Effect_Tone_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Module-RBJFilter.hpp"

namespace Baund::StudioEngine::Tone {
    class Bright final: public Effect {
        static inline auto category = std::string("Tone");
        static inline auto name     = std::string("Bright");
        static inline auto key      = std::string("tone-bright");
        
    public:
        ~Bright() override    = default;
        Bright();
        Bright(Bright const&) = default;
        
        auto operator=(Bright const&) -> Bright& = default;

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
            Event             amount  = 0.5;
            Module::RBJFilter filterL = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highShelf,
                .frequency   = 1000.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
            Module::RBJFilter filterR = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highShelf,
                .frequency   = 1000.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
        } m;
    };
    class Stuffy final: public Effect {
        static inline auto category = std::string("Tone");
        static inline auto name     = std::string("Stuffy");
        static inline auto key      = std::string("tone-stuffy");
        
    public:
        ~Stuffy() override    = default;
        Stuffy();
        Stuffy(Stuffy const&) = default;
        
        auto operator=(Stuffy const&) -> Stuffy& = default;

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
            Event             amount  = 0.5;
            Module::RBJFilter filterL = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highShelf,
                .frequency   = 1000.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
            Module::RBJFilter filterR = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highShelf,
                .frequency   = 1000.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
        } m;
    };
    class Thin final: public Effect {
        static inline auto category = std::string("Tone");
        static inline auto name     = std::string("Thin");
        static inline auto key      = std::string("tone-thin");
        
    public:
        ~Thin() override  = default;
        Thin();
        Thin(Thin const&) = default;
        
        auto operator=(Thin const&) -> Thin& = default;

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
            Event             amount  = 0.5;
            Module::RBJFilter filterL = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highPass,
                .frequency   = 20.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
            Module::RBJFilter filterR = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highPass,
                .frequency   = 20.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
        } m;
    };
    class Light final: public Effect {
        static inline auto category = std::string("Tone");
        static inline auto name     = std::string("Light");
        static inline auto key      = std::string("tone-light");
        
    public:
        ~Light() override   = default;
        Light();
        Light(Light const&) = default;
        
        auto operator=(Light const&) -> Light& = default;

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
            Event             amount   = 0.5;
            Module::RBJFilter filterL1 = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highPass,
                .frequency   = 20.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
            Module::RBJFilter filterR1 = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::highPass,
                .frequency   = 20.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
            Module::RBJFilter filterL2 = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::lowPass,
                .frequency   = 20000.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
            Module::RBJFilter filterR2 = Module::RBJFilter({
                .type        = Module::RBJFilter::Filter::lowPass,
                .frequency   = 20000.0,
                .resValue    = 1.0,
                .gainDecibel = 0.0,
                .resonant    = Module::RBJFilter::Resonant::Q
            });
        } m;
    };

    inline Bright::Bright() {
        getParameters()[0] = m.amount;
        constexpr auto maxDecibel = 12.0;
        auto gainDecibel = m.amount * maxDecibel;
        m.filterL.setGainDecibel(gainDecibel);
        m.filterR.setGainDecibel(gainDecibel);
    }
    inline void Bright::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Bright::isValid() const {
        return Effect::isValid();
    }
    inline Bright::Category const& Bright::getCategory() const { return category; }
    inline Bright::Name     const& Bright::getName    () const { return name; }
    inline Bright::Key      const& Bright::getKey     () const { return key; }
    inline Effect*                 Bright::create     () const { return new Bright; }
    inline Effect::Name     const& Bright::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Bright::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Bright::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.filterL.processing(0.0f);
            m.filterR.processing(0.0f);
        }
    }
    inline void Bright::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            constexpr auto maxDecibel = 12.0;
            auto gainDecibel = m.amount * maxDecibel;
            m.filterL.setGainDecibel(gainDecibel);
            m.filterR.setGainDecibel(gainDecibel);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0)
                in[i][0] = m.filterL.processing(in[i][0]);
            if (in.getChannelSize() > 1)
                in[i][1] = m.filterR.processing(in[i][1]);
        }
    }
    
    inline Stuffy::Stuffy() {
        getParameters()[0] = m.amount;
        constexpr auto maxDecibel = -24.0;
        auto gainDecibel = m.amount * maxDecibel;
        m.filterL.setGainDecibel(gainDecibel);
        m.filterR.setGainDecibel(gainDecibel);
    }
    inline void Stuffy::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Stuffy::isValid() const {
        return Effect::isValid();
    }
    inline Stuffy::Category const& Stuffy::getCategory() const { return category; }
    inline Stuffy::Name     const& Stuffy::getName    () const { return name; }
    inline Stuffy::Key      const& Stuffy::getKey     () const { return key; }
    inline Effect*                 Stuffy::create     () const { return new Stuffy; }
    inline Effect::Name     const& Stuffy::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Stuffy::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Stuffy::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.filterL.processing(0.0f);
            m.filterR.processing(0.0f);
        }
    }
    inline void Stuffy::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            constexpr auto maxDecibel = -24.0;
            auto gainDecibel = m.amount * maxDecibel;
            m.filterL.setGainDecibel(gainDecibel);
            m.filterR.setGainDecibel(gainDecibel);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0)
                in[i][0] = m.filterL.processing(in[i][0]);
            if (in.getChannelSize() > 1)
                in[i][1] = m.filterR.processing(in[i][1]);
        }
    }

    inline Thin::Thin() {
        getParameters()[0] = m.amount;
        constexpr auto minFreq = 20.0;
        constexpr auto maxFreq = 4000.0;
        auto freq = (m.amount * (maxFreq-minFreq)) + minFreq;
        m.filterL.setFrequency(freq);
        m.filterR.setFrequency(freq);
    }
    inline void Thin::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Thin::isValid() const {
        return Effect::isValid();
    }
    inline Thin::Category const& Thin::getCategory() const { return category; }
    inline Thin::Name     const& Thin::getName    () const { return name; }
    inline Thin::Key      const& Thin::getKey     () const { return key; }
    inline Effect*               Thin::create     () const { return new Thin; }
    inline Effect::Name   const& Thin::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Thin::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Thin::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.filterL.processing(0.0f);
            m.filterR.processing(0.0f);
        }
    }
    inline void Thin::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            constexpr auto minFreq = 20.0;
            constexpr auto maxFreq = 4000.0;
            auto freq = (m.amount * (maxFreq-minFreq)) + minFreq;
            m.filterL.setFrequency(freq);
            m.filterR.setFrequency(freq);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0)
                in[i][0] = m.filterL.processing(in[i][0]);
            if (in.getChannelSize() > 1)
                in[i][1] = m.filterR.processing(in[i][1]);
        }
    }

    inline Light::Light() {
        getParameters()[0] = m.amount;
        constexpr auto minFreq = 20.0;
        constexpr auto midFreq = 2000.0;
        constexpr auto maxFreq = 20000.0;
        auto freq1 = (m.amount * (midFreq-minFreq)) + minFreq;
        auto freq2 = maxFreq - (m.amount * (maxFreq-midFreq));
        m.filterL1.setFrequency(freq1);
        m.filterR1.setFrequency(freq1);
        m.filterL2.setFrequency(freq2);
        m.filterR2.setFrequency(freq2);
    }
    inline void Light::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Light::isValid() const {
        return Effect::isValid();
    }
    inline Light::Category const& Light::getCategory() const { return category; }
    inline Light::Name     const& Light::getName    () const { return name; }
    inline Light::Key      const& Light::getKey     () const { return key; }
    inline Effect*                Light::create     () const { return new Light; }
    inline Effect::Name    const& Light::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Light::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Light::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.filterL1.processing(0.0f);
            m.filterR1.processing(0.0f);
            m.filterL2.processing(0.0f);
            m.filterR2.processing(0.0f);
        }
    }
    inline void Light::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            constexpr auto minFreq = 20.0;
            constexpr auto midFreq = 2000.0;
            constexpr auto maxFreq = 20000.0;
            auto freq1 = (m.amount * (midFreq-minFreq)) + minFreq;
            auto freq2 = maxFreq - (m.amount * (maxFreq-midFreq));
            m.filterL1.setFrequency(freq1);
            m.filterR1.setFrequency(freq1);
            m.filterL2.setFrequency(freq2);
            m.filterR2.setFrequency(freq2);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.filterL1.processing(in[i][0]);
                in[i][0] = m.filterL2.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.filterR1.processing(in[i][1]);
                in[i][1] = m.filterR2.processing(in[i][1]);
            }
        }
    }
}

#endif//BaundStudioEngine_Sources_Effect_Effect_Tone_hpp
