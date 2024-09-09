/*
 * Baund Studio Engine
 *
 * DeviceManager.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_DeviceManager_hpp
#define BaundStudioEngine_Sources_BaseLibrary_DeviceManager_hpp

#include "Definitions.hpp"
#include "Algorithm.hpp"
#include "LogManager.hpp"
#include "AudioBuffer.hpp"
#include <functional>

namespace Baund::StudioEngine {
    class AudioDevice final: public virtual BaseObject {
    public:
        struct StreamData;
        using IOCallback    = std::function<void(AudioBuffer const&)>;
        using ErrorCallback = std::function<void()>;
        using SampleRate    = AudioBuffer::SampleRate;
        using Frames        = AudioBuffer::Frames;
        using Channels      = AudioBuffer::Channels;
        
        ~AudioDevice() override;
        AudioDevice()                   = default;
        AudioDevice(AudioDevice const&) = delete;
        AudioDevice(AudioDevice     &&);

        auto operator=(AudioDevice const&) -> AudioDevice& = delete;
        auto operator=(AudioDevice     &&) -> AudioDevice&;

        void reset  ()       override;
        bool isValid() const override;

        auto isRunning() const -> bool;
        auto ready() -> void;
        auto close() -> void;
        auto start() -> void;
        auto stop () -> void;

        auto setIOCallback   (IOCallback   ) -> void;
        auto setErrorCallback(ErrorCallback) -> void;

        auto getSampleRate () const -> SampleRate const&;
        auto getBufferSize () const -> Frames     const&;
        auto getChannelSize() const -> Channels   const&;
        auto getLatency    () const -> Frames;

#ifdef __ANDROID__
        auto setID(int,int) -> void;
#endif//__ANDROID__
        
    private:
        struct MemberVariables {
            StreamData*   streamData    = nullptr;
            IOCallback    ioCallback    = nullptr;
            ErrorCallback errorCallback = nullptr;
            SampleRate    sampleRate    = 0.0;
            Frames        bufferSize    = 0;
            Frames        correction    = 0;
            Channels      numChannels   = 0;
#ifdef __ANDROID__
            int inputID  = -1;
            int outputID = -1;
#endif//__ANDROID__
        } m;
    };
    extern class DeviceManager final: public virtual BaseObject {
    public:
        ~DeviceManager() override = default;
        DeviceManager()           = default;
        DeviceManager(DeviceManager const&) = delete;
        DeviceManager(DeviceManager     &&) = delete;
        
        auto operator=(DeviceManager const&) -> DeviceManager& = delete;
        auto operator=(DeviceManager     &&) -> DeviceManager& = delete;

        void reset  ()       override;
        bool isValid() const override;
        
        auto getAudioDevice()       -> AudioDevice&;
        auto getAudioDevice() const -> AudioDevice const&;
        
    private:
        struct MemberVariables {
            AudioDevice audioDevice;
        } m;
    } deviceManager;
    
    inline AudioDevice::~AudioDevice() {
        this->close();
    }
    inline AudioDevice::AudioDevice(AudioDevice&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto AudioDevice::operator=(AudioDevice&& in) -> AudioDevice& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void AudioDevice::reset() {
        this->close();
        m = MemberVariables();
    }
    inline auto AudioDevice::setIOCallback(IOCallback in) -> void {
        m.ioCallback = in;
    }
    inline auto AudioDevice::setErrorCallback(ErrorCallback in) -> void {
        m.errorCallback = in;
    }
    inline auto AudioDevice::getSampleRate() const -> SampleRate const& {
        return m.sampleRate;
    }
    inline auto AudioDevice::getBufferSize() const -> Frames const& {
        return m.bufferSize;
    }
    inline auto AudioDevice::getChannelSize() const -> Channels const& {
        return m.numChannels;
    }
    inline auto AudioDevice::getLatency() const -> Frames {
#if defined(__APPLE__)
        return m.bufferSize * 4;
#elif defined(__ANDROID__)
        return m.correction; 
#endif
    }
    inline void DeviceManager::reset() {
        m.audioDevice.close();
        m = MemberVariables();
    }
    inline bool DeviceManager::isValid() const {
        return m.audioDevice.isValid();
    }
    inline auto DeviceManager::getAudioDevice() -> AudioDevice& {
        return m.audioDevice;
    }
    inline auto DeviceManager::getAudioDevice() const -> AudioDevice const& {
        return m.audioDevice;
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_DeviceManager_hpp
