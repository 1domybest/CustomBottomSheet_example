/*
 * Baund Studio Engine
 *
 * ProjectManager.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "ProjectManager.hpp"

namespace Baund::StudioEngine {
    ProjectManager projectManager = { };
    
    auto Project::ioCallback(AudioBuffer const& in) -> void {
        // 모니터링버퍼 설정.
        m.monitorBufferRef = &in;
        // 지역변수 선언.
        auto  position = m.transport.getPosition();
        auto  isPlay   = m.transport.getPlayState();
        auto  isRecord = m.transport.getRecordState();
        auto& buffer   = m.routingManager.getOutputMasterRouting()->getAudioBuffer();

        if (isPlay and isRecord) {
            if (not n.recorder.isStart()) {
                auto  now          = std::chrono::system_clock::now();
                auto  time         = std::chrono::system_clock::to_time_t(now);
                auto& tempDir      = m.fileManager.getTempDir();
                auto  fileName     = String("record_") + std::to_string(time) + ".m4a";
                auto  filePath     = tempDir + fileName;
                n.recorder.setPath(filePath);
                n.recorder.ready();
                n.recorder.start();
            }
            n.recorder.write(in);
        } 
        // 오디오데이터 처리.
        if (m.connectionToIO) {
            m.routingManager.checkBufferSize();
            m.routingManager.silenceAllRoutings();
        }
        if (m.connectionToIO) {
            m.trackManager.writeToAllOutput(position,isPlay);
        }
        if (m.connectionToIO) {
            m.mixer.writeToAllRouting();
        }
        // 모니터링버퍼 해제.
        m.monitorBufferRef = nullptr;
        // 입출력버퍼에 쓰기.
        if (in.getSampleRate()  == buffer.getSampleRate() and
            in.getFrameSize()   == buffer.getFrameSize()  and
            in.getChannelSize() == buffer.getChannelSize()) {
            for (auto i=0; i<buffer.getDataSize(); i++) {
                if (buffer.getData()[i] >  1.0) buffer.getData()[i] =  1.0;
                if (buffer.getData()[i] < -1.0) buffer.getData()[i] = -1.0;
            }
            std::copy(buffer.getData(), buffer.getData()+buffer.getDataSize(), in.getData());
        } else {
            in.silence();
        }
        // 재생위치 이동.
        if (isPlay) {
            m.transport.setPosition(position+in.getFrameSize());
        }
    }
    auto Project::errorCallback() -> void {
        // 오디오기기 가져오기.
        auto& audioDevice = deviceManager.getAudioDevice();
        if (not audioDevice.isValid()) {
            logManager.add({Log::error,"Project::errorCallback","audio device not ready"});
            return;
        }
        // 샘플레이트 확인 및 적용.
        auto oldSampleRate = m.routingManager.getOutputMasterRouting()->getAudioBuffer().getSampleRate();
        auto newSampleRate = audioDevice.getSampleRate();
        if (oldSampleRate != newSampleRate) {
            m.fileManager.refreshAllAudioFile();
            m.recordingData.clear();
            m.recordingItem.reset();
            auto before = m.transport.getPosition();
            auto after  = std::ceil(before * (newSampleRate/oldSampleRate));
            m.transport.setPlayState(false);
            m.transport.setPosition (after);
        }
    }
    auto Project::audioMixdown() -> AudioFile {
        // 오디오기기 가져오기.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"Project::audioMixdown","audio device not ready"});
            return { };
        }
        auto isRunning = device.isRunning();
        device.stop();
        m.monitorBufferRef = nullptr;
        // 버퍼 생성.
        auto bufferSize = device.getBufferSize();
        auto length     = getLength();
        auto frameSize  = length - (length % bufferSize) + bufferSize;
        auto mixBuffer  = AudioBuffer({
            .sampleRate  = device.getSampleRate(),
            .frameSize   = frameSize,
            .channelSize = device.getChannelSize()
        });
        // 오디오 읽기.
        for (auto readFrame=0; readFrame<length;) {
            m.routingManager.silenceAllRoutings();
            m.trackManager.writeToAllOutput(readFrame,true);
            m.mixer.writeToAllRouting();
            auto& buffer = m.routingManager.getOutputMasterRouting()->getAudioBuffer();
            for (auto i=0; i<buffer.getFrameSize(); i++) {
                for (auto c=0; c<buffer.getChannelSize(); c++) {
                    mixBuffer[readFrame+i][c] = buffer[i][c];
                }
            }
            readFrame += buffer.getFrameSize();
        }
        // 오디오파일 저장.
        auto  now          = std::chrono::system_clock::now();
        auto  time         = std::chrono::system_clock::to_time_t(now);
        auto& tempDir      = m.fileManager.getTempDir();
        auto  fileName     = String("mixdown_") + std::to_string(time) + ".wav";
        auto  filePath     = tempDir + fileName;
        auto  audioFile    = AudioFile(std::move(filePath));
        audioFile.setWriteFormat(AudioFile::Format::PCM);
        audioFile.setTempDirRef (tempDir);
        audioFile.write(mixBuffer);
        
        if (isRunning) device.start();
        return audioFile;
    }
}
