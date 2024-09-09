/*
 * Baund Studio Engine
 *
 * Effect-Distortion.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Effect_Distortion_hpp
#define BaundStudioEngine_Sources_Effect_Effect_Distortion_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Module-BitCrusher.hpp"

namespace Baund::StudioEngine::Distortion {
    class FourBit final: public Effect {
        static inline auto category = std::string("Distortion");
        static inline auto name     = std::string("4Bit");
        static inline auto key      = std::string("distortion-4bit");
        
    public:
        ~FourBit() override     = default;
        FourBit();
        FourBit(FourBit const&) = default;
        
        auto operator=(FourBit const&) -> FourBit& = default;
        
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
            Module::BitCrusher2 bitCrusherL = []{ Module::BitCrusher2 fx; fx.setStep(4); return fx; }();
            Module::BitCrusher2 bitCrusherR = []{ Module::BitCrusher2 fx; fx.setStep(4); return fx; }();
        } m;
    };
    class LowBit final: public Effect {
        static inline auto category = std::string("Distortion");
        static inline auto name     = std::string("LowBit");
        static inline auto key      = std::string("distortion-lowbit");
        
    public:
        ~LowBit() override    = default;
        LowBit();
        LowBit(LowBit const&) = default;
        
        auto operator=(LowBit const&) -> LowBit& = default;
        
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
            Module::BitCrusher1 bitCrusherL = []{ Module::BitCrusher1 fx; fx.setStep(2); return fx; }();
            Module::BitCrusher1 bitCrusherR = []{ Module::BitCrusher1 fx; fx.setStep(2); return fx; }();
        } m;
    };

    inline FourBit::FourBit() {
        getParameters()[0] = m.amount;
    }
    inline void FourBit::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool FourBit::isValid() const {
        return Effect::isValid();
    }
    inline FourBit::Category const& FourBit::getCategory() const { return category; }
    inline FourBit::Name     const& FourBit::getName    () const { return name; }
    inline FourBit::Key      const& FourBit::getKey     () const { return key; }
    inline Effect*                  FourBit::create     () const { return new FourBit; }
    inline Effect::Name      const& FourBit::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void FourBit::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void FourBit::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.bitCrusherL.processing(0.0f);
            m.bitCrusherR.processing(0.0f);
        }
    }
    inline void FourBit::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                auto dry = in[i][0];
                auto wet = in[i][0];
                //if (std::abs(wet) > 1.0 / m.bitCrusherL.getStep() / 32) {
                //if (std::abs(wet) > 0.0001) {
                    wet = m.bitCrusherL.processing(in[i][0]);
                //} else wet = 0.0;
                // auto wet = m.bitCrusherL.processing(in[i][1]);
                in[i][0] = (dry * (1.0 - m.amount)) + (wet * m.amount);
            }
            if (in.getChannelSize() > 1) {
                auto dry = in[i][1];
                auto wet = in[i][1];
                //if (std::abs(wet) > 1.0 / m.bitCrusherR.getStep() / 32) {
                    wet = m.bitCrusherR.processing(in[i][1]);
                //} else wet = 0.0;
                // auto wet = m.bitCrusherR.processing(in[i][1]);
                in[i][1] = (dry * (1.0 - m.amount)) + (wet * m.amount);
            }
        }
    }
    
    inline LowBit::LowBit() {
        getParameters()[0] = m.amount;
;
    }
    inline void LowBit::reset() {
        m = MemberVariables();
        Effect::reset();
    }
    inline bool LowBit::isValid() const {
        return Effect::isValid();
    }
    inline LowBit::Category const& LowBit::getCategory() const { return category; }
    inline LowBit::Name     const& LowBit::getName    () const { return name; }
    inline LowBit::Key      const& LowBit::getKey     () const { return key; }
    inline Effect*                 LowBit::create     () const { return new LowBit; }
    inline Effect::Name     const& LowBit::getParameterName(Index const& in) const {
        static auto parameterName0    = std::string("amount");
        static auto parameterNameNull = std::string("");
        if (in == 0) return parameterName0;
        else         return parameterNameNull;
    }
    inline void LowBit::processing_off(AudioBuffer const& in) {
        in.silence();
    }
    inline void LowBit::processing_bypass(AudioBuffer const& in) {
        for (auto i=0; i<in.getFrameSize(); i++) {
            m.bitCrusherL.processing(0.0f);
            m.bitCrusherR.processing(0.0f);
        }
    }
    inline void LowBit::processing_on(AudioBuffer const& in) {
        if (getParameters()[0] < 0.0) getParameters()[0] = 0.0;
        if (getParameters()[0] > 1.0) getParameters()[0] = 1.0;
        if (getParameters()[0] != m.amount) {
            m.amount = getParameters()[0];
        }
        for (auto i=0; i<in.getFrameSize(); i++) {
            if (in.getChannelSize() > 0) {
                auto dry = in[i][0];
                auto wet = in[i][0];
                //if (std::abs(wet) > 1.0 / m.bitCrusherL.getStep() / 2) {
                if (std::abs(wet) > 0.0001) {
                    wet = m.bitCrusherL.processing(in[i][0]);
                } else wet = 0.0;
                //auto wet = m.bitCrusherL.processing(in[i][0]);
                in[i][0] = (dry * (1.0 - m.amount)) + (wet * m.amount);
            }
            if (in.getChannelSize() > 1) {
                auto dry = in[i][1];
                auto wet = in[i][1];
                //if (std::abs(wet) > 1.0 / m.bitCrusherR.getStep() / 2) {
                if (std::abs(wet) > 0.0001) {
                    wet = m.bitCrusherR.processing(in[i][1]);
                } else wet = 0.0;
                //auto wet = m.bitCrusherL.processing(in[i][1]);
                in[i][1] = (dry * (1.0 - m.amount)) + (wet * m.amount);
            }
        }
    }
}

#endif /* Effect_Distortion_hpp */
