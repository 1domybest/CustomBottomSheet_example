/*
 * Baund Studio Engine
 *
 * EffectLibrary.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_EffectLibrary_hpp
#define BaundStudioEngine_Sources_Project_EffectLibrary_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include <array>

namespace Baund::StudioEngine::Unit {
}
namespace Baund::StudioEngine::Module {
}
namespace Baund::StudioEngine {
    class Effect: public BaseObject {
    public:
        static constexpr auto parameterSize = 128;
        
        using Category   = String;
        using Name       = String;
        using Key        = String;
        using Event      = double;
        using Parameters = std::array<Event,parameterSize>;
        using Tempo      = double;
        enum State { off, bypass, on };
        
        virtual ~Effect() override = default;
        Effect()              = default;
        Effect(Effect const&) = default;
        Effect(Effect     &&);
        
        auto operator=(Effect const&) -> Effect& = default;
        auto operator=(Effect     &&) -> Effect&;

        virtual void reset  ()       override = 0;
        virtual bool isValid() const override = 0;
        
        virtual Category const& getCategory() const = 0;
        virtual Name     const& getName    () const = 0;
        virtual Key      const& getKey     () const = 0;
        virtual Effect*         create     () const = 0;
        
        auto setState(State const&) -> void;
        auto getState() const -> State;
        
        auto getParameters()       -> Parameters&;
        auto getParameters() const -> Parameters const&;
        virtual Name const& getParameterName(Index const&) const = 0;
        
        auto processing(AudioBuffer const&) -> void;
        
        auto setTempo(Tempo const&) -> void;
        auto getTempo() const -> Tempo;
        
    private:
        virtual void processing_off   (AudioBuffer const&);
        virtual void processing_bypass(AudioBuffer const&);
        virtual void processing_on    (AudioBuffer const&) = 0;
        
        struct MemberVariables {
            State        state      = State::on;
            Parameters   parameters = []{ Parameters result; result.fill(-1.0); return result; }();
            Tempo const* tempo      = nullptr;
        } m;
    };
    
    inline Effect::Effect(Effect&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto Effect::operator=(Effect&& in) -> Effect& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Effect::reset() {
        m = MemberVariables();
    }
    inline bool Effect::isValid() const {
        return m.state == State::on;
    }
    inline auto Effect::setState(State const& in) -> void {
        m.state = in;
    }
    inline auto Effect::getState() const -> State {
        return m.state;
    }
    inline auto Effect::getParameters() -> Parameters& {
        return m.parameters;
    }
    inline auto Effect::getParameters() const -> Parameters const& {
        return m.parameters;
    }
    inline auto Effect::processing(AudioBuffer const& in) -> void {
        if      (m.state == State::off   ) processing_off   (in);
        else if (m.state == State::bypass) processing_bypass(in);
        else if (m.state == State::on    ) processing_on    (in);
    }
    inline auto Effect::processing_off(AudioBuffer const& in) -> void {
        in.silence();
    }
    inline auto Effect::processing_bypass(AudioBuffer const&) -> void { }
    inline auto Effect::setTempo(Tempo const& in) -> void {
        m.tempo = &in;
    }
    inline auto Effect::getTempo() const -> Tempo {
        return (m.tempo != nullptr) ? *(m.tempo) : 120.0;
    }

    extern class EffectLibrary final: public BaseObject {
    public:
        using Data = std::vector<std::unique_ptr<Effect>>;
        using Key  = String;
        
        ~EffectLibrary() override = default;
        EffectLibrary()           = default;
        EffectLibrary(EffectLibrary const&) = delete;
        EffectLibrary(EffectLibrary     &&) = default;

        auto operator=(EffectLibrary const&) -> EffectLibrary& = delete;
        auto operator=(EffectLibrary     &&) -> EffectLibrary& = default;

        void reset  ()       override;
        bool isValid() const override;
        
        template< typename T >
        auto add() -> bool;
        auto create(Key const&) -> Effect*;
        
    private:
        struct MemberVariables {
            Data data;
        } m;
    } effectLibrary;
        
    inline void EffectLibrary::reset() {
        m = MemberVariables();
    }
    inline bool EffectLibrary::isValid() const {
        return m.data.size() > 0;
    }
    template< typename T >
    inline auto EffectLibrary::add() -> bool {
        if constexpr(std::is_base_of_v<Effect, T>) {
            m.data.push_back(std::unique_ptr<Effect>(new T));
            return true;
        } else return false;
    }
    inline auto EffectLibrary::create(Key const& in) -> Effect* {
        for (auto& v : m.data) {
            if (v->getKey() == in) {
                auto fx = v->create();
                return fx;
            }
        }
        return nullptr;
    }
}

#endif//BaundStudioEngine_Sources_Project_EffectLibrary_hpp
