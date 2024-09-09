/*
 * Baund Studio Engine
 *
 * Effect-Reverb.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Effect_Reverb_hpp
#define BaundStudioEngine_Sources_Effect_Effect_Reverb_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Module-Reverb.hpp"

namespace Baund::StudioEngine::Reverb {
    class Shorter final: public Effect {
        static inline auto category = std::string("Reverb");
        static inline auto name     = std::string("Shorter");
        static inline auto key      = std::string("reverb-shorter");
        
    public:
        ~Shorter() override     = default;
        Shorter();
        Shorter(Shorter const&) = default;
        
        auto operator=(Shorter const&) -> Shorter& = default;
        
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
            Event             amount = 0.5;
            Module::Reverb    reverb;// = []{ Module::Reverb fx; fx.setLength(0.0); return fx; }();
            Module::RBJFilter highShelfL;
            Module::RBJFilter highShelfR;
        } m;
    };
    class Medium final: public Effect {
        static inline auto category = std::string("Reverb");
        static inline auto name     = std::string("Medium");
        static inline auto key      = std::string("reverb-medium");
        
    public:
        ~Medium() override    = default;
        Medium();
        Medium(Medium const&) = default;
        
        auto operator=(Medium const&) -> Medium& = default;
        
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
            Event             amount = 0.5;
            Module::Reverb    reverb;// = []{ Module::Reverb fx; fx.setLength(0.5); return fx; }();
            Module::RBJFilter highShelfL;
            Module::RBJFilter highShelfR;
        } m;
    };
    class Longer final: public Effect {
        static inline auto category = std::string("Reverb");
        static inline auto name     = std::string("Longer");
        static inline auto key      = std::string("reverb-longer");
        
    public:
        ~Longer() override    = default;
        Longer();
        Longer(Longer const&) = default;
        
        auto operator=(Longer const&) -> Longer& = default;
        
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
            Event             amount = 0.5;
            Module::Reverb    reverb;// = []{ Module::Reverb fx; fx.setLength(1.0); return fx; }();
            Module::RBJFilter highShelfL;
            Module::RBJFilter highShelfR;
        } m;
    };

    inline Shorter::Shorter() {
        getParameters()[0] = m.amount;
        m.reverb.setWet(m.amount);
        m.reverb.setLength(0.0);
        m.highShelfL.setType(Module::RBJFilter::Filter::highShelf);
        m.highShelfR.setType(Module::RBJFilter::Filter::highShelf);
        m.highShelfL.setFrequency(8000);
        m.highShelfR.setFrequency(8000);
        m.highShelfL.setGainDecibel(-3.0);
        m.highShelfR.setGainDecibel(-3.0);
    }
    inline void Shorter::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Shorter::isValid() const {
        return Effect::isValid();
    }
    inline Shorter::Category const& Shorter::getCategory() const { return category; }
    inline Shorter::Name     const& Shorter::getName    () const { return name; }
    inline Shorter::Key      const& Shorter::getKey     () const { return key; }
    inline Effect*                  Shorter::create     () const { return new Shorter; }
    inline Effect::Name      const& Shorter::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Shorter::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Shorter::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.reverb.processingL(0.0f);
            m.reverb.processingR(0.0f);
            m.highShelfL.processing(0.0f);
            m.highShelfR.processing(0.0f);
        }
    }
    inline void Shorter::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.reverb.setWet(m.amount);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.reverb.processingL(in[i][0]);
                in[i][0] = m.highShelfL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.reverb.processingR(in[i][1]);
                in[i][1] = m.highShelfR.processing(in[i][1]);
            } else {
                in[i][1] = m.reverb.processingR(in[i][0]);
                in[i][1] = m.highShelfR.processing(in[i][1]);
            }
        }
    }
    
    inline Medium::Medium() {
        getParameters()[0] = m.amount;
        m.reverb.setWet(m.amount);
        m.reverb.setLength(0.5);
        m.highShelfL.setType(Module::RBJFilter::Filter::highShelf);
        m.highShelfR.setType(Module::RBJFilter::Filter::highShelf);
        m.highShelfL.setFrequency(8000);
        m.highShelfR.setFrequency(8000);
        m.highShelfL.setGainDecibel(-3.0);
        m.highShelfR.setGainDecibel(-3.0);
    }
    inline void Medium::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Medium::isValid() const {
        return Effect::isValid();
    }
    inline Medium::Category const& Medium::getCategory() const { return category; }
    inline Medium::Name     const& Medium::getName    () const { return name; }
    inline Medium::Key      const& Medium::getKey     () const { return key; }
    inline Effect*                 Medium::create     () const { return new Medium; }
    inline Effect::Name     const& Medium::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Medium::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Medium::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.reverb.processingL(0.0f);
            m.reverb.processingR(0.0f);
            m.highShelfL.processing(0.0f);
            m.highShelfR.processing(0.0f);
        }
    }
    inline void Medium::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.reverb.setWet(m.amount);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.reverb.processingL(in[i][0]);
                in[i][0] = m.highShelfL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.reverb.processingR(in[i][1]);
                in[i][1] = m.highShelfR.processing(in[i][1]);
            } else {
                in[i][1] = m.reverb.processingR(in[i][0]);
                in[i][1] = m.highShelfR.processing(in[i][1]);
            }
        }
    }

    inline Longer::Longer() {
        getParameters()[0] = m.amount;
        m.reverb.setWet(m.amount);
        m.reverb.setLength(1.0);
        m.highShelfL.setType(Module::RBJFilter::Filter::highShelf);
        m.highShelfR.setType(Module::RBJFilter::Filter::highShelf);
        m.highShelfL.setFrequency(8000);
        m.highShelfR.setFrequency(8000);
        m.highShelfL.setGainDecibel(-3.0);
        m.highShelfR.setGainDecibel(-3.0);
    }
    inline void Longer::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool Longer::isValid() const {
        return Effect::isValid();
    }
    inline Longer::Category const& Longer::getCategory() const { return category; }
    inline Longer::Name     const& Longer::getName    () const { return name; }
    inline Longer::Key      const& Longer::getKey     () const { return key; }
    inline Effect*                 Longer::create     () const { return new Longer; }
    inline Effect::Name     const& Longer::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void Longer::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void Longer::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.reverb.processingL(0.0f);
            m.reverb.processingR(0.0f);
            m.highShelfL.processing(0.0f);
            m.highShelfR.processing(0.0f);
        }
    }
    inline void Longer::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
            m.reverb.setWet(m.amount);
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                in[i][0] = m.reverb.processingL(in[i][0]);
                in[i][0] = m.highShelfL.processing(in[i][0]);
            }
            if (in.getChannelSize() > 1) {
                in[i][1] = m.reverb.processingR(in[i][1]);
                in[i][1] = m.highShelfR.processing(in[i][1]);
            } else {
                in[i][1] = m.reverb.processingR(in[i][0]);
                in[i][1] = m.highShelfR.processing(in[i][1]);
            }
        }
    }
}

#endif /* Effect_Reverb_hpp */
