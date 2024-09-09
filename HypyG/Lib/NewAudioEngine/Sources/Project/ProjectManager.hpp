/*
 * Baund Studio Engine
 *
 * ProjectManager.hpp
 * v1.0.3
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_ProjectManager_hpp
#define BaundStudioEngine_Sources_Project_ProjectManager_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "TrackManager.hpp"
#include "Transport.hpp"

namespace Baund::StudioEngine {
    class Project final: public PublicObject {
    public:
        using State             = bool;
        using RecordingData     = std::vector<AudioBuffer>;
        using MonitorBufferRef  = AudioBuffer const*;
        using SampleValue       = AudioBuffer::SampleValue;
        using AudioItemAccessor = ItemManager::AudioItemAccessor;
        using Frames            = AudioBuffer::Frames;
        using Tempo             = double;
        
        ~Project() override     = default;
        Project()               = default;
        Project(Project const&);
        Project(Project     &&);
        
        auto operator=(Project const&) -> Project&;
        auto operator=(Project     &&) -> Project&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto getMediaFileManager()       -> MediaFileManager&;
        auto getMediaFileManager() const -> MediaFileManager const&;
        auto getItemManager     ()       -> ItemManager&;
        auto getItemManager     () const -> ItemManager const&;
        auto getRoutingManager  ()       -> RoutingManager&;
        auto getRoutingManager  () const -> RoutingManager const&;
        auto getMixer           ()       -> Mixer&;
        auto getMixer           () const -> Mixer const&;
        auto getTrackManager    ()       -> TrackManager&;
        auto getTrackManager    () const -> TrackManager const&;
        auto getTransport       ()       -> Transport&;
        auto getTransport       () const -> Transport const&;
        
        auto ioCallback(AudioBuffer const&) -> void;
        auto errorCallback() -> void;
        
        auto getRecordingData    () -> RecordingData const&;
        auto getMonitorBufferRef () -> MonitorBufferRef const&;
        auto getRecordMeterValue () -> SampleValue;
        auto writeDataToAudioItem() -> void;
        auto getRecordingItem    () -> AudioItemAccessor;
        auto getLength           () -> Frames;
        auto audioMixdown        () -> AudioFile;
        auto clearRecordingData  () -> void;
        
        auto setGlobalTempo(Tempo const&) -> void;
        auto getGlobalTempo() const -> Tempo const&;
        
        auto connectionToIO  () -> void;
        auto deconnectionToIO() -> void;
        auto isConnectionToIO() const -> bool;
        
    private:
        struct NonCopyables {
            AudioFileWriter recorder;
            TaskQueue       taskQueue;
        } n;
        struct MemberVariables {
            MediaFileManager  fileManager;
            ItemManager       itemManager;
            RoutingManager    routingManager;
            Mixer             mixer;
            TrackManager      trackManager;
            Transport         transport;
            RecordingData     recordingData;
            MonitorBufferRef  monitorBufferRef = nullptr;
            SampleValue       recordMeterValue = 0.0f;
            AudioItemAccessor recordingItem;
            Tempo             tempo = 120.0;
            State             connectionToIO = false;
        } m;
    };
    extern class ProjectManager final: public virtual BaseObject {
    public:
        using ProjectList     = List<Project>;
        using ProjectAccessor = ProjectList::Accessor;
        
        ~ProjectManager() override            = default;
        ProjectManager()                      = default;
        ProjectManager(ProjectManager const&) = delete;
        ProjectManager(ProjectManager     &&) = delete;
        
        auto operator=(ProjectManager const&) -> ProjectManager& = delete;
        auto operator=(ProjectManager     &&) -> ProjectManager& = delete;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setActivated(ProjectAccessor const&) -> void;
        auto getActivated() const -> ProjectAccessor;
        auto isActivated(ProjectAccessor const&) const -> bool;
        
        auto getProjectList()       -> ProjectList&;
        auto getProjectList() const -> ProjectList const&;
        auto getProject(PublicObject::ID const&) const -> ProjectAccessor;
        
        auto add(Project         const&) -> ProjectAccessor;
        auto add(Project             &&) -> ProjectAccessor;
        auto sub(ProjectAccessor const&) -> void;
        
    private:
        struct MemberVariables {
            ProjectList     projectList;
            ProjectAccessor activated;
        } m;
    } projectManager;

    inline Project::Project(Project const& in)
    : m(in.m) {
    }
    inline Project::Project(Project&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto Project::operator=(Project const& in) -> Project& {
        this->m = in.m;
        return *this;
    }
    inline auto Project::operator=(Project&& in) -> Project& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Project::reset() {
        m = MemberVariables();
    }
    inline bool Project::isValid() const {
        return PublicObject::isValid();
    }
    inline auto Project::getMediaFileManager() -> MediaFileManager& {
        return m.fileManager;
    }
    inline auto Project::getMediaFileManager() const -> MediaFileManager const& {
        return m.fileManager;
    }
    inline auto Project::getItemManager() -> ItemManager& {
        return m.itemManager;
    }
    inline auto Project::getItemManager() const -> ItemManager const& {
        return m.itemManager;
    }
    inline auto Project::getRoutingManager() -> RoutingManager& {
        return m.routingManager;
    }
    inline auto Project::getRoutingManager() const -> RoutingManager const& {
        return m.routingManager;
    }
    inline auto Project::getMixer() -> Mixer& {
        return m.mixer;
    }
    inline auto Project::getMixer() const -> Mixer const& {
        return m.mixer;
    }
    inline auto Project::getTrackManager() -> TrackManager& {
        return m.trackManager;
    }
    inline auto Project::getTrackManager() const -> TrackManager const& {
        return m.trackManager;
    }
    inline auto Project::getTransport() -> Transport& {
        return m.transport;
    }
    inline auto Project::getTransport() const -> Transport const& {
        return m.transport;
    }
    inline auto Project::getRecordingData() -> RecordingData const& {
        return m.recordingData;
    }
    inline auto Project::getMonitorBufferRef() -> MonitorBufferRef const& {
        return m.monitorBufferRef;
    }
    inline auto Project::getRecordMeterValue() -> SampleValue {
        return m.recordMeterValue;
    }
    inline auto Project::writeDataToAudioItem() -> void {
        // 오디오파일 등록.
        auto lock = this->getLock();
        m.recordingItem.reset();
        if (n.recorder.isStart()) {
            n.recorder.stop();
            n.recorder.close();
            while (not n.taskQueue.isEmpty() and n.recorder.isReady())
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else return;
        auto filePath  = n.recorder.getPath();
        auto audioFile = AudioFile(std::move(filePath));
        auto frameSize = audioFile.getFrameSize();
        audioFile.setWriteFormat(AudioFile::Format::AAC);
        audioFile.setTempDirRef (m.fileManager.getTempDir());
        auto fileAccessor = m.fileManager.add(std::move(audioFile));
        if (not fileAccessor.isValid()) return;
        else if (fileAccessor->buffer.getFrameSize() == 0) return;
        // 오디오아이템 생성.
        auto audioItem = AudioItem();
        audioItem.setPosition (m.transport.getRecordBegin());
        audioItem.setLength   (frameSize);
        audioItem.setAudioFile(fileAccessor);
#if defined(__APPLE__)
        constexpr auto codecDelay = 96;
        auto latency = deviceManager.getAudioDevice().getLatency();
#elif defined(__ANDROID__)
        constexpr auto codecDelay = 2048;
        auto latency = deviceManager.getAudioDevice().getLatency();
#endif
        auto delay = codecDelay + latency;
        audioItem.setOffset(-delay);
        auto itemAccessor = m.itemManager.add(std::move(audioItem));
        // 트랙에 추가.
        for (auto accessor : m.trackManager.getAudioTrackList()) {
            if (accessor->getRecordState() == true) {
                accessor->add(itemAccessor);
                itemAccessor->setAudioTrack(accessor->getObjectID());
                accessor->sortItemData();
                m.recordingData.clear();
                m.recordingItem = itemAccessor;
                break;
            }
        }
    }
    inline auto Project::getRecordingItem() -> AudioItemAccessor {
        auto result = m.recordingItem;
        m.recordingItem.reset();
        return result;
    }
    inline auto Project::getLength() -> Frames {
        auto lastFrame = Frames(0);
        for (auto accessor : m.itemManager.getAudioItemList()) {
            auto endFrame = accessor->getPosition() + accessor->getLength();
            if (endFrame > lastFrame) lastFrame = endFrame;
        }
        return lastFrame;
    }
    inline auto Project::clearRecordingData() -> void {
        n.recorder.stop();
        n.recorder.close();
    }

    inline auto Project::setGlobalTempo(Tempo const& in) -> void {
        m.tempo = in;
    }
    inline auto Project::getGlobalTempo() const -> Tempo const& {
        return m.tempo;
    }
    inline auto Project::connectionToIO() -> void {
        m.connectionToIO = true;
    }
    inline auto Project::deconnectionToIO() -> void {
        m.connectionToIO = false;
    }
    inline auto Project::isConnectionToIO() const -> bool {
        return m.connectionToIO;
    }

    inline void ProjectManager::reset() {
        m = MemberVariables();
    }
    inline bool ProjectManager::isValid() const {
        return m.projectList.count() > 0;
    }
    inline auto ProjectManager::setActivated(ProjectAccessor const& in) -> void {
        // 오디오기기 가져오기.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) return;
        // 오디오기기 콜백 등록.
        [&device,&in] {
            auto project = in;
            if (project.isValid()) {
                device.setIOCallback([project](AudioBuffer const& buffer){
                    if(not project.isValid()) return;
                    else if (project->isConnectionToIO()) project->ioCallback(buffer);
                    else buffer.silence();
                });
                device.setErrorCallback([project]{
                    if(not project.isValid()) return;
                    else project->errorCallback();
                });
                project->connectionToIO();
            } else {
                device.setIOCallback   (nullptr);
                device.setErrorCallback(nullptr);
            }
        }();
        // 활성화 프로젝트 등록.
        if (m.activated.isValid()) m.activated->deconnectionToIO();
        m.activated = in;
    }
    inline auto ProjectManager::getActivated() const -> ProjectAccessor {
        return m.activated;
    }
    inline auto ProjectManager::isActivated(ProjectAccessor const& in) const -> bool {
        if (not in.isValid()) return false;
        else return m.activated == in;
    }

    inline auto ProjectManager::getProjectList() -> ProjectList& {
        return m.projectList;
    }
    inline auto ProjectManager::getProjectList() const -> ProjectList const& {
        return m.projectList;
    }
    inline auto ProjectManager::getProject(PublicObject::ID const& in) const -> ProjectAccessor {
        auto lock = this->getLock();
        auto const errorResult = (ProjectAccessor) {};
        if (in.value == PublicObject::none.value) {
            goto error;
        }
        for (auto accessor : m.projectList) {
            if (accessor->getObjectID().value == in.value) return accessor;
        }
    error:
        return errorResult;
    }
    inline auto ProjectManager::add(Project const& in) -> ProjectAccessor {
        auto project = in;
        return add(std::move(project));
    }
    inline auto ProjectManager::add(Project&& in) -> ProjectAccessor {
        auto lock = this->getLock();
        return m.projectList.add(std::move(in));
    }
    inline auto ProjectManager::sub(ProjectAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) {
            if (isActivated(in)) {
                for (auto accessor : m.projectList) {
                    if (accessor != in) {
                        setActivated(accessor);
                        break;
                    }
                }
            }
            if (isActivated(in)) setActivated({});
            in->reset();
            m.projectList.sub(in);
        }
    }
}

#endif//BaundStudioEngine_Sources_Project_ProjectManager_hpp
