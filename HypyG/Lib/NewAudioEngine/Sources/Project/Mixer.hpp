/*
 * Baund Studio Engine
 *
 * Mixer.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_Mixer_hpp
#define BaundStudioEngine_Sources_Project_Mixer_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Sources/Effect/Effect.hpp"
#include "RoutingManager.hpp"

namespace Baund::StudioEngine {
    class MixChannel final: public virtual BaseObject {
    public:
        using RoutingAccessor = RoutingManager::RoutingAccessor;
        using State           = bool;
        using Insert          = std::vector<std::unique_ptr<Effect>>;
        
        ~MixChannel() override        = default;
        MixChannel()                  = default;
        MixChannel(MixChannel const&);
        MixChannel(MixChannel     &&);
        
        auto operator=(MixChannel const&) -> MixChannel&;
        auto operator=(MixChannel     &&) -> MixChannel&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setInputRouting (RoutingAccessor const&) -> void;
        auto setOutputRouting(RoutingAccessor const&) -> void;
        auto getInputRouting () const -> RoutingAccessor;
        auto getOutputRouting() const -> RoutingAccessor;

        auto setPanValue (Module::Pan::Value const&) -> void;
        auto getPanValue () const -> Module::Pan::Value;
        auto setGainValue(Module::Gain::Value const&) -> void;
        auto getGainValue() const -> Module::Gain::Value;
        
        auto setMuteState(State const&) -> void;
        auto getMuteState() const -> State;

        auto writeToOutputRouting() -> void;

        auto setInsertFX(Effect*, Index) -> void;
        auto getInsertFX(Index) -> Effect*;
        
        auto operator==(MixChannel const&) const -> bool;
        auto operator!=(MixChannel const&) const -> bool;
        auto operator>=(MixChannel const&) const -> bool;
        auto operator<=(MixChannel const&) const -> bool;
        auto operator> (MixChannel const&) const -> bool;
        auto operator< (MixChannel const&) const -> bool;
        
        auto setActiveState(State const&) -> void;
        auto getActiveState() const -> State;
        
    private:
        struct NonCopyables {
            Insert insert;
            std::mutex mutex;
        } n;
        struct MemberVariables {
            RoutingAccessor iRouting;
            RoutingAccessor oRouting;
            State           mute = false;
            AudioBuffer     buffer;
            Module::Gain    gain = Module::Gain(0.5);
            Module::Pan     pan;
            State           active = false;
        } m;
    };
    class Mixer final: public virtual BaseObject {
    public:
        using ChannelList     = List<MixChannel>;
        using ChannelAccessor = ChannelList::Accessor;
        
        ~Mixer() override   = default;
        Mixer()             = default;
        Mixer(Mixer const&) = default;
        Mixer(Mixer     &&);
        
        auto operator=(Mixer const&) -> Mixer& = default;
        auto operator=(Mixer     &&) -> Mixer&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto getMasterChannel()       -> MixChannel&;
        auto getMasterChannel() const -> MixChannel const&;

        auto getChannelList ()       -> ChannelList&;
        auto getChannelList () const -> ChannelList const&;
        auto sortChannelList() -> void;

        auto add(MixChannel      const&) -> ChannelAccessor;
        auto add(MixChannel          &&) -> ChannelAccessor;
        auto sub(ChannelAccessor const&) -> void;
        
        auto writeToAllRouting() -> void;

    private:
        struct MemberVariables {
            MixChannel  masterChannel;
            ChannelList channelList;
        } m;
    };
    
    inline MixChannel::MixChannel(MixChannel const& in)
    : m(in.m) {
    }
    inline MixChannel::MixChannel(MixChannel&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto MixChannel::operator=(MixChannel const& in) -> MixChannel& {
        this->m = in.m;
        return *this;
    }
    inline auto MixChannel::operator=(MixChannel&& in) -> MixChannel& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void MixChannel::reset() {
        m = MemberVariables();
    }
    inline bool MixChannel::isValid() const {
        return m.iRouting.isValid();
    }
    inline auto MixChannel::setInputRouting(RoutingAccessor const& in) -> void {
        m.iRouting = in;
    }
    inline auto MixChannel::setOutputRouting(RoutingAccessor const& in) -> void {
        m.oRouting = in;
    }
    inline auto MixChannel::getInputRouting() const -> RoutingAccessor {
        return m.iRouting;
    }
    inline auto MixChannel::getOutputRouting() const -> RoutingAccessor {
        return m.oRouting;
    }
    inline auto MixChannel::setPanValue(Module::Pan::Value const& in) -> void {
        auto value = in;
        if (value >  1.0) value =  1.0;
        if (value < -1.0) value = -1.0;
        m.pan.setValue(value);
    }
    inline auto MixChannel::getPanValue() const -> Module::Pan::Value {
        return m.pan.getValue();
    }
    inline auto MixChannel::setGainValue(Module::Gain::Value const& in) -> void {
        auto value = in;
        if (value < 0.0) value = 0.0;
        m.gain.setValue(value);
    }
    inline auto MixChannel::getGainValue() const -> Module::Gain::Value {
        return m.gain.getValue();
    }
    inline auto MixChannel::setMuteState(State const& in) -> void {
        m.mute = in;
    }
    inline auto MixChannel::getMuteState() const -> State {
        return m.mute;
    }
    inline auto MixChannel::writeToOutputRouting() -> void {
        std::unique_lock<std::mutex> lock(n.mutex);
        if (not m.active) return;
        // 입력라우팅 버퍼 읽기.
        if (not m.iRouting.isValid()) return;
        m.buffer = m.iRouting->getAudioBuffer();
        // 인서트 FX 적용.
        for (auto& effect : n.insert) {
            if (effect == nullptr) continue;
            else effect->processing(m.buffer);
        }
        // 패닝 및 게인적용.
        m.pan .processing(m.buffer);
        m.gain.processing(m.buffer);
        // 뮤트적용 및 출력라우팅 버퍼에 쓰기.
        if (m.mute) return;
        if (not m.oRouting.isValid()) return;
        for (auto i=0; i<m.buffer.getFrameSize(); i++) {
            for (auto c=0; c<m.buffer.getChannelSize(); c++) {
                m.oRouting->getAudioBuffer()[i][c] += m.buffer[i][c];
            }
        }
    }
    inline auto MixChannel::setInsertFX(Effect* fx, Index in) -> void {
        std::unique_lock<std::mutex> lock(n.mutex);
        if (in >= n.insert.size()) n.insert.resize(in+1);
        n.insert[in].reset(fx);
        //n.insert[in] = std::unique_ptr<Effect>(fx);
    }
    inline auto MixChannel::getInsertFX(Index in) -> Effect* {
        std::unique_lock<std::mutex> lock(n.mutex);
        if (in >= n.insert.size()) return nullptr;
        else return n.insert[in].get();
    }
    inline auto MixChannel::operator==(MixChannel const& in) const -> bool {
        if (not this->m.iRouting.isValid() or not in.m.iRouting.isValid()) {
            return this->m.iRouting.isValid();
        } else {
            return this->m.iRouting->getLevel() == in.m.iRouting->getLevel();
        }
    }
    inline auto MixChannel::operator!=(MixChannel const& in) const -> bool {
        if (not this->m.iRouting.isValid() or not in.m.iRouting.isValid()) {
            return this->m.iRouting.isValid();
        } else {
            return this->m.iRouting->getLevel() != in.m.iRouting->getLevel();
        }
    }
    inline auto MixChannel::operator>=(MixChannel const& in) const -> bool {
        if (not this->m.iRouting.isValid() or not in.m.iRouting.isValid()) {
            return this->m.iRouting.isValid();
        } else {
            return this->m.iRouting->getLevel() >= in.m.iRouting->getLevel();
        }
    }
    inline auto MixChannel::operator<=(MixChannel const& in) const -> bool {
        if (not this->m.iRouting.isValid() or not in.m.iRouting.isValid()) {
            return this->m.iRouting.isValid();
        } else {
            return this->m.iRouting->getLevel() <= in.m.iRouting->getLevel();
        }
    }
    inline auto MixChannel::operator> (MixChannel const& in) const -> bool {
        if (not this->m.iRouting.isValid() or not in.m.iRouting.isValid()) {
            return this->m.iRouting.isValid();
        } else {
            return this->m.iRouting->getLevel() >  in.m.iRouting->getLevel();
        }
    }
    inline auto MixChannel::operator< (MixChannel const& in) const -> bool {
        if (not this->m.iRouting.isValid() or not in.m.iRouting.isValid()) {
            return this->m.iRouting.isValid();
        } else {
            return this->m.iRouting->getLevel() <  in.m.iRouting->getLevel();
        }
    }
    inline auto MixChannel::setActiveState(State const& in) -> void {
        m.active = in;
    }
    inline auto MixChannel::getActiveState() const -> State {
        return m.active;
    }
    
    inline Mixer::Mixer(Mixer&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto Mixer::operator=(Mixer&& in) -> Mixer& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Mixer::reset() {
        m = MemberVariables();
    }
    inline bool Mixer::isValid() const {
        return m.channelList.count() > 0;
    }
    inline auto Mixer::getMasterChannel() -> MixChannel& {
        return m.masterChannel;
    }
    inline auto Mixer::getMasterChannel() const -> MixChannel const& {
        return m.masterChannel;
    }
    inline auto Mixer::getChannelList() -> ChannelList& {
        return m.channelList;
    }
    inline auto Mixer::getChannelList() const -> ChannelList const& {
        return m.channelList;
    }
    inline auto Mixer::sortChannelList() -> void {
        auto lock = this->getLock();
        // 체널순번 초기화.
        for (auto accessor : m.channelList) {
            auto input = accessor->getInputRouting();
            if (input.isValid()) input->setLevel(0);
        }
        // 체널순번 설정.
        for (auto accessor : m.channelList) {
            auto input  = accessor->getInputRouting();
            auto output = accessor->getOutputRouting();
            auto level  = Routing::Level(0);
            if (input .isValid()) level = input->getLevel();
            if (output.isValid()) output->setLevel(level + 1);
        }
        // 체널 재정렬.
        m.channelList.sort();
    }
    inline auto Mixer::add(MixChannel const& in) -> ChannelAccessor {
        auto lock = this->getLock();
        auto channel = in;
        return m.channelList.add(std::move(channel));
    }
    inline auto Mixer::add(MixChannel&& in) -> ChannelAccessor {
        auto lock = this->getLock();
        return m.channelList.add(std::move(in));
    }
    inline auto Mixer::sub(ChannelAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) {
            in->reset();
            m.channelList.sub(in);
        }
    }
    inline auto Mixer::writeToAllRouting() -> void {
        auto lock = this->getLock();
        for(auto accessor : m.channelList)
            accessor->writeToOutputRouting();
        m.masterChannel.writeToOutputRouting();
    }
}

#endif//BaundStudioEngine_Sources_Project_Mixer_hpp
