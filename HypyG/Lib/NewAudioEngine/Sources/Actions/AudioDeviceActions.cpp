/*
 * Baund Studio Engine
 *
 * AudioDeviceActions.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "AudioDeviceActions.hpp"
#include "Sources/Project/ProjectManager.hpp"

using namespace Baund::StudioEngine;

void _BSEAudioDevice_ready() {
    try {
        deviceManager.getAudioDevice().ready();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.ready","catch exception"});
    }
}
void _BSEAudioDevice_start() {
    try {
        deviceManager.getAudioDevice().start();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.start","catch exception"});
    }
}
void _BSEAudioDevice_stop() {
    try {
        deviceManager.getAudioDevice().stop();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.stop","catch exception"});
    }
}
void _BSEAudioDevice_close() {
    try {
        deviceManager.getAudioDevice().close();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.close","catch exception"});
    }
}
_BSEBool _BSEAudioDevice_isReady() {
    try {
        auto isValid = deviceManager.getAudioDevice().isValid();
        logManager.add({Log::debug,"BSEAudioDevice.isReady","",(isValid ? "true" : "false")});
        return isValid;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.isReady","catch exception"});
        return 0;
    }
}
_BSEBool _BSEAudioDevice_isStart() {
    try {
        auto isRunning = deviceManager.getAudioDevice().isRunning();
        logManager.add({Log::debug,"BSEAudioDevice.isStart","",(isRunning ? "true" : "false")});
        return isRunning;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.isStart","catch exception"});
        return 0;
    }
}
_BSESeconds _BSEAudioDevice_getLatency(void) {
    constexpr auto errorResult = 0.0;
    try {
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioDevice.getLatency","audio device not ready"});
            return errorResult;
        } else {
            auto latency = device.getLatency();
            auto time    = Algorithm::convertAudioFramesToSeconds(latency, device.getSampleRate());
            logManager.add({Log::debug,"BSEAudioDevice.getLatency","",
                std::to_string(time) + "s"
            });
            return time;
        };
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.getLatency","catch exception"});
        return errorResult;
    }
}

#ifdef __ANDROID__
void _BSEAudioDevice_setID(int inputID, int outputID) {
    try {
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioDevice.setID","audio device not ready"});
            return;
        }
        device.setID(inputID,outputID);
    } catch (...) {
        logManager.add({Log::error,"BSEAudioDevice.setID","catch exception"});
    }
}
#endif//__ANDROID__