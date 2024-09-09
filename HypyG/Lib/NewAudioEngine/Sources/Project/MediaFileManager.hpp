/*
 * Baund Studio Engine
 *
 * MediaFileManager.hpp
 * v1.0.3
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_MediaFileManager_hpp
#define BaundStudioEngine_Sources_Project_MediaFileManager_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine {
    class MediaFile: public virtual BaseObject {
    public:
        using Path = String;

        virtual ~MediaFile() override = default;
        MediaFile()                   = default;
        MediaFile(MediaFile const&)   = default;
        MediaFile(MediaFile     &&);
        MediaFile(Path      const&);

        auto operator=(MediaFile const&) -> MediaFile& = default;
        auto operator=(MediaFile     &&) -> MediaFile&;

        virtual void reset  ()       override = 0;
        virtual bool isValid() const override = 0;

        auto setPath(Path const&) -> void;
        auto getPath() const -> Path const&;
        
        auto operator==(MediaFile const&) const -> bool;
        auto operator!=(MediaFile const&) const -> bool;
        auto operator>=(MediaFile const&) const -> bool;
        auto operator<=(MediaFile const&) const -> bool;
        auto operator> (MediaFile const&) const -> bool;
        auto operator< (MediaFile const&) const -> bool;

    private:
        struct MemberVariables {
            Path path = "";
        } m;
    };
    class AudioFile final: public MediaFile {
    public:
        enum class Format { AAC, PCM };
        using SampleRate = AudioBuffer::SampleRate;
        using Frames     = AudioBuffer::Frames;
        using Channels   = AudioBuffer::Channels;

        ~AudioFile() override       = default;
        AudioFile()                 = default;
        AudioFile(AudioFile const&) = default;
        AudioFile(AudioFile     &&);
        AudioFile(Path      const&);

        auto operator=(AudioFile const&) -> AudioFile& = default;
        auto operator=(AudioFile     &&) -> AudioFile&;

        void reset  ()       override;
        bool isValid() const override;
        
        auto setWriteFormat(Format const&) -> void;
        auto getWriteFormat() const -> Format;

        auto setTempDirRef(Path const&) -> void;
        auto getTempDirRef() const -> Path;

        auto getSampleRate () const -> SampleRate;
        auto getFrameSize  () const -> Frames;
        auto getChannelSize() const -> Channels;

        auto readTo(AudioBuffer      &) const -> bool;
        auto write (AudioBuffer const&) const -> bool;

    private:
        struct MemberVariables {
            using DirRef = Path const*;
            Format format     = Format::AAC;
            DirRef tempDirRef = nullptr;
        } m;
    };
    class AudioFileWriter final
    : public virtual BaseObject {
    public:
        struct Impl;

        ~AudioFileWriter() override;
        AudioFileWriter();
        AudioFileWriter(AudioFileWriter const&) = delete;
        AudioFileWriter(AudioFileWriter     &&);
        AudioFileWriter(AudioFile::Path const&);

        auto operator=(AudioFileWriter const&) -> AudioFileWriter& = delete;
        auto operator=(AudioFileWriter     &&) -> AudioFileWriter&;

        void reset  ()       override;
        bool isValid() const override;
        
        auto isReady() const -> bool;
        auto isStart() const -> bool;
        auto ready() -> void;
        auto start() -> void;
        auto stop () -> void;
        auto close() -> void;
        
        auto setPath(AudioFile::Path const&) -> void;
        auto getPath() const -> AudioFile::Path;
        auto write(AudioBuffer const& in) -> void;
        
    private:
        struct NonCopyables {
            mutable TaskQueue taskQueue;
        } n;
        struct MemberVariables {
            AudioFile::Path path;
            Impl*           impl    = nullptr;
            bool            isReady = false;
            bool            isStart = false;
        } m;
    };
    class MediaFileManager final: public virtual BaseObject {
    public:
        struct AudioFileBuffer {
            AudioFile   audioFile;
            AudioBuffer buffer;
        };
        using AudioFileList     = List<AudioFileBuffer>;
        using AudioFileAccessor = AudioFileList::Accessor;
        using Path              = MediaFile::Path;

        ~MediaFileManager() override              = default;
        MediaFileManager()                        = default;
        MediaFileManager(MediaFileManager const&) = default;
        MediaFileManager(MediaFileManager     &&);
        
        auto operator=(MediaFileManager const&) -> MediaFileManager& = default;
        auto operator=(MediaFileManager     &&) -> MediaFileManager&;

        void reset  ()       override;
        bool isValid() const override;
        
        auto getAudioFileList()       -> AudioFileList&;
        auto getAudioFileList() const -> AudioFileList const&;

        auto add(AudioFile const&, AudioBuffer&&) -> AudioFileAccessor;
        auto add(AudioFile     &&, AudioBuffer&&) -> AudioFileAccessor;
        auto add(AudioFile                const&) -> AudioFileAccessor;
        auto add(AudioFile                    &&) -> AudioFileAccessor;
        auto sub(AudioFileAccessor        const&) -> void;

        auto setTempDir(Path const&) -> void;
        auto getTempDir() const -> Path const&;

        auto refreshAllAudioFile() -> void;
        auto refresh(AudioFileAccessor const&) -> void;
        
    private:
        struct MemberVariables {
            AudioFileList audioFileList;
            Path          tempDir = "";
        } m;
    };

    inline MediaFile::MediaFile(MediaFile&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline MediaFile::MediaFile(Path const& in)
    : m({.path = in}) {
    }
    inline auto MediaFile::operator=(MediaFile&& in) -> MediaFile& {
        if(this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void MediaFile::reset() {
        m = MemberVariables();
    }
    inline bool MediaFile::isValid() const {
        auto file   = std::ifstream(m.path);
        auto result = file.good();
        file.close();
        return result;
    }
    inline auto MediaFile::setPath(Path const& in) -> void {
        m.path = in;
    }
    inline auto MediaFile::getPath() const -> Path const& {
        return m.path;
    }
    inline auto MediaFile::operator==(MediaFile const& in) const -> bool {
        return this->m.path == in.m.path;
    }
    inline auto MediaFile::operator!=(MediaFile const& in) const -> bool {
        return this->m.path != in.m.path;
    }
    inline auto MediaFile::operator>=(MediaFile const& in) const -> bool {
        return this->m.path >= in.m.path;
    }
    inline auto MediaFile::operator<=(MediaFile const& in) const -> bool {
        return this->m.path <= in.m.path;
    }
    inline auto MediaFile::operator> (MediaFile const& in) const -> bool {
        return this->m.path >  in.m.path;
    }
    inline auto MediaFile::operator< (MediaFile const& in) const -> bool {
        return this->m.path <  in.m.path;
    }

    inline AudioFile::AudioFile(AudioFile&& in)
    : MediaFile(std::move(in))
    , m        (std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline AudioFile::AudioFile(Path const& in)
    : MediaFile(in) {
    }
    inline auto AudioFile::operator=(AudioFile&& in) -> AudioFile& {
        if (this != &in) {
            this->MediaFile::operator=(std::move(in));
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void AudioFile::reset() {
        m = MemberVariables();
        MediaFile::reset();
    }
    inline bool AudioFile::isValid() const {
        return MediaFile::isValid();
    }
    inline auto AudioFile::setWriteFormat(Format const& in) -> void {
        m.format = in;
    }
    inline auto AudioFile::getWriteFormat() const -> Format {
        return m.format;
    }
    inline auto AudioFile::setTempDirRef(Path const& in) -> void {
        m.tempDirRef = &in;
    }
    inline auto AudioFile::getTempDirRef() const -> Path {
        return (m.tempDirRef != nullptr) ? *(m.tempDirRef) : "";
    }

    inline AudioFileWriter::AudioFileWriter(AudioFileWriter&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline AudioFileWriter::AudioFileWriter(AudioFile::Path const& in)
    : m({.path = in}) {
    }
    inline auto AudioFileWriter::operator=(AudioFileWriter&& in) -> AudioFileWriter& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void AudioFileWriter::reset() {
        this->m = MemberVariables();
    }
    inline bool AudioFileWriter::isValid() const {
        return m.impl;
    }
    inline auto AudioFileWriter::isReady() const -> bool {
        return m.isReady;
    }
    inline auto AudioFileWriter::isStart() const -> bool {
        return m.isStart;
    }
    inline auto AudioFileWriter::setPath(AudioFile::Path const& in) -> void {
        n.taskQueue.enqueue([this,in] {
            m.path = in;
        });
    }
    inline auto AudioFileWriter::getPath() const -> AudioFile::Path {
        auto isFinish = std::shared_ptr<bool>(new bool(false));
        n.taskQueue.enqueue([this,isFinish] {
            *isFinish = true;
        });
        while (not *isFinish) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return m.path;
    }
    
    inline MediaFileManager::MediaFileManager(MediaFileManager&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto MediaFileManager::operator=(MediaFileManager&& in) -> MediaFileManager& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void MediaFileManager::reset() {
        m = MemberVariables();
    }
    inline bool MediaFileManager::isValid() const {
        return m.audioFileList.isValid();
    }
    inline auto MediaFileManager::getAudioFileList() -> AudioFileList& {
        return m.audioFileList;
    }
    inline auto MediaFileManager::getAudioFileList() const -> AudioFileList const& {
        return m.audioFileList;
    }
    inline auto MediaFileManager::add(AudioFile const& audioFile, AudioBuffer&& buffer) -> AudioFileAccessor {
        auto lock = this->getLock();
        return m.audioFileList.add({audioFile,std::move(buffer)});
    }
    inline auto MediaFileManager::add(AudioFile&& audioFile, AudioBuffer&& buffer) -> AudioFileAccessor {
        auto lock = this->getLock();
        return m.audioFileList.add({std::move(audioFile),std::move(buffer)});
    }
    inline auto MediaFileManager::add(AudioFile const& audioFile) -> AudioFileAccessor {
        auto file = audioFile;
        return add(std::move(file));
    }
    inline auto MediaFileManager::add(AudioFile&& audioFile) -> AudioFileAccessor {
        auto lock = this->getLock();
        auto& audioDevice = deviceManager.getAudioDevice();
        if (not audioDevice.isValid()) return { };
        for (auto file : m.audioFileList) {
            if (audioFile == file->audioFile) return file;
        }
        auto buffer = AudioBuffer({
            .sampleRate  = deviceManager.getAudioDevice().getSampleRate(),
            .frameSize   = 0,
            .channelSize = deviceManager.getAudioDevice().getChannelSize()
        });
        audioFile.setTempDirRef(m.tempDir);
        audioFile.readTo(buffer);
        return m.audioFileList.add({std::move(audioFile),std::move(buffer)});
    }
    inline auto MediaFileManager::sub(AudioFileAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) {
            in->audioFile.reset();
            in->buffer   .reset();
            m.audioFileList.sub(in);
        }
    }
    inline auto MediaFileManager::setTempDir(Path const& in) -> void {
        m.tempDir = in;
        for (auto accessor : m.audioFileList) {
            accessor->audioFile.setTempDirRef(m.tempDir);
        }
    }
    inline auto MediaFileManager::getTempDir() const -> Path const& {
        return m.tempDir;
    }
    inline auto MediaFileManager::refresh(AudioFileAccessor const& in) -> void {
        if (not in.isValid()) return;
        auto& audioDevice = deviceManager.getAudioDevice();
        if (not audioDevice.isValid()) return;
        auto buffer = AudioBuffer({
            .sampleRate  = deviceManager.getAudioDevice().getSampleRate(),
            .frameSize   = 0,
            .channelSize = deviceManager.getAudioDevice().getChannelSize()
        });
        if (not in->audioFile.readTo(buffer)) return;
        else in->buffer = std::move(buffer);
    }
    inline auto MediaFileManager::refreshAllAudioFile() -> void {
        auto lock = this->getLock();
        for (auto accessor : m.audioFileList)
            refresh(accessor);
    }
}

#endif//BaundStudioEngine_Sources_Project_MediaFileManager_hpp
