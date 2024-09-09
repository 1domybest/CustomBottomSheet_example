/*
 * Baund Studio Engine
 *
 * Module-BitCrusher.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Module_BitCrusher_hpp
#define BaundStudioEngine_Sources_Effect_Module_BitCrusher_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Unit-BitQuantize.hpp"

namespace Baund::StudioEngine::Module {
    class BitCrusher1 final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Event       = double;
        
        ~BitCrusher1() override         = default;
        BitCrusher1()                   = default;
        BitCrusher1(BitCrusher1 const&) = default;
        
        auto operator=(BitCrusher1 const&) -> BitCrusher1& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setStep(Event const&) -> void;
        auto getStep() const -> Event;
        
        auto processing(SampleValue const&) -> SampleValue;
        
    private:
        struct MemberVariables {
            Unit::BitQuantize1 bitQuantize;
        } m;
    };
    class BitCrusher2 final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Event       = double;
        
        ~BitCrusher2() override         = default;
        BitCrusher2()                   = default;
        BitCrusher2(BitCrusher2 const&) = default;
        
        auto operator=(BitCrusher2 const&) -> BitCrusher2& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setStep(Event const&) -> void;
        auto getStep() const -> Event;
        
        auto processing(SampleValue const&) -> SampleValue;
        
    private:
        struct MemberVariables {
            Unit::BitQuantize2 bitQuantize;
        } m;
    };
    
    inline void BitCrusher1::reset() {
        m = MemberVariables();
    }
    inline bool BitCrusher1::isValid() const {
        return true;
    }
    inline auto BitCrusher1::setStep(Event const& in) -> void {
        m.bitQuantize.setStep(in);
    }
    inline auto BitCrusher1::getStep() const -> Event {
        return m.bitQuantize.getStep();
    }
    inline auto BitCrusher1::processing(SampleValue const& in) -> SampleValue {
        return m.bitQuantize.processing(in);
    }
    
    inline void BitCrusher2::reset() {
        m = MemberVariables();
    }
    inline bool BitCrusher2::isValid() const {
        return true;
    }
    inline auto BitCrusher2::setStep(Event const& in) -> void {
        m.bitQuantize.setStep(in);
    }
    inline auto BitCrusher2::getStep() const -> Event {
        return m.bitQuantize.getStep();
    }
    inline auto BitCrusher2::processing(SampleValue const& in) -> SampleValue {
        return m.bitQuantize.processing(in);
    }
}

#endif//BaundStudioEngine_Sources_Effect_Module_BitCrusher_hpp
