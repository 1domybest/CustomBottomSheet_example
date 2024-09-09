/*
 * Baund Studio Engine
 *
 * Module-Gain.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Module_Gain_hpp
#define BaundStudioEngine_Sources_Effect_Module_Gain_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Unit-SmoothValue.hpp"

namespace Baund::StudioEngine::Module {
    class Gain final: public BaseObject {
    public:
        using Value = double;
            
        ~Gain() override  = default;
        Gain()            = default;
        Gain(Gain const&) = default;
        Gain(Value const&);
        
        auto operator=(Gain const&) -> Gain& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setValue(Value const&) -> void;
        auto getValue() const -> Value;
        
        auto processing(AudioBuffer const&) -> void;
        
    private:
        struct MemberVariables {
            Value               value  = 1.0;
            Unit::SmoothValue<> smooth = value;
        } m;
    };
    
    inline Gain::Gain(Value const& in)
    : m { .value = in, .smooth = in } {
    }
    inline void Gain::reset() {
        m = MemberVariables();
    }
    inline bool Gain::isValid() const {
        return true;
    }
    inline auto Gain::setValue(Value const& in) -> void {
        m.value = in > 0.0 ? in : 0.0;
    }
    inline auto Gain::getValue() const -> Value {
        return m.value;
    }
    inline auto Gain::processing(AudioBuffer const& in) -> void {
        for (auto i=0; i<in.getFrameSize(); i++) {
            auto value = m.smooth.processing(m.value);
            for (auto c=0; c<in.getChannelSize(); c++) {
                in[i][c] *= value;
            }
        }
    }
}

#endif//BaundStudioEngine_Sources_Effect_Module_Gain_hpp
