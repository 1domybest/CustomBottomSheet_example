/*
 * Baund Studio Engine
 *
 * MediaFileManager.cpp
 * v1.0.3
 *
 * mega.winter0424@gmail.com
 */

#include "MediaFileManager.hpp"
#include "Sources/Support/iOSAudioFileSupport.hpp"
#include "Sources/Support/AndroidAudioFileSupport.hpp"

#if defined(__APPLE__)
namespace Baund::StudioEngine {
    auto AudioFile::getSampleRate() const -> SampleRate {
        // 지역변수 선언.
        auto audioFile  = (ExtAudioFileRef) nullptr;
        auto sampleRate = (Support::SampleRate) {};
        // 오디오파일 열기.
        if (not Support::open(audioFile,getPath())) {
            logManager.add({Log::error,"AudioFile","failed to open audio file"});
            audioFile = nullptr;
            goto error;
        }
        // 샘플레이트 가져오기.
        if (not Support::get(audioFile,sampleRate)) {
            logManager.add({Log::error,"AudioFile","failed to get sample rate"});
            goto error;
        }
        // 결과 반환.
    success:
        Support::close(audioFile);
        return sampleRate.value;
    error:
        Support::close(audioFile);
        return 0.0;
    }
    auto AudioFile::getFrameSize() const -> Frames {
        // 지역변수 선언.
        auto audioFile = (ExtAudioFileRef) nullptr;
        auto numFrames = (Support::NumFrames) {};
        // 오디오파일 열기.
        if (not Support::open(audioFile,getPath())) {
            logManager.add({Log::error,"AudioFile","failed to open audio file"});
            audioFile = nullptr;
            goto error;
        }
        // 프레임의 수 가져오기.
        if (not Support::get(audioFile,numFrames)) {
            logManager.add({Log::error,"AudioFile","failed to get frame size"});
            goto error;
        }
        // 결과 반환.
    success:
        Support::close(audioFile);
        return numFrames.value;
    error:
        Support::close(audioFile);
        return 0;
    }
    auto AudioFile::getChannelSize() const -> Size {
        // 지역변수 선언.
        auto audioFile   = (ExtAudioFileRef) nullptr;
        auto numChannels = (Support::NumChannels) {};
        // 오디오파일 열기.
        if (not Support::open(audioFile,getPath())) {
            logManager.add({Log::error,"AudioFile","failed to open audio file"});
            audioFile = nullptr;
            goto error;
        }
        // 체널의 수 가져오기.
        if (not Support::get(audioFile,numChannels)) {
            logManager.add({Log::error,"AudioFile","failed to get channel size"});
            goto error;
        }
        // 결과 반환.
    success:
        Support::close(audioFile);
        return numChannels.value;
    error:
        Support::close(audioFile);
        return 0;
    }
    auto AudioFile::readTo(AudioBuffer& buffer) const -> bool {
        // 지역변수 선언.
        auto pathToRead  = this->getPath();
        auto audioFile   = (ExtAudioFileRef) nullptr;
        auto sampleRate  = (Support::SampleRate) {};
        auto numChannels = (Support::NumChannels) {};
        auto numFrames   = (Support::NumFrames) {};
        auto outSampleRate = (Float64) 0.0;
        auto outFrames     = (SInt64) 0;
        auto outChannels   = (UInt32) 0;
        // MP4파일 변환.
        if (Support::isMP4Video(pathToRead)) {
            auto tempFile = pathToRead.substr(pathToRead.find_last_of('/')+1);
            auto tempName = tempFile.substr(0,tempFile.find_last_of('.'));
            auto tempPath = (getTempDirRef().size() > 0 ? getTempDirRef() : "") + tempName + ".m4a";
            if (not Support::extractM4A(pathToRead,tempPath)) {
                logManager.add({Log::error,"AudioFile","failed to extract m4a from mp4"});
                goto error;
            }
            pathToRead = tempPath;
        }
        // 오디오파일 열기.
        if (not Support::open(audioFile,pathToRead)) {
            logManager.add({Log::error,"AudioFile","failed to open audio file"});
            audioFile = nullptr;
            goto error;
        }
        // 샘플레이트 가져오기.
        if (not Support::get(audioFile,sampleRate)) {
            logManager.add({Log::error,"AudioFile","failed to get sample rate"});
            goto error;
        }
        // 프레임의 수 가져오기.
        if (not Support::get(audioFile,numFrames)) {
            logManager.add({Log::error,"AudioFile","failed to get frame size"});
            goto error;
        }
        // 체널의 수 가져오기.
        if (not Support::get(audioFile,numChannels)) {
            logManager.add({Log::error,"AudioFile","failed to get channel size"});
            goto error;
        }
        // 오디오버퍼 설정.
        if(buffer.getSampleRate() != sampleRate.value) {
            outSampleRate = buffer.getSampleRate();
            outFrames     = (SInt64)std::ceil(numFrames.value * (buffer.getSampleRate()/sampleRate.value));
        } else {
            outSampleRate = sampleRate.value;
            outFrames     = numFrames.value;
        }
        if (buffer.getChannelSize() != numChannels.value) {
            outChannels = (UInt32)buffer.getChannelSize();
        } else {
            outChannels = numChannels.value;
        }
        buffer = AudioBuffer({.sampleRate = outSampleRate,.frameSize = outFrames,.channelSize = outChannels});
        // 오디오파일 읽기.
        if (not Support::read(audioFile,{outSampleRate},{outChannels},{outFrames},buffer.getData())) {
            logManager.add({Log::error,"AudioFile","failed to read audio file"});
            goto error;
        }
        // 결과 반환.
    success:
        Support::close(audioFile);
        return true;
    error:
        Support::close(audioFile);
        return false;
    }
    auto AudioFile::write(AudioBuffer const& buffer) const -> bool {
        // 경로 가져오기.
        auto pathToWrite = this->getPath();
        // 오디오파일 쓰기.
        
        auto fileType = (m.format == Format::AAC) ? kAudioFileM4AType : kAudioFileWAVEType;
        if (not Support::write(pathToWrite,
                               fileType,
                               {buffer.getSampleRate()},
                               {(UInt32)buffer.getChannelSize()},
                               {buffer.getFrameSize()},
                               buffer.getData())) {
            logManager.add({Log::error,"AudioFile","failed to write audio file"});
            goto error;
        }
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    
    struct AudioFileWriter::Impl {
        Support::RTRecorder recorder;
    };
    AudioFileWriter::AudioFileWriter()
    : m({.impl = new Impl}) {
    }
    AudioFileWriter::~AudioFileWriter() {
        if (m.impl) {
            auto temp = this->getPath();
            Support::close(m.impl->recorder);
            delete m.impl;
        }
    }
    auto AudioFileWriter::ready() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl) m.impl = new Impl;
            if (m.isStart)  return;
            if (not m.isReady) {
                if (Support::ready<AudioBuffer::SampleValue>(m.impl->recorder,
                                                             m.path,
                                                             deviceManager.getAudioDevice().getSampleRate(),
                                                             deviceManager.getAudioDevice().getChannelSize())) {
                    m.isReady = true;
                }
            } else return;
        });
    }
    auto AudioFileWriter::start() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl)    return;
            if (not m.isReady) return;
            if (not m.isStart) {
                if (Support::start(m.impl->recorder))
                    m.isStart = true;
            } else return;
        });
    }
    auto AudioFileWriter::stop() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl)    return;
            if (not m.isReady) return;
            if (m.isStart) {
                if (Support::stop(m.impl->recorder))
                    m.isStart = false;
            } else return;
        });
    }
    auto AudioFileWriter::close() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl) return;
            if (m.isStart)  return;
            if (m.isReady) {
                Support::close(m.impl->recorder);
                m.isReady = false;
            }
        });
    }
    auto AudioFileWriter::write(AudioBuffer const& in) -> void {
        n.taskQueue.enqueue([this,in = in] {
            if (not m.impl)    return;
            if (not m.isReady) return;
            if (not m.isStart) return;
            Support::writeAACFile(m.impl->recorder,in.getData(),in.getFrameSize(),in.getChannelSize());
        });
    }
}
#elif defined(__ANDROID__)
namespace Baund::StudioEngine {
    auto AudioFile::getSampleRate() const -> SampleRate {
        return Support::getSampleRate(getPath());
    }
    auto AudioFile::getFrameSize() const -> Size {
        return Support::getFrameSize(getPath());
    }
    auto AudioFile::getChannelSize() const -> Size {
        return Support::getChannelSize(getPath());
    }
    auto AudioFile::readTo(AudioBuffer& input) const -> bool {
        auto sampleRate  = (bool)input.getSampleRate() ? input.getSampleRate() : this->getSampleRate();
        auto channelSize = (int32_t)input.getChannelSize() ? input.getChannelSize() : getChannelSize();
        auto readBuffer  = Support::readAudioFile(getPath(),(int)sampleRate);
        input = AudioBuffer({
            .sampleRate  = sampleRate,
            .frameSize   = (Size)readBuffer.size() / channelSize,
            .channelSize = channelSize
        });
        memcpy(input.getData(),readBuffer.data(),readBuffer.size()*sizeof(AudioBuffer::SampleValue));
        return true;
    }
    auto AudioFile::write(AudioBuffer const& input) const -> bool {
        auto format = m.format == Format::AAC ? Support::AAC : Support::PCM;
        auto buffer = std::vector<float>(input.getDataSize());
        memcpy(buffer.data(),input.getData(),input.getDataSize()*sizeof(float));
        writeAudioFile(getPath(),buffer,format,(int)input.getSampleRate(),input.getChannelSize());
        return true;
    }

    struct AudioFileWriter::Impl {
        Support::RTRecorder recorder;
    };
    AudioFileWriter::AudioFileWriter()
    : m { .impl = new Impl } {
    }
    AudioFileWriter::~AudioFileWriter() {
        if (m.impl) {
            auto temp = this->getPath();
            Support::close(m.impl->recorder);
            delete m.impl;
        }
    }
    auto AudioFileWriter::ready() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl) m.impl = new Impl;
            if (m.isStart)  return;
            if (not m.isReady) {
                if (Support::ready(m.impl->recorder,
                                   m.path,
                                   deviceManager.getAudioDevice().getSampleRate(),
                                   deviceManager.getAudioDevice().getChannelSize())) {
                    m.isReady = true;
                }
            } else return;
        });
    }
    auto AudioFileWriter::start() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl)    return;
            if (not m.isReady) return;
            if (not m.isStart) {
                if (Support::start(m.impl->recorder))
                    m.isStart = true;
            } else return;
        });
    }
    auto AudioFileWriter::stop() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl)    return;
            if (not m.isReady) return;
            if (m.isStart) {
                if (Support::stop(m.impl->recorder))
                    m.isStart = false;
            } else return;
        });
    }
    auto AudioFileWriter::close() -> void {
        n.taskQueue.enqueue([this] {
            if (not m.impl) return;
            if (m.isStart)  return;
            if (m.isReady) {
                Support::close(m.impl->recorder);
                m.isReady = false;
            }
        });
    }
    auto AudioFileWriter::write(AudioBuffer const& in) -> void {
        n.taskQueue.enqueue([this,in = in] {
            if (not m.impl)    return;
            if (not m.isReady) return;
            if (not m.isStart) return;
            auto tempBuffer = std::vector<float>(in.getDataSize());
            std::copy(in.getData(),in.getData()+in.getDataSize(),tempBuffer.begin());
            auto buffer = Support::convertTo16BitAudio(tempBuffer);
            Support::writeAACFile(m.impl->recorder,buffer);
        });
    }
}

#endif
