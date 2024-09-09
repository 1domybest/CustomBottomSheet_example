/*
 * Baund Studio Engine
 *
 * TransportActions.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "TransportActions.hpp"
#include "Sources/Project/ProjectManager.hpp"

using namespace Baund::StudioEngine;

void _BSETransport_play() {
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.play","no active project"});
        } else {
            activated->getTransport().setPlayState(true);
            logManager.add({Log::debug,"BSETransport.play",""});
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.play","catch exception"});
    }
}
void _BSETransport_stop() {
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.stop","no active project"});
        } else {
            activated->getTransport().setPlayState(false);
            logManager.add({Log::debug,"BSETransport.stop",""});
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.stop","catch exception"});
    }

}
_BSEBool _BSETransport_isPlaying() {
    constexpr auto errorResult = false;
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.isPlaying","no active project"});
            return errorResult;
        } else {
            auto state = activated->getTransport().getPlayState();
            //logManager.add({Log::debug,"BSETransport.isPlaying","",(state ? "true" : "false")});
            return state;
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.isPlaying","catch exception"});
        return errorResult;
    }
}
void _BSETransport_recordOn() {
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.recordOn","no active project"});
        } else {
            activated->getTransport().setRecordState(true);
            logManager.add({Log::debug,"BSETransport.recordOn",""});
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.recordOn","catch exception"});
    }
}
void _BSETransport_recordOff() {
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.recordOff","no active project"});
        } else {
            activated->getTransport().setRecordState(false);
            logManager.add({Log::debug,"BSETransport.recordOff",""});
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.recordOff","catch exception"});
    }
}
_BSEBool _BSETransport_isRecording() {
    constexpr auto errorResult = false;
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.isRecording","no active project"});
            return errorResult;
        } else {
            auto state = activated->getTransport().getRecordState();
            //logManager.add({Log::debug,"BSETransport.isRecording","",(state ? "true" : "false")});
            return state;
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.isRecording","catch exception"});
        return errorResult;
    }
}
void _BSETransport_setPosition(_BSESeconds in) {
    try {
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSETransport.setPosition","audio device not ready"});
            return;
        }
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.setPosition","no active project"});
        } else {
            auto frame = Algorithm::convertSecondsToAudioFrames(in, device.getSampleRate());
            logManager.add({Log::debug,"BSETransport.setPosition","",
                std::to_string(in) + "s"
            });
            activated->getTransport().setPosition(frame);
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.setPosition","catch exception"});
    }
}
_BSESeconds _BSETransport_getPosition() {
    constexpr auto errorResult = 0.0;
    try {
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSETransport.getPosition","audio device not ready"});
            return errorResult;
        }
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSETransport.getPosition","no active project"});
            return errorResult;
        } else {
            auto frame = activated->getTransport().getPosition();
            auto position = Algorithm::convertAudioFramesToSeconds(frame, device.getSampleRate());
            logManager.add({Log::debug,"BSETransport.getPosition","",
                std::to_string(position) + "s"
            });
            return position;
        }
    } catch (...) {
        logManager.add({Log::error,"BSETransport.getPosition","catch exception"});
        return errorResult;
    }
}
