/*
 * Baund Studio Engine
 *
 * Unit-Biquad1.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Unit_Biquad1_hpp
#define BaundStudioEngine_Sources_Effect_Unit_Biquad1_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine::Unit {
    class Biquad1 final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        
        ~Biquad1() override     = default;
        Biquad1()               = default;
        Biquad1(Biquad1 const&) = default;
        
        auto operator=(Biquad1 const&) -> Biquad1& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        struct Args {
            SampleValue a0;
            SampleValue a1;
            SampleValue a2;
            SampleValue b0;
            SampleValue b1;
            SampleValue b2;
        };
        auto processing(SampleValue const&) -> SampleValue;
        auto setValues (Args        const&) -> void;
        
    private:
        struct MemberVariables {
            SampleValue b0a0 = 0.0;
            SampleValue b1a0 = 0.0;
            SampleValue b2a0 = 0.0;
            SampleValue a1a0 = 0.0;
            SampleValue a2a0 = 0.0;
            SampleValue out1 = 0.0;
            SampleValue out2 = 0.0;
            SampleValue in1  = 0.0;
            SampleValue in2  = 0.0;
        } m;
    };
    
    inline void Biquad1::reset() {
        m = MemberVariables();
    }
    inline bool Biquad1::isValid() const {
        return true;
    }
    inline auto Biquad1::processing(SampleValue const& in0) -> SampleValue {
        auto yn = m.b0a0*in0 + m.b1a0*m.in1 + m.b2a0*m.in2 - m.a1a0*m.out1 - m.a2a0*m.out2;
        m.in2  = m.in1;
        m.in1  = in0;
        m.out2 = m.out1;
        m.out1 = yn;
        return yn;
    }
    inline auto Biquad1::setValues(Args const& args) -> void {
        m.b0a0 = args.b0 / args.a0;
        m.b1a0 = args.b1 / args.a0;
        m.b2a0 = args.b2 / args.a0;
        m.a1a0 = args.a1 / args.a0;
        m.a2a0 = args.a2 / args.a0;
    }
}

#endif//BaundStudioEngine_Sources_Effect_Unit_Biquad1_hpp
