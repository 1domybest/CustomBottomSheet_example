/*
 * Baund Studio Engine
 *
 * Unit-BitQuantize.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Unit_BitQuantize_hpp
#define BaundStudioEngine_Sources_Effect_Unit_BitQuantize_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine::Unit {
    class BitQuantize1 final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Step        = double;
        
        ~BitQuantize1() override          = default;
        BitQuantize1()                    = default;
        BitQuantize1(BitQuantize1 const&) = default;
        
        auto operator=(BitQuantize1 const&) -> BitQuantize1& = default;
        
        void reset  ()       override;
        bool isValid() const override;

        auto processing(SampleValue const&) -> SampleValue;
        auto setStep   (Step        const&) -> void;
        auto getStep() const -> Step;
        
    private:
        struct MemberVariables {
            Step step = 16.0;
        } m;
    };
    class BitQuantize2 final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Step        = double;
        
        ~BitQuantize2() override          = default;
        BitQuantize2()                    = default;
        BitQuantize2(BitQuantize2 const&) = default;
        
        auto operator=(BitQuantize2 const&) -> BitQuantize2& = default;
        
        void reset  ()       override;
        bool isValid() const override;

        auto processing(SampleValue const&) -> SampleValue;
        auto setStep   (Step        const&) -> void;
        auto getStep() const -> Step;
        
    private:
        struct MemberVariables {
            Step step = 16.0;
        } m;
    };

    inline void BitQuantize1::reset() {
        m = MemberVariables();
    }
    inline bool BitQuantize1::isValid() const {
        return true;
    }
    inline auto BitQuantize1::processing(SampleValue const& in) -> SampleValue {
        auto depth = std::pow(2,m.step);
        auto out   = std::floor(in*depth) / depth;
        return out;
    }
    inline auto BitQuantize1::setStep(Step const& in) -> void {
        m.step = in;
    }
    inline auto BitQuantize1::getStep() const -> Step {
        return m.step;
    }
    
    inline void BitQuantize2::reset() {
        m = MemberVariables();
    }
    inline bool BitQuantize2::isValid() const {
        return true;
    }
    inline auto BitQuantize2::processing(SampleValue const& in) -> SampleValue {
        //auto depth = std::pow(2,m.step);
        //auto out   = std::floor(in*depth) / depth;
        auto depth = std::pow(2,(int(m.step)|0)-1.0);
        auto inver = 1.0 / depth;
        auto noise = 1.0 / 16;
        auto out   = std::max(std::min((std::floor(in*depth+noise)*inver),1.0),-1.0);
        return out;
    }
    inline auto BitQuantize2::setStep(Step const& in) -> void {
        m.step = in;
    }
    inline auto BitQuantize2::getStep() const -> Step {
        return m.step;
    }

}

#endif//BaundStudioEngine_Sources_Effect_Unit_BitQuantize_hpp
