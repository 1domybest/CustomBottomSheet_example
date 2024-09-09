/*
 * Baund Studio Engine
 *
 * RoutingManager.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_RoutingManager_hpp
#define BaundStudioEngine_Sources_Project_RoutingManager_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine {
    class Routing final: public virtual BaseObject {
    public:
        using Level = Index;
        using State = bool;
        
        ~Routing() override     = default;
        Routing()               = default;
        Routing(Routing const&) = default;
        Routing(Routing     &&);

        auto operator=(Routing const&) -> Routing& = default;
        auto operator=(Routing     &&) -> Routing&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setLevel(Level const&) -> void;
        auto getLevel() const -> Level;
        
        auto getAudioBuffer()       -> AudioBuffer&;
        auto getAudioBuffer() const -> AudioBuffer const&;
        
        auto operator==(Routing const&) const -> bool;
        auto operator!=(Routing const&) const -> bool;
        auto operator>=(Routing const&) const -> bool;
        auto operator<=(Routing const&) const -> bool;
        auto operator> (Routing const&) const -> bool;
        auto operator< (Routing const&) const -> bool;

        auto setActiveState(State const&) -> void;
        auto getActiveState() const -> State const&;
        
    private:
        struct MemberVariables {
            Level       level       = 0;
            AudioBuffer audioBuffer = AudioBuffer({
                .sampleRate  = deviceManager.getAudioDevice().getSampleRate(),
                .frameSize   = deviceManager.getAudioDevice().getBufferSize(),
                .channelSize = deviceManager.getAudioDevice().getChannelSize()
            });
            State active = false;
        } m;
    };
    class RoutingManager final: public virtual BaseObject {
    public:
        using RoutingList     = List<Routing>;
        using RoutingAccessor = RoutingList::Accessor;
        
        ~RoutingManager() override            = default;
        RoutingManager();
        RoutingManager(RoutingManager const&) = default;
        RoutingManager(RoutingManager     &&);
        
        auto operator=(RoutingManager const&) -> RoutingManager& = default;
        auto operator=(RoutingManager     &&) -> RoutingManager&;

        void reset  ()       override;
        bool isValid() const override;

        auto getRoutingList()       -> RoutingList&;
        auto getRoutingList() const -> RoutingList const&;

        auto getInputMasterRouting () const -> RoutingAccessor;
        auto getOutputMasterRouting() const -> RoutingAccessor;

        auto add(Routing         const&) -> RoutingAccessor;
        auto add(Routing             &&) -> RoutingAccessor;
        auto sub(RoutingAccessor const&) -> void;
            
        auto silenceAllRoutings() -> void;
        auto checkBufferSize() -> void;
        
    private:
        struct MemberVariables {
            RoutingList     routingList;
            RoutingList     masterList;
            RoutingAccessor iMasterRouting;
            RoutingAccessor oMasterRouting;
        } m;
    };
    
    inline Routing::Routing(Routing&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto Routing::operator=(Routing&& in) -> Routing& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Routing::reset() {
        m = MemberVariables();
    }
    inline bool Routing::isValid() const {
        return m.audioBuffer.isValid();
    }
    inline auto Routing::setLevel(Level const& in) -> void {
        m.level = in;
    }
    inline auto Routing::getLevel() const -> Level {
        return m.level;
    }
    inline auto Routing::getAudioBuffer() -> AudioBuffer& {
        return m.audioBuffer;
    }
    inline auto Routing::getAudioBuffer() const -> AudioBuffer const& {
        return m.audioBuffer;
    }
    inline auto Routing::operator==(Routing const& in) const -> bool {
        return this->m.level == in.m.level;
    }
    inline auto Routing::operator!=(Routing const& in) const -> bool {
        return this->m.level != in.m.level;
    }
    inline auto Routing::operator>=(Routing const& in) const -> bool {
        return this->m.level >= in.m.level;
    }
    inline auto Routing::operator<=(Routing const& in) const -> bool {
        return this->m.level <= in.m.level;
    }
    inline auto Routing::operator> (Routing const& in) const -> bool {
        return this->m.level >  in.m.level;
    }
    inline auto Routing::operator< (Routing const& in) const -> bool {
        return this->m.level <  in.m.level;
    }
    inline auto Routing::setActiveState(State const& in) -> void {
        m.active = in;
    }
    inline auto Routing::getActiveState() const -> State const& {
        return m.active;
    }

    inline RoutingManager::RoutingManager() {
        m.iMasterRouting = m.masterList.add(Routing());
        m.oMasterRouting = m.masterList.add(Routing());
        m.iMasterRouting->setActiveState(true);
        m.oMasterRouting->setActiveState(true);
    }
    inline RoutingManager::RoutingManager(RoutingManager&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto RoutingManager::operator=(RoutingManager&& in) -> RoutingManager& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void RoutingManager::reset() {
        m = MemberVariables();
    }
    inline bool RoutingManager::isValid() const {
        return m.routingList.count() > 0;
    }
    inline auto RoutingManager::getRoutingList() -> RoutingList& {
        return m.routingList;
    }
    inline auto RoutingManager::getRoutingList() const -> RoutingList const& {
        return m.routingList;
    }
    inline auto RoutingManager::getInputMasterRouting() const -> RoutingAccessor {
        return m.iMasterRouting;
    }
    inline auto RoutingManager::getOutputMasterRouting() const -> RoutingAccessor {
        return m.oMasterRouting;
    }
    inline auto RoutingManager::add(Routing const& in) -> RoutingAccessor {
        auto routing = in;
        return add(std::move(routing));
    }
    inline auto RoutingManager::add(Routing&& in) -> RoutingAccessor {
        auto lock = this->getLock();
        return m.routingList.add(std::move(in));
    }
    inline auto RoutingManager::sub(RoutingAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) {
            in->reset();
            m.routingList.sub(in);
        }
    }
    inline auto RoutingManager::silenceAllRoutings() -> void {
        auto lock = this->getLock();
        for (auto accessor : m.routingList)
            if (accessor->getActiveState())
                accessor->getAudioBuffer().silence();
        for (auto accessor : m.masterList)
            if (accessor->getActiveState())
                accessor->getAudioBuffer().silence();
    }
    inline auto RoutingManager::checkBufferSize() -> void {
        auto& device = deviceManager.getAudioDevice();
        for (auto accessor : m.routingList) {
            if (not accessor->getActiveState()) continue;
            auto& buffer = accessor->getAudioBuffer();
            if (buffer.getFrameSize() != device.getBufferSize()) {
                buffer = AudioBuffer({
                    .sampleRate = device.getSampleRate(),
                    .frameSize  = device.getBufferSize(),
                    .channelSize = device.getChannelSize()
                });
            }
        }
        auto& iBuffer = m.iMasterRouting->getAudioBuffer();
        if (iBuffer.getFrameSize() != device.getBufferSize()) {
            iBuffer = AudioBuffer({
                .sampleRate  = device.getSampleRate(),
                .frameSize   = device.getBufferSize(),
                .channelSize = device.getChannelSize()
            });
        }
        auto& oBuffer = m.oMasterRouting->getAudioBuffer();
        if (oBuffer.getFrameSize() != device.getBufferSize()) {
            oBuffer = AudioBuffer({
                .sampleRate  = device.getSampleRate(),
                .frameSize   = device.getBufferSize(),
                .channelSize = device.getChannelSize()
            });
        }
    }
}

#endif//BaundStudioEngine_Sources_Project_RoutingManager_hpp
