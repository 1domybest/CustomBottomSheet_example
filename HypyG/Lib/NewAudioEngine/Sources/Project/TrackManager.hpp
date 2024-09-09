/*
 * Baund Studio Engine
 *
 * TrackManager.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_TrackManager_hpp
#define BaundStudioEngine_Sources_Project_TrackManager_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "ItemManager.hpp"
#include "Mixer.hpp"

namespace Baund::StudioEngine {
    class Track: public PublicObject {
    public:
        virtual ~Track() override = default;
        virtual void reset  ()       override = 0;
        virtual bool isValid() const override = 0;
    };
    class AudioTrack final: public Track {
    public:
        using RoutingAccessor  = RoutingManager::RoutingAccessor;
        using ChannelAccessor  = Mixer::ChannelAccessor;
        using State            = bool;
        using ItemAccessor     = ItemManager::AudioItemAccessor;
        using ItemData         = std::vector<ItemAccessor>;
        using Frames           = AudioBuffer::Frames;
        using MonitorBufferRef = AudioBuffer const* const*;

        ~AudioTrack() override        = default;
        AudioTrack()                  = default;
        AudioTrack(AudioTrack const&) = default;
        AudioTrack(AudioTrack     &&);
        
        auto operator=(AudioTrack const&) -> AudioTrack& = default;
        auto operator=(AudioTrack     &&) -> AudioTrack&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setRouting(RoutingAccessor const&) -> void;
        auto getRouting() const -> RoutingAccessor;
        
        auto setMixChannel(ChannelAccessor const&) -> void;
        auto getMixChannel() const -> ChannelAccessor;
        
        auto setRecordState(State const&) -> void;
        auto getRecordState() const -> State;
        
        auto setMonitorBuffer(AudioBuffer const* const*) -> void;
        auto setMonitorState (State const&) -> void;
        auto getMonitorState () const -> State;
        
        auto getItemData ()       -> ItemData&;
        auto getItemData () const -> ItemData const&;
        auto sortItemData()       -> void;
        
        auto add(ItemAccessor const&) -> void;
        auto sub(ItemAccessor const&) -> void;

        auto writeToOutput(Frames const&, State const&) -> void;

        auto setActiveState(State const&) -> void;
        auto getActiveState() const -> State const&;
        
    private:
        struct MemberVariables {
            RoutingAccessor  routing;
            ChannelAccessor  channel;
            State            record  = false;
            State            monitor = false;
            ItemData         itemData;
            MonitorBufferRef monitorBufferRef = nullptr;
            State            active  = false;
        } m;
    };
    class TrackManager final: public virtual BaseObject {
    public:
        using AudioTrackList     = List<AudioTrack>;
        using AudioTrackAccessor = AudioTrackList::Accessor;
        using Frames             = AudioBuffer::Frames;
        using State              = bool;
        
        ~TrackManager() override          = default;
        TrackManager()                    = default;
        TrackManager(TrackManager const&) = default;
        TrackManager(TrackManager     &&);
        
        auto operator=(TrackManager const&) -> TrackManager& = default;
        auto operator=(TrackManager     &&) -> TrackManager&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto getAudioTrackList()       -> AudioTrackList&;
        auto getAudioTrackList() const -> AudioTrackList const&;
        auto getAudioTrack(PublicObject::ID const&) const -> AudioTrackAccessor;
        
        auto add(AudioTrack         const&) -> AudioTrackAccessor;
        auto add(AudioTrack             &&) -> AudioTrackAccessor;
        auto sub(AudioTrackAccessor const&) -> void;
        
        auto writeToAllOutput(Frames const&, State const&) -> void;

    private:
        struct MemberVariables {
            AudioTrackList audioTrackList;
        } m;
    };

    inline AudioTrack::AudioTrack(AudioTrack&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto AudioTrack::operator=(AudioTrack&& in) -> AudioTrack& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void AudioTrack::reset() {
        m = MemberVariables();
    }
    inline bool AudioTrack::isValid() const {
        return m.routing.isValid();
    }
    inline auto AudioTrack::setRouting(RoutingAccessor const& in) -> void {
        m.routing = in;
    }
    inline auto AudioTrack::getRouting() const -> RoutingAccessor {
        return m.routing;
    }
    inline auto AudioTrack::setMixChannel(ChannelAccessor const& in) -> void {
        m.channel = in;
    }
    inline auto AudioTrack::getMixChannel() const -> ChannelAccessor {
        return m.channel;
    }
    inline auto AudioTrack::setRecordState(State const& in) -> void {
        m.record = in;
    }
    inline auto AudioTrack::getRecordState() const -> State {
        return m.record;
    }
    inline auto AudioTrack::setMonitorBuffer(AudioBuffer const* const* in) -> void {
        m.monitorBufferRef = in;
    }
    inline auto AudioTrack::setMonitorState(State const& in) -> void {
        m.monitor = in;
    }
    inline auto AudioTrack::getMonitorState() const -> State {
        return m.monitor;
    }
    inline auto AudioTrack::getItemData() -> ItemData& {
        return m.itemData;
    }
    inline auto AudioTrack::getItemData() const -> ItemData const& {
        return m.itemData;
    }
    inline auto AudioTrack::sortItemData() -> void {
        auto lock = this->getLock();
        auto invalids = std::vector<ItemAccessor>();
        for (auto i=0; i<m.itemData.size(); i++) {
            if (not m.itemData[i].isValid()) invalids.push_back(m.itemData[i]);
        }
        for (auto accessor : invalids) {
            m.itemData.erase(std::remove(m.itemData.begin(),m.itemData.end(),accessor),m.itemData.end());
        }
        for (auto i=0; i<m.itemData.size(); i++) {
            auto minIndex = i;
            for (auto j=i+1; j<m.itemData.size(); j++) {
                if (m.itemData[j].getValue() < m.itemData[minIndex].getValue()) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                std::swap(m.itemData[i], m.itemData[minIndex]);
            }
        }
    }
    inline auto AudioTrack::add(ItemAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) m.itemData.push_back(in);
    }
    inline auto AudioTrack::sub(ItemAccessor const& in) -> void {
        auto lock = this->getLock();
        m.itemData.erase(std::remove(m.itemData.begin(),m.itemData.end(),in),m.itemData.end());
    }
    inline auto AudioTrack::writeToOutput(Frames const& frame, State const& isPlay) -> void {
        auto lock = this->getLock();
        if (not m.active) return;
        if (not m.routing.isValid()) return;
        auto& oBuffer = m.routing->getAudioBuffer();
        // 녹음 모니터링.
        if (m.monitor and
            m.monitorBufferRef != nullptr and
            *(m.monitorBufferRef) != nullptr) {
            for (auto i=0; i<oBuffer.getFrameSize(); i++) {
                for (auto c=0; c<oBuffer.getChannelSize(); c++) {
                    oBuffer[i][c] = (**(m.monitorBufferRef))[i][c];
                }
            }
            return;
        }
        // 트랙 재생시.
        if (isPlay) for (auto accessor : m.itemData) {
            // 아이템 가져오기.
            if (not accessor.isValid()) continue;
            // 아이템 읽기 및 라우팅버퍼에 쓰기.
            for (auto i=0; i<oBuffer.getFrameSize(); i++) {
                auto readFrame = frame - accessor->getPosition() + i;
                if (readFrame < 0 or readFrame >= accessor->getLength()) continue;
                auto& readBuffer = accessor->readSampleFrame(readFrame);
                for (auto c=0; c<oBuffer.getChannelSize(); c++) {
                    oBuffer[i][c] = readBuffer[0][c];
                }
            }
        }
    }
    inline auto AudioTrack::setActiveState(State const& in) -> void {
        m.active = in;
    }
    inline auto AudioTrack::getActiveState() const -> State const& {
        return m.active;
    }

    inline TrackManager::TrackManager(TrackManager&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto TrackManager::operator=(TrackManager&& in) -> TrackManager& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void TrackManager::reset() {
        m = MemberVariables();
    }
    inline bool TrackManager::isValid() const {
        return m.audioTrackList.count() > 0;
    }
    inline auto TrackManager::getAudioTrackList() -> AudioTrackList& {
        return m.audioTrackList;
    }
    inline auto TrackManager::getAudioTrackList() const -> AudioTrackList const& {
        return m.audioTrackList;
    }
    inline auto TrackManager::getAudioTrack(PublicObject::ID const& in) const -> AudioTrackAccessor {
        auto lock = this->getLock();
        auto const errorResult = (AudioTrackAccessor) {};
        if (in.value == PublicObject::none.value) {
            goto error;
        }
        for (auto accessor : m.audioTrackList) {
            if (accessor->getObjectID().value == in.value) return accessor;
        }
    error:
        return errorResult;
    }
    inline auto TrackManager::add(AudioTrack const& in) -> AudioTrackAccessor {
        auto track = in;
        return add(std::move(track));
    }
    inline auto TrackManager::add(AudioTrack&& in) -> AudioTrackAccessor {
        auto lock = this->getLock();
        return m.audioTrackList.add(std::move(in));
    }
    inline auto TrackManager::sub(AudioTrackAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) {
            in->reset();
            m.audioTrackList.sub(in);
        }
    }
    inline auto TrackManager::writeToAllOutput(Frames const& in, State const& isPlay) -> void {
        auto lock = this->getLock();
        for (auto accessor : m.audioTrackList)
            accessor->writeToOutput(in,isPlay);
    }
}

#endif//BaundStudioEngine_Sources_Project_TrackManager_hpp
