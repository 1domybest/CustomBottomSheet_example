/*
 * Baund Studio Engine
 *
 * Transport.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_Transport_hpp
#define BaundStudioEngine_Sources_Project_Transport_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine {
    class Transport final: public virtual BaseObject {
    public:
        using Frames = AudioBuffer::Frames;
        using State  = bool;
        
        ~Transport() override       = default;
        Transport()                 = default;
        Transport(Transport const&) = default;
        Transport(Transport     &&);
        
        auto operator=(Transport const&) -> Transport& = default;
        auto operator=(Transport     &&) -> Transport&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setPosition   (Frames const&) -> void;
        auto setRecordBegin(Frames const&) -> void;
        auto setPlayState  (State  const&) -> void;
        auto setRecordState(State  const&) -> void;
        auto getPosition   () const -> Frames;
        auto getRecordBegin() const -> Frames;
        auto getPlayState  () const -> State;
        auto getRecordState() const -> State;
        
    private:
        struct MemberVariables {
            Frames position    = 0;
            Frames recordBegin = 0;
            State  play        = false;
            State  record      = false;
        } m;
    };
    
    inline Transport::Transport(Transport&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto Transport::operator=(Transport&& in) -> Transport& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Transport::reset() {
        m = MemberVariables();
    }
    inline bool Transport::isValid() const {
        return true;
    }
    inline auto Transport::setPosition(Frames const& in) -> void {
        m.position = in;
    }
    inline auto Transport::setRecordBegin(Frames const& in) -> void {
        m.recordBegin = in;
    }
    inline auto Transport::setPlayState(State const& in) -> void {
        if (in == true) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (not m.play and m.record)
                m.recordBegin = m.position;
        }
        m.play = in;
    }
    inline auto Transport::setRecordState(State const& in) -> void {
        if (in == true) {
            if (m.play and not m.record)
                m.recordBegin = m.position;
        }
        m.record = in;
    }
    inline auto Transport::getPosition() const -> Frames {
        return m.position;
    }
    inline auto Transport::getRecordBegin() const -> Frames {
        return m.recordBegin;
    }
    inline auto Transport::getPlayState() const -> State {
        return m.play;
    }
    inline auto Transport::getRecordState() const -> State {
        return m.record;
    }
}

#endif//BaundStudioEngine_Sources_Project_Transport_hpp
