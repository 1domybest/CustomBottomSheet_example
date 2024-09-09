/*
 * Baund Studio Engine
 *
 * Module-Pan.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Module_Pan_hpp
#define BaundStudioEngine_Sources_Effect_Module_Pan_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Unit-SmoothValue.hpp"

namespace Baund::StudioEngine::Module {
    class Pan final: public BaseObject {
    public:
        using Value = double;
            
        ~Pan() override = default;
        Pan()           = default;
        Pan(Pan const&) = default;
        
        auto operator=(Pan const&) -> Pan& = default;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setValue(Value const&) -> void;
        auto getValue() const -> Value;
        
        auto processing(AudioBuffer const&) -> void;
        
    private:
        struct MemberVariables {
            Value               value  = 0.0;
            Unit::SmoothValue<> smooth = value;
        } m;
    };
    
    inline void Pan::reset() {
        m = MemberVariables();
    }
    inline bool Pan::isValid() const {
        return true;
    }
    inline auto Pan::setValue(Value const& in) -> void {
        m.value = (in > 1.0) ? 1.0 : (in < -1.0) ? -1.0 : in;
    }
    inline auto Pan::getValue() const -> Value {
        return m.value;
    }
    inline auto Pan::processing(AudioBuffer const& in) -> void {
        if (in.getChannelSize() != 2) return;
        for(auto i=0; i<in.getFrameSize(); i++) {
            auto panningValue = m.smooth.processing(m.value);
            for(auto c=0; c<in.getChannelSize(); c++) {
                auto value = panningValue / 2.0 + 0.5;
                value = c == 0
                ? std::sqrt(1-value) * M_SQRT2
                : std::sqrt(value  ) * M_SQRT2;
                value = c == 0
                ? panningValue > 0 ? value : 1
                : panningValue < 0 ? value : 1;
                value = c == 0
                ? panningValue !=  1 ? value : 0
                : panningValue != -1 ? value : 0;
                in[i][c] *= value;
            }
        }
    }
}

#endif//BaundStudioEngine_Sources_Effect_Module_Pan_hpp
