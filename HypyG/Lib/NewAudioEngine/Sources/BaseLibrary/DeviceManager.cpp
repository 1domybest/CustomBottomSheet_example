/*
 * Baund Studio Engine
 *
 * DeviceManager.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "DeviceManager.hpp"
#include "Sources/Support/iOSAudioDeviceSupport.hpp"
#include "Sources/Support/AndroidAudioDeviceSupport.hpp"

namespace Baund::StudioEngine {
    DeviceManager deviceManager = { };
}
#if defined(__APPLE__)
namespace Baund::StudioEngine {
    struct AudioDevice::StreamData {
        static constexpr auto defaultFrameSize = 256;
        AudioUnit   audioUnit  = nullptr;
        bool        errorState = false;
        AudioBuffer ioBuffer;
        static OSStatus ioCallback(void*                       inRefCon,
                                   AudioUnitRenderActionFlags* ioActionFlags,
                                   const AudioTimeStamp*       inTimeStamp,
                                   UInt32                      inBusNumber,
                                   UInt32                      inNumberFrames,
                                   AudioBufferList*            ioData) {
            auto  device   = static_cast<AudioDevice*>(inRefCon);
            auto  stream   = device->m.streamData;
            auto& ioBuffer = stream->ioBuffer;
            // 입출력버퍼 초기화 또는 재할당.
            if (ioBuffer.getFrameSize() != inNumberFrames) {
                device->m.bufferSize = inNumberFrames;
                ioBuffer = AudioBuffer({
                    .sampleRate  = device->m.sampleRate,
                    .frameSize   = device->m.bufferSize,
                    .channelSize = device->m.numChannels
                });
                logManager.add({Log::debug,"AudioDevice.ioCallback","set new io buffer size",device->m.bufferSize});
            }
            // 기기버퍼 설정.
            ioData->mNumberBuffers = 1;
            ioData->mBuffers[0].mNumberChannels = (UInt32)ioBuffer.getChannelSize();
            ioData->mBuffers[0].mDataByteSize   = (UInt32)ioBuffer.getDataSize() * sizeof(AudioBuffer::SampleValue);
            ioData->mBuffers[0].mData           = ioBuffer.getData();
            // 오디오입력 가져오기.
            if (AudioUnitRender(stream->audioUnit,ioActionFlags,inTimeStamp,1,inNumberFrames,ioData) != noErr) {
                logManager.add({Log::error,"AudioDevice.ioCallback","failed to get audio input"});
                ioBuffer.silence();
            }
            // 입출력콜백 호출.
            if (device->m.ioCallback) device->m.ioCallback(ioBuffer);
            else ioBuffer.silence();
            // 콜백 닫기.
            return noErr;
        }
        static void restart(AudioDevice* device) {
            auto lock   = device->getLock();
            auto stream = device->m.streamData;
            // UInt32 bufferSize;
            if (not Support::stop(stream->audioUnit)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to stop audio unit"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","stop"});
            }
            if (not Support::getDeviceSampleRate(stream->audioUnit, device->m.sampleRate)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to get device sample rate"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","get new sample rate"});
            }
            if (not Support::setStreamFormat(stream->audioUnit, device->m.sampleRate)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to set stream format"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","set io stream data"});
            }
            if (not Support::setBufferSize(StreamData::defaultFrameSize)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to set buffer size"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","set new buffer size"});
            }
            //if (not Support::runPreCallback(stream->audioUnit, bufferSize)) {
            //    logManager.add({Log::error,"AudioDevice.restart","failed to run pre callback"});
            //    goto error;
            //} else {
            //    logManager.add({Log::debug,"AudioDevice.restart","run pre callback"});
            //    device->m.bufferSize = bufferSize;
            //}
            //if (stream->ioBuffer.getFrameSize() != bufferSize) {
            //    stream->ioBuffer = AudioBuffer();
            //}
            if (device->m.errorCallback) device->m.errorCallback();
            if (not Support::start(stream->audioUnit)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to start audio unit"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","new sample rate",device->m.sampleRate});
                logManager.add({Log::debug,"AudioDevice.restart","new buffer size",device->m.bufferSize});
            }
        success:
            stream->errorState = false;
            return;
        error:
            stream->errorState = false;
            //device->close();
            return;
        }
        static void errorCallback(void*               userData,
                                  AudioUnit           ci,
                                  AudioUnitPropertyID propertyID,
                                  AudioUnitScope      scope,
                                  AudioUnitElement    element) {
            auto device   = static_cast<AudioDevice*>(userData);
            auto stream   = device->m.streamData;
            auto format   = (AudioStreamBasicDescription) {};
            auto propSize = (UInt32) sizeof(format);
            AudioUnitGetProperty(ci,
                                 kAudioUnitProperty_StreamFormat,
                                 kAudioUnitScope_Output,
                                 0, // output bus
                                 &format,
                                 &propSize);
            if (format.mSampleRate != device->m.sampleRate and stream->errorState == false) {
                stream->errorState = true;
                std::thread th(StreamData::restart,device);
                th.detach();
            }
        }
    };
    bool AudioDevice::isValid() const {
        return m.streamData and m.streamData->audioUnit;
    }
    auto AudioDevice::isRunning() const -> bool {
        if (m.streamData and m.streamData->audioUnit) {
            return Support::isRunning(m.streamData->audioUnit);
        } else return false;
    }
    auto AudioDevice::ready() -> void {
        // auto lock = this->getLock();
        if (isRunning()) this->stop();
        if (m.streamData) return;
        m.streamData = new StreamData;
        UInt32 bufferSize;
        if (not Support::open(m.streamData->audioUnit)) {
            m.streamData->audioUnit = nullptr;
            logManager.add({Log::error,"AudioDevice.ready","failed to open audio unit"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","open audio unit",m.streamData->audioUnit});
        }
        if (not Support::enableIO(m.streamData->audioUnit)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to enable IO"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","enable IO"});
        }
        if (not Support::init(m.streamData->audioUnit)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to init audio unit"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","init audio unit"});
        }
        if (not Support::getDeviceSampleRate(m.streamData->audioUnit,m.sampleRate)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to get device sample rate"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","device sample rate"});
        }
        if (not Support::setStreamFormat(m.streamData->audioUnit,m.sampleRate)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to set stream format"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","set stream format"});
        }
        if (not Support::setBufferSize(StreamData::defaultFrameSize)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to set buffer size"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","set buffer size"});
        }
        if (not Support::runPreCallback(m.streamData->audioUnit,bufferSize)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to run pre callback"});
            goto error;
        } else {
            m.bufferSize = bufferSize;
            logManager.add({Log::debug,"AudioDevice.ready","sample rate", m.sampleRate});
            logManager.add({Log::debug,"AudioDevice.ready","buffer size", m.bufferSize});
        }
        if (not Support::setIOCallback(m.streamData->audioUnit,StreamData::ioCallback,this)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to set IO callback"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","set io callback"});
        }
        if (not Support::setErrorCallback(m.streamData->audioUnit,StreamData::errorCallback,this)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to set error callback"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","set error callback"});
        }
    success:
        m.numChannels = AudioBuffer::stereo;
        return;
    error:
        this->close();
        return;
    }
    auto AudioDevice::close() -> void {
        // auto lock = this->getLock();
        if (m.streamData) {
            if (isRunning()) { // 오디오 디바이스 정지.
                if (not Support::stop(m.streamData->audioUnit)) {
                    logManager.add({Log::error,"AudioDevice.close","invalid audio unit"});
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    logManager.add({Log::debug,"AudioDevice.close","stop audio unit"});
                }
            }
            // 오디오 유닛 닫기.
            if (not Support::close(m.streamData->audioUnit)) {
                logManager.add({Log::error,"AudioDevice.close","failed to close audio unit"});
            } else {
                logManager.add({Log::debug,"AudioDevice.close","close audio unit"});
            }
            // 스트림 데이터 해제.
            delete m.streamData;
            logManager.add({Log::debug,"AudioDevice.close","delete stream data",m.streamData});
            m.streamData = nullptr;
        } else {
            logManager.add({Log::debug,"AudioDevice.close","stream data is null"});
        }
        m.sampleRate  = 0.0;
        m.bufferSize  = 0.0;
        m.numChannels = 0;
    }
    auto AudioDevice::start() -> void {
        // auto lock = this->getLock();
        if (not m.streamData or not m.streamData->audioUnit) {
            logManager.add({Log::error,"AudioDevice.start","not ready stream data or audio unit"});
            goto error;
        }
        if (not isRunning()) {
            if (not Support::start(m.streamData->audioUnit)) {
                logManager.add({Log::error,"AudioDevice.start","failed to start audio unit"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.start","start audio device"});
            }
        }
    success:
        return;
    error:
        return;
    }
    auto AudioDevice::stop() -> void {
        // auto lock = this->getLock();
        if (not m.streamData or not m.streamData->audioUnit) {
            logManager.add({Log::debug,"AudioDevice.stop","not ready stream data or audio unit"});
            goto error;
        }
        if (isRunning()) {
            if (not Support::stop(m.streamData->audioUnit)) {
                logManager.add({Log::error,"AudioDevice.stop","failed to stop audio unit"});
                goto error;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            logManager.add({Log::debug,"AudioDevice.stop","stop audio device"});
        }
    success:
        return;
    error:
        return;
    }
}
#elif defined(__ANDROID__)
namespace Baund::StudioEngine {
    struct AudioDevice::StreamData {
        static constexpr auto defaultFrameSize = 256;
        AAudioStreamBuilder* builder    = nullptr;
        AAudioStream*        iStream    = nullptr;
        AAudioStream*        oStream    = nullptr;
        bool                 errorState = false;
        AudioBuffer          ioBuffer;
        static aaudio_data_callback_result_t ioCallback(AAudioStream *oStream,
                                                        void *userData,
                                                        void *audioData,
                                                        int32_t numFrames) {
            auto  device   = static_cast<AudioDevice *>(userData);
            auto  stream   = device->m.streamData;
            auto& ioBuffer = stream->ioBuffer;
            // 레이턴시 확인.
            auto iBufferSize = AAudioStream_getBufferSizeInFrames(stream->iStream);
            auto oBufferSize = AAudioStream_getBufferSizeInFrames(stream->oStream);
            auto iBurst = AAudioStream_getFramesPerBurst(stream->iStream);
            auto oBurst = AAudioStream_getFramesPerBurst(stream->oStream);
            auto correction = (iBufferSize+oBufferSize)+((iBurst+oBurst)*2);
            if (correction > 0 and correction != device->m.correction) {
                device->m.correction = correction;
            }
            // 입출력버퍼 초기화 또는 재할당.
            if (ioBuffer.getFrameSize() != numFrames) {
                device->m.bufferSize = numFrames;
                ioBuffer = AudioBuffer({
                    .sampleRate  = device->m.sampleRate,
                    .frameSize   = device->m.bufferSize,
                    .channelSize = device->m.numChannels
                });
                logManager.add({Log::debug,"AudioDevice.ioCallback","set new io buffer size",device->m.bufferSize});
            }
            if (AAudioStream_getState(oStream) == AAUDIO_STREAM_STATE_STARTED) {
                // 오디오입력 가져오기.
                AAudioStream_read(stream->iStream, ioBuffer.getData(), numFrames, 0);
                // 입출력콜백 호출.
                if (device->m.ioCallback) device->m.ioCallback(ioBuffer);
                else ioBuffer.silence();
                // 출력버퍼에 쓰기.
                memcpy(audioData, ioBuffer.getData(), numFrames * 2 * sizeof(AudioBuffer::SampleValue));
            } else {
                auto output = static_cast<AudioBuffer::SampleValue*>(audioData);
                std::fill(output,output+numFrames*device->m.numChannels,0.0);
            }
            return AAUDIO_CALLBACK_RESULT_CONTINUE;
        }
        static auto restart(AudioDevice *device) -> void {
            //auto lock   = device->getLock();
            auto stream = device->m.streamData;
            uint32_t bufferSize;
            if (not Support::stop(stream->oStream)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to stop output stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","stop output stream"});
            }
            if (not Support::stop(stream->iStream)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to stop input stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","stop input stream"});
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (not Support::close(stream->iStream)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to close input stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","close input stream"});
            }
            if (not Support::close(stream->oStream)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to close output stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","close output stream"});
            }
            if (not Support::close(stream->builder)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to close stream builder"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","close stream builder"});
            }
            if (not Support::open(stream->builder,StreamData::defaultFrameSize)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to open stream builder"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","open stream builder"});
            }
            if (not Support::open(stream->builder, stream->iStream, AAUDIO_DIRECTION_INPUT)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to open input stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","open input stream"});
            }
            if (not Support::runPreCallback(stream->builder, bufferSize)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to run pre callback"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","run pre callback"});
                device->m.bufferSize = bufferSize;
            }
            if (not Support::setIOCallback(stream->builder, StreamData::ioCallback, device)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to set io callback"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","set io callback"});
            }
            if (not Support::setErrorCallback(stream->builder, StreamData::errorCallback, device)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to set error callback"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","set error callback"});
            }
            if (not Support::open(stream->builder, stream->oStream, AAUDIO_DIRECTION_OUTPUT)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to open input stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","open input stream"});
            }
            if (not Support::getSampleRate(stream->oStream, device->m.sampleRate)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to get sample rate"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","get sample rate"});
            }
            if (stream->ioBuffer.getFrameSize() != bufferSize) {
                stream->ioBuffer = AudioBuffer();
            }
            if(device->m.errorCallback) device->m.errorCallback();
            if (not Support::start(stream->iStream)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to start input stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","start input stream"});
            }
            if (not Support::start(stream->oStream)) {
                logManager.add({Log::error,"AudioDevice.restart","failed to start output stream"});
                goto error;
            } else {
                logManager.add({Log::debug,"AudioDevice.restart","start output stream"});
            }
        success:
            stream->errorState = false;
            return;
        error:
            stream->errorState = false;
            //device->close();
            return;
        }
        static void errorCallback(AAudioStream *astream,
                                  void *userData,
                                  aaudio_result_t result) {
            auto device = static_cast<AudioDevice *>(userData);
            auto stream = device->m.streamData;
            if (stream->errorState == false) {
                stream->errorState = true;
                std::thread th(StreamData::restart, device);
                th.detach();
            }
        }
    };
    bool AudioDevice::isValid() const {
        return m.streamData and m.streamData->iStream and m.streamData->oStream;
    }
    auto AudioDevice::isRunning() const -> bool {
        if (m.streamData and m.streamData->iStream and m.streamData->oStream) {
            return Support::isRunning(m.streamData->oStream);
        } else return false;
    }
    auto AudioDevice::ready() -> void {
        //auto lock = this->getLock();
        if (isRunning()) this->stop();
        if (m.streamData) return;
        m.streamData = new StreamData;
        uint32_t bufferSize;
        if (not Support::open(m.streamData->builder,StreamData::defaultFrameSize)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to open stream builder"});
            m.streamData->builder = nullptr;
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","open stream builder"});
        }
        if (m.inputID > -1) {
            if (not Support::setID(m.streamData->builder,m.inputID)) {
                logManager.add({Log::error,"AudioDevice.setID","failed to set input device id"});
                goto error;
            }
        }
        if (not Support::open(m.streamData->builder,m.streamData->iStream,AAUDIO_DIRECTION_INPUT)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to open input stream"});
            m.streamData->iStream = nullptr;
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","open input stream"});
        }
        if (not Support::runPreCallback(m.streamData->builder,bufferSize)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to run pre callback"});
            goto error;
        } else {
            m.bufferSize = bufferSize;
            logManager.add({Log::debug,"AudioDevice.ready","buffer size", m.bufferSize});
        }
        if (not Support::setIOCallback(m.streamData->builder,StreamData::ioCallback,this)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to set io callback"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","set io callback"});
        }
        if (not Support::setErrorCallback(m.streamData->builder,StreamData::errorCallback,this)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to set error callback"});
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","set error callback"});
        }
        if (not Support::open(m.streamData->builder,m.streamData->oStream,AAUDIO_DIRECTION_OUTPUT)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to open output stream"});
            m.streamData->oStream = nullptr;
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","open output stream"});
        }
        if (m.outputID > -1) {
            if (not Support::setID(m.streamData->builder,m.outputID)) {
                logManager.add({Log::error,"AudioDevice.setID","failed to set output device id"});
                goto error;
            }
        }
        if (not Support::getSampleRate(m.streamData->oStream,m.sampleRate)) {
            logManager.add({Log::error,"AudioDevice.ready","failed to get sample rate"});
            m.sampleRate = 0.0;
            goto error;
        } else {
            logManager.add({Log::debug,"AudioDevice.ready","get sample rate",m.sampleRate});
        }
    success:
        m.numChannels = AudioBuffer::stereo;
        m.inputID  = -1;
        m.outputID = -1;
        return;
    error:
        this->close();
        m.inputID  = -1;
        m.outputID = -1;
        return;
    }
    auto AudioDevice::close() -> void {
        //auto lock = this->getLock();
        if (m.streamData) {
            if (isRunning()) {
                Support::stop(m.streamData->oStream);
                Support::stop(m.streamData->iStream);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                logManager.add({Log::debug,"AudioDevice.close","stop io stream"});
            }
            Support::close(m.streamData->oStream);
            Support::close(m.streamData->iStream);
            Support::close(m.streamData->builder);
            logManager.add({Log::debug,"AudioDevice.close","close io stream and builder"});
            delete m.streamData;
            logManager.add({Log::debug,"AudioDevice.close","delete stream data",m.streamData});
            m.streamData = nullptr;
        } else {
            logManager.add({Log::debug,"AudioDevice.close","stream data is null"});
        }
        m.sampleRate  = 0.0;
        m.bufferSize  = 0.0;
        m.numChannels = 0;
    }
    auto AudioDevice::start() -> void {
        //auto lock = this->getLock();
        if (not m.streamData or not m.streamData->iStream or not m.streamData->oStream) goto error;
        if (not isRunning()) {
            Support::start(m.streamData->iStream);
            Support::start(m.streamData->oStream);
            logManager.add({Log::debug,"AudioDevice.start","start io stream"});
        }
    success:
        return;
    error:
        logManager.add({Log::error,"AudioDevice.start","not ready stream data or io stream"});
        return;
    }
    auto AudioDevice::stop() -> void {
        //auto lock = this->getLock();
        if (not m.streamData or not m.streamData->iStream or not m.streamData->oStream) goto error;
        if (isRunning()) {
            Support::stop(m.streamData->oStream);
            Support::stop(m.streamData->iStream);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            logManager.add({Log::debug,"AudioDevice.stop","stop io stream"});
        }
    success:
        return;
    error:
        logManager.add({Log::error,"AudioDevice.stop","not ready stream data or io stream"});
        return;
    }
    auto AudioDevice::setID(int inputID, int outputID) -> void {
        this->m.inputID  = inputID;
        this->m.outputID = outputID;
    }
}
#endif
