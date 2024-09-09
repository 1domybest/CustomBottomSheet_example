/*
 * Baund Studio Engine
 *
 * Unit-SmoothValue.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Unit_SmoothValue_hpp
#define BaundStudioEngine_Sources_Effect_Unit_SmoothValue_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine::Unit {
    template< auto _limitRatio = 64 >
    class SmoothValue final: public BaseObject {
    public:
        using Value = double;
        
        ~SmoothValue() override         = default;
        SmoothValue()                   = default;
        SmoothValue(SmoothValue const&);
        SmoothValue(Value       const&);
        
        auto operator=(SmoothValue const&) -> SmoothValue&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto processing(Value const&) -> Value;

    private:
        struct NonCopyables {
            Value targetValue = 0.0;
            Value oldValue    = targetValue;
        } n;
    };
    
    template< auto _limitRatio >
    inline SmoothValue<_limitRatio>::SmoothValue(SmoothValue const& in)
    : n({.targetValue = in.n.targetValue,.oldValue = in.n.targetValue}) {
    }
    template< auto _limitRatio >
    inline SmoothValue<_limitRatio>::SmoothValue(Value const& in)
    : n({.targetValue = in,.oldValue = in}) {
    }
    template< auto _limitRatio >
    inline auto SmoothValue<_limitRatio>::operator=(SmoothValue const& in) -> SmoothValue& {
        this->n.targetValue = in.n.targetValue;
        this->n.oldValue    = in.n.targetValue;
        return *this;
    }
    template< auto _limitRatio >
    inline void SmoothValue<_limitRatio>::reset() {
        n.oldValue = n.targetValue;
    }
    template< auto _limitRatio >
    inline bool SmoothValue<_limitRatio>::isValid() const {
        return n.oldValue != n.targetValue;
    }
    template< auto _limitRatio >
    inline auto SmoothValue<_limitRatio>::processing(Value const& in) -> Value {
        constexpr auto limitValue = 1.0 / _limitRatio;
        n.targetValue = in;
        auto distance = std::abs(n.targetValue - n.oldValue);
        if (distance >= limitValue) {
            if (n.oldValue >= n.targetValue)
                return n.oldValue -= limitValue;
            else return n.oldValue += limitValue;
        } else return n.oldValue = n.targetValue;
    }
}

#endif//BaundStudioEngine_Sources_Effect_Unit_SmoothValue_hpp
