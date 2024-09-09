/*
 * Baund Studio Engine
 *
 * ProjectActions.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "ProjectActions.hpp"
#include "Sources/Project/ProjectManager.hpp"

using namespace Baund::StudioEngine;

_BSEObjectID _BSEProject_create() {
    constexpr auto errorResult = PublicObject::none.value;
    try {
        // 오디오기기 상태확인.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEProject.create","audio device not ready"});
            return errorResult;
        } else {
            logManager.add({Log::debug,"BSEProject.create","",
                "sample rate: "  + std::to_string(device.getSampleRate()) + " " +
                "buffer size: "  + std::to_string(device.getBufferSize()) + " " +
                "channel size: " + std::to_string(device.getChannelSize())
            });
        }
        // 프로젝트 생성.
        auto project = projectManager.add(Project());
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEProject.create","failed to create project"});
            return errorResult;
        } else {
            logManager.add({Log::debug,"BSEProject.create","",
                "project ID: " + std::to_string(project->getObjectID().value)
            });
        }
        // 믹서 입출력 설정.
        auto& masterChannel = project->getMixer().getMasterChannel();
        masterChannel.setInputRouting (project->getRoutingManager().getInputMasterRouting());
        masterChannel.setOutputRouting(project->getRoutingManager().getOutputMasterRouting());
        masterChannel.setActiveState(true);
        // 프로젝트 활성화.
        projectManager.setActivated(project);
        auto activated = projectManager.getActivated();
        logManager.add({Log::debug,"BSEProject.create","set active project",
            "project ID: " + std::to_string(activated->getObjectID().value)
        });
        return activated->getObjectID().value;
    } catch (...) {
        logManager.add({Log::error,"BSEProject.create","catch exception"});
        return errorResult;
    }
}
void _BSEProject_remove(_BSEObjectID projectID) {
    try {
        // 프로젝트 가져오기.
        auto project = projectManager.getProject({projectID});
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEProject.remove","failed to get project",
                "project ID: " + std::to_string(projectID)
            });
            return;
        }
        auto activated = projectManager.getActivated();
        projectManager.sub(project);
        logManager.add({Log::debug,"BSEProject.remove","",
            "project ID: " + std::to_string(projectID)
        });
        // 변경된 활성화 프로젝트 확인.
        auto newActivated = projectManager.getActivated();
        if (not newActivated.isValid()) {
            logManager.add({Log::debug,"BSEProject.remove","no active project"});
        } else if(newActivated != activated) {
            logManager.add({Log::debug,"BSEProject.remove","change active project",
                "project ID: " + std::to_string(newActivated->getObjectID().value)
            });
        }
    } catch (...) {
        logManager.add({Log::error,"BSEProject.remove","catch exception"});
    }
}
void _BSEProject_active(_BSEObjectID projectID) {
    try {
        // 오디오기기 상태확인.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEProject.active","audio device not ready"});
            return;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getProject({projectID});
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEProject.active","failed to get project",
                "project ID: " + std::to_string(projectID)
            });
            return;
        }
        // 프로젝트 활성화.
        projectManager.setActivated(project);
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::debug,"BSEProject.active","no active project"});
            return;
        }
        logManager.add({Log::debug,"BSEProject.active","set active project",
            "project ID: " + std::to_string(activated->getObjectID().value)
        });
    } catch (...) {
        logManager.add({Log::error,"BSEProject.active","catch exception"});
    }
}
_BSEBool _BSEProject_isActivated(_BSEObjectID projectID) {
    constexpr auto errorResult = false;
    try {
        // 프로젝트 가져오기.
        auto project = projectManager.getProject({projectID});
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEProject.isActivated","failed to get project",
                "project ID: " + std::to_string(projectID)
            });
            return errorResult;
        }
        // 활성화여부 확인.
        auto isActivated = projectManager.isActivated(project);
        logManager.add({Log::debug,"BSEProject.isActivated","",
            "project ID: " + std::to_string(projectID) + " " +
            "is activated: " + (isActivated ? "true" : "false")
        });
        return isActivated;
    } catch (...) {
        logManager.add({Log::error,"BSEProject.isActivated","catch exception"});
        return errorResult;
    }
}
_BSEBool _BSEProject_isValid(_BSEObjectID projectID) {
    try {
        auto isValid = projectManager.getProject({projectID}).isValid();
        logManager.add({Log::debug,"BSEProject.isValid","",
            "project ID: " + std::to_string(projectID) + " " +
            "is valid: " + (isValid ? " true" : " false")
        });
        return isValid;
    } catch (...) {
        logManager.add({Log::error,"BSEProject.isValid","catch exception"});
        return false;
    }
}
void _BSEProject_setTempDir(_BSEPath path) {
    try {
        // 프로젝트 가져오기.
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.setTempDir","no active project"});
            return;
        }
        // 임시 디렉토리 지정
        activated->getMediaFileManager().setTempDir(path);
        logManager.add({Log::debug,"BSEProject.setTempDir","",
            "project ID: " + std::to_string(activated->getObjectID().value) + " " +
            "temp path: " + activated->getMediaFileManager().getTempDir()
        });
    } catch (...) {
        logManager.add({Log::error,"BSEProject.setTempDir","catch exception"});
    }
}
_BSEPath _BSEProject_getTempDir() {
    constexpr auto errorResult = "";
    try {
        // 프로젝트 가져오기.
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.getTempDir","no active project"});
            return errorResult;
        }
        // 임시 디렉토리 가져오기.
        auto& tempDir = activated->getMediaFileManager().getTempDir();
        logManager.add({Log::debug,"BSEProject.getTempDir","",
            "project ID: " + std::to_string(activated->getObjectID().value) + " " +
            "temp path: " + tempDir
        });
        return tempDir.c_str();
    } catch (...) {
        logManager.add({Log::error,"BSEProject.getTempDir","catch exception"});
        return errorResult;
    }
}
_BSESampleValue _BSEProject_getRecValue() {
    constexpr auto errorResult = 0.0f;
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            return errorResult;
        }
        return projectManager.getActivated()->getRecordMeterValue();
    } catch (...) {
        logManager.add({Log::error,"BSEProject.getRecValue","catch exception"});
        return errorResult;
    }
}
_BSESeconds _BSEProject_getLength() {
    constexpr auto errorResult = 0.0;
    try {
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEProject.getLength","audio device not ready"});
            return errorResult;
        }
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.getLength","no active project"});
            return errorResult;
        }
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEProject.getLength","failed to get sample rate"});
            return errorResult;
        }
        auto length = activated->getLength();
        auto time   = Algorithm::convertAudioFramesToSeconds(length,sampleRate);
        logManager.add({Log::debug,"BSEProject.getLength","",
            "project ID: " + std::to_string(activated->getObjectID().value) + " " +
            "length: " + std::to_string(time) + "s"
        });
        return time;
    } catch (...) {
        logManager.add({Log::error,"BSEProject.getLength","catch exception"});
        return errorResult;
    }
}
_BSEPath _BSEProject_writeToAudioFile() {
    constexpr auto errorResult = "";
    try {
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEProject.writeToAudioFile","audio device not ready"});
            return errorResult;
        }
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.writeToAudioFile","no active project"});
            return errorResult;
        }
        static auto path = AudioFile::Path();
        path = activated->audioMixdown().getPath();
        if (path == "") {
            logManager.add({Log::error,"BSEProject.writeToAudioFile","failed to audio mixdown"});
            return errorResult;
        } else {
            logManager.add({Log::debug,"BSEProject.writeToAudioFile","",
                "project ID: " + std::to_string(activated->getObjectID().value) + " " +
                "file path: " + path
            });
            return path.c_str();
        }
    } catch (...) {
        logManager.add({Log::error,"BSEProject.writeToAudioFile","catch exception"});
        return errorResult;
    }
}
_BSEPath _BSEProject_writeLogToFile(_BSEPath in) {
    constexpr auto errorResult = "";
    try {
        static auto path = LogManager::Path();
        path = logManager.writeLogToFile(in);
        if (path == "") {
            logManager.add({Log::error,"BSEProject.writeLogToFile","failed to write log file"});
            return errorResult;
        }
        logManager.add({Log::debug,"BSEProject.writeLogToFile","",
            "file path: " + path
        });
        return path.c_str();
    } catch (...) {
        logManager.add({Log::error,"BSEProject.writeLogToFile","catch exception"});
        return errorResult;
    }
}
_BSEObjectID _BSEProject_getRecordItem() {
    constexpr auto errorResult = PublicObject::none.value;
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.getRecordItem","no active project"});
            return errorResult;
        }
        activated->writeDataToAudioItem();
        auto item = activated->getRecordingItem();
        if (not item.isValid()) {
            logManager.add({Log::error,"BSEProject.getRecordItem","not find record item"});
            return errorResult;
        }
        logManager.add({Log::debug,"BSEProject.getRecordItem","",
            "project ID: " + std::to_string(activated->getObjectID().value) + " " +
            "track ID: " + std::to_string(item->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(item->getObjectID().value) + " " +
            "position: " + std::to_string(item->getPosition()) + " " +
            "length: " + std::to_string(item->getLength()) + " " +
            "offset: " + std::to_string(item->getOffset()) + " " +
            "file: " + item->getAudioFile()->audioFile.getPath()
        });
        return item->getObjectID().value;
    } catch (...) {
        logManager.add({Log::error,"BSEProject.getRecordItem","catch exception"});
        return errorResult;
    }
}
void _BSEProject_clearRecordBuffer() {
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.clearRecordBuffer","no active project"});
            return;
        }
        activated->clearRecordingData();
        logManager.add({Log::debug,"BSEProject.clearRecordBuffer",""});
    } catch (...) {
        logManager.add({Log::error,"BSEProject.clearRecordBuffer","catch exception"});
    }
}
void _BSEProject_setGlobalTempo(_BSETempo in) {
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.setGlobalTempo","no active project"});
            return;
        }
        activated->setGlobalTempo(in);
        logManager.add({Log::debug,"BSEProject.setGlobalTempo","",
            "project ID: " + std::to_string(activated->getObjectID().value) + " " +
            "tempo: " + std::to_string(activated->getGlobalTempo())
        });
    } catch (...) {
        logManager.add({Log::error,"BSEProject.setGlobalTempo","catch exception"});
    }
}
_BSETempo _BSEProject_getGlobalTempo() {
    constexpr auto errorResult = 0.0;
    try {
        auto activated = projectManager.getActivated();
        if (not activated.isValid()) {
            logManager.add({Log::error,"BSEProject.getGlobalTempo","no active project"});
            return errorResult;
        }
        auto tempo = activated->getGlobalTempo();
        logManager.add({Log::debug,"BSEProject.getGlobalTempo","",
            "project ID: " + std::to_string(activated->getObjectID().value) + " " +
            "tempo: " + std::to_string(tempo)
        });
        return tempo;
    } catch (...) {
        logManager.add({Log::error,"BSEProject.getGlobalTempo","catch exception"});
        return errorResult;
    }
}
