/*
 * Baund Studio Engine
 *
 * AudioTrackActions.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "AudioTrackActions.hpp"
#include "Sources/Project/ProjectManager.hpp"

using namespace Baund::StudioEngine;

_BSEObjectID _BSEAudioTrack_create() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    constexpr auto errorResult = PublicObject::none.value;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.create","no active project"});
            return errorResult;
        }
        project->deconnectionToIO();
        // 새로운 트랙을 생성합니다.
        auto track = AudioTrack();
        // 트랙의 입출력을 연결합니다.
        track.setMonitorBuffer(&(project->getMonitorBufferRef()));
        track.setRouting(project->getRoutingManager().add(Routing()));
        track.setMixChannel(project->getMixer().add(MixChannel()));
        track.getMixChannel()->setInputRouting(track.getRouting());
        track.getMixChannel()->setOutputRouting(project->getMixer().getMasterChannel().getInputRouting());
        auto added = project->getTrackManager().add(std::move(track));
        added->getRouting()->setActiveState(true);
        added->getMixChannel()->setActiveState(true);
        added->setActiveState(true);
        if (not added.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.create","failed to create audio track"});
            return errorResult;
        }
        // 트랙의 아이디를 반환합니다.
        logManager.add({Log::debug,"BSEAudioTrack.create","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(added->getObjectID().value)
        });
        project->connectionToIO();
        return added->getObjectID().value;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.create","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_remove(_BSEObjectID in) {
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.remove","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({in});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.remove","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(in)
            });
            return;
        }
        project->deconnectionToIO();
        track->setActiveState(false);
        track->getMixChannel()->setActiveState(false);
        track->getRouting()   ->setActiveState(false);
        auto removeTrack = std::move(track.getValue());
        project->getTrackManager().sub(track);
        for (auto trackItem : removeTrack.getItemData()) {
            if (not trackItem.isValid()) continue;
            auto audioFile = trackItem->getAudioFile();
            auto find      = 0;//false;
            for (auto item : project->getItemManager().getAudioItemList()) {
                if (audioFile->audioFile == item->getAudioFile()->audioFile) {
                    find += 1;//true;
                    //break;
                }
            }
            if (find <= 1) {//not find) {
                logManager.add({Log::debug,"BSEAudioTrack.remove","remove audio file",
                    "project ID: " + std::to_string(project->getObjectID().value) + " " +
                    "file path: " + trackItem->getAudioFile()->audioFile.getPath()
                });
                project->getMediaFileManager().sub(trackItem->getAudioFile());
            }
            logManager.add({Log::debug,"BSEAudioTrack.remove","remove audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(trackItem->getObjectID().value)
            });
            project->getItemManager().sub(trackItem);
        }
        // 트랙에 연결된 믹스체널을 제거합니다.
        auto mixChannel = removeTrack.getMixChannel();
        project->getMixer().sub(mixChannel);
        // 트랙을 제거합니다.
        logManager.add({Log::debug,"BSEAudioTrack.remove","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(removeTrack.getObjectID().value)
        });
        project->connectionToIO();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.remove","catch exception"});
    }
}
_BSEBool _BSEAudioTrack_isValid(_BSEObjectID in) {
    constexpr auto errorResult = false;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isValid","no active project"});
            return errorResult;
        }
        // 트랙이 유효한지를 확인합니다.
        auto result = project->getTrackManager().getAudioTrack({in}).isValid();
        logManager.add({Log::debug,"BSEAudioTrack.isValid","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(in) + " " +
            (result ? "true" : "false")});
        return result;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.isValid","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_setPanValue(_BSEObjectID trackID, _BSEPercent value) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setPanValue","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setPanValue","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setPanValue","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 패닝값을 설정합니다.
        channel->setPanValue(value / 100.0);
        auto panValue = channel->getPanValue() * 100.0;
        logManager.add({Log::debug,"BSEAudioTrack.setPanValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "value: " + (panValue >  0 ? "R" + std::to_string(panValue) :
                         panValue == 0 ? "C" : "L" + std::to_string(std::abs(panValue)))
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.setPanValue","catch exception"});
    }
}
_BSEPercent _BSEAudioTrack_getPanValue(_BSEObjectID trackID) {
    constexpr auto errorResult = 0.0;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getPanValue","no active project"});
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getPanValue","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getPanValue","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 패닝값을 가져옵니다.
        auto panValue = channel->getPanValue() * 100.0;
        logManager.add({Log::debug,"BSEAudioTrack.getPanValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "value: " + (panValue >  0 ? "R" + std::to_string(panValue) :
                         panValue == 0 ? "C" : "L" + std::to_string(std::abs(panValue)))
        });
        return panValue;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.getPanValue","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_setGainValue(_BSEObjectID trackID, _BSEPercent value) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setGainValue","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setGainValue","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setGainValue","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 게인값을 설정합니다.
        channel->setGainValue(value / 100.0);
        auto gainValue = channel->getGainValue() * 100.0;
        logManager.add({Log::debug,"BSEAudioTrack.setGainValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "value: " + std::to_string(gainValue) + "%"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.setGainValue","catch exception"});
    }
}
_BSEPercent _BSEAudioTrack_getGainValue(_BSEObjectID trackID) {
    constexpr auto errorResult = 0.0;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getGainValue","no active project"});
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getGainValue","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getGainValue","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 게인값을 가져옵니다.
        auto gainValue = channel->getGainValue() * 100.0;
        logManager.add({Log::debug,"BSEAudioTrack.getGainValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "value: " + std::to_string(gainValue) + "%"
        });
        return gainValue;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.getGainValue","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_muteOn(_BSEObjectID trackID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.muteOn","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.muteOn","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.muteOn","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 뮤트를 활성화합니다.
        channel->setMuteState(true);
        logManager.add({Log::debug,"BSEAudioTrack.muteOn","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (channel->getMuteState() ? "true" : "false")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.muteOn","catch exception"});
    }
}
void _BSEAudioTrack_muteOff(_BSEObjectID trackID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.muteOff","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.muteOff","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.muteOff","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 뮤트를 비활성화합니다.
        channel->setMuteState(false);
        logManager.add({Log::debug,"BSEAudioTrack.muteOff","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (channel->getMuteState() ? "true" : "false")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.muteOff","catch exception"});
    }
}
_BSEBool _BSEAudioTrack_isMute(_BSEObjectID trackID) {
    constexpr auto errorResult = false;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isMute","no active project"});
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isMute","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널을 가져옵니다.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isMute","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 뮤트의 상태를 가져옵니다.
        auto state = channel->getMuteState();
        logManager.add({Log::debug,"BSEAudioTrack.isMute","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (state ? "true" : "false")
        });
        return state;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.isMute","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_recordOn(_BSEObjectID trackID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.recordOn","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.recordOn","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 레코딩을 활성화합니다.
        track->setRecordState(true);
        logManager.add({Log::debug,"BSEAudioTrack.recordOn","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (track->getRecordState() ? "true" : "false")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.recordOn","catch exception"});
    }
}
void _BSEAudioTrack_recordOff(_BSEObjectID trackID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.recordOff","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.recordOff","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 레코딩을 비활성화합니다.
        track->setRecordState(false);
        logManager.add({Log::debug,"BSEAudioTrack.recordOff","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (track->getRecordState() ? "true" : "false")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.recordOff","catch exception"});
    }
}
_BSEBool _BSEAudioTrack_isRecording(_BSEObjectID trackID) {
    constexpr auto errorResult = false;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isRecording","no active project"});
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isRecording","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 레코딩 상태를 반환합니다.
        auto state = track->getRecordState();
        logManager.add({Log::debug,"BSEAudioTrack.isRecording","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (state ? "true" : "false")
        });
        return state;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.isRecording","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_monitorOn(_BSEObjectID trackID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.monitorOn","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.monitorOn","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 모니터링을 활성화합니다.
        track->setMonitorState(true);
        logManager.add({Log::debug,"BSEAudioTrack.monitorOn","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (track->getMonitorState() ? "true" : "false")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.monitorOn","catch exception"});
    }
}
void _BSEAudioTrack_monitorOff(_BSEObjectID trackID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.monitorOff","no active project"});
            return;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.monitorOff","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 모니터링을 비활성화합니다.
        track->setMonitorState(false);
        logManager.add({Log::debug,"BSEAudioTrack.monitorOff","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (track->getMonitorState() ? "true" : "false")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.monitorOff","catch exception"});
    }
}
_BSEBool _BSEAudioTrack_isMonitoring(_BSEObjectID trackID) {
    constexpr auto errorResult = false;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isRecording","no active project"});
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isRecording","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 모니터링 상태를 반환합니다.
        auto state = track->getMonitorState();
        logManager.add({Log::debug,"BSEAudioTrack.isRecording","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "state: " + (state ? "true" : "false")
        });
        return state;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.isRecording","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_addFX(_BSEObjectID trackID, _BSEIndex slot, _BSEString key) {
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.addFX","slot is out of range",slot});
            return;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.addFX","no active project"});
            return;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.addFX","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.addFX","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 이펙트 설정하기.
        auto effect = effectLibrary.create(key);
        if (not effect) {
            channel->setInsertFX(nullptr, slot);
            logManager.add({Log::debug,"BSEAudioTrack.addFX","remove effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "key: " + key
            });
            return;
        }
        effect->setTempo(project->getGlobalTempo());
        channel->setInsertFX(effect, slot);
        logManager.add({Log::debug,"BSEAudioTrack.addFX","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "key: " + key
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.addFX","catch exception"});
    }
}
_BSEString _BSEAudioTrack_getFXCategory(_BSEObjectID trackID, _BSEIndex slot) {
    constexpr auto errorResult = "";
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.getFXCategory","slot is out of range",slot});
            return errorResult;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXCategory","no active project"});
            return errorResult;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXCategory","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXCategory","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.getFXCategory","failed to get effect",slot});
            return errorResult;
        }
        // 카테고리 가져오기.
        auto& category = effect->getCategory();
        logManager.add({Log::debug,"BSEAudioTrack.getFXCategory","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "category: " + category
        });
        return category.c_str();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.getFXCategory","catch exception"});
        return errorResult;
    }
}
_BSEString _BSEAudioTrack_getFXName(_BSEObjectID trackID, _BSEIndex slot) {
    constexpr auto errorResult = "";
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.getFXName","slot is out of range",slot});
            return errorResult;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXName","no active project"});
            return errorResult;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXName","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXName","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.getFXName","failed to get effect",slot});
            return errorResult;
        }
        // 이름 가져오기.
        auto& name = effect->getName();
        logManager.add({Log::debug,"BSEAudioTrack.getFXName","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "name: " + name
        });
        return name.c_str();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.getFXName","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_setFXParameterValue(_BSEObjectID trackID, _BSEIndex slot, _BSEIndex parameterIndex, _BSEEvent value) {
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.setFXParameterValue","slot is out of range",slot});
            return;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setFXParameterValue","no active project"});
            return;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setFXParameterValue","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.setFXParameterValue","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.setFXParameterValue","failed to get effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "slot: " + std::to_string(slot)
            });
            return;
        }
        // 이펙트의 설정값변경.
        effect->getParameters()[parameterIndex] = value;
        logManager.add({Log::debug,"BSEAudioTrack.setFXParameterValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            effect->getParameterName(parameterIndex) + ": " + std::to_string(effect->getParameters()[parameterIndex])
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.setFXParameterValue","catch exception"});
    }
}
_BSEEvent _BSEAudioTrack_getFXParameterValue(_BSEObjectID trackID, _BSEIndex slot, _BSEIndex parameterIndex) {
    constexpr auto errorResult = 0.0;
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterValue","slot is out of range",slot});
            return errorResult;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterValue","no active project"});
            return errorResult;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterValue","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterValue","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterValue","failed to get effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "slot: " + std::to_string(slot)
            });
            return errorResult;
        }
        // 설정값 가져오기.
        auto value = effect->getParameters()[parameterIndex];
        logManager.add({Log::debug,"BSEAudioTrack.getFXParameterValue","get parameter value",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            effect->getParameterName(parameterIndex) + ": " + std::to_string(value)
        });
        return value;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.getFXParameterValue","catch exception"});
        return errorResult;
    }
}
_BSEString _BSEAudioTrack_getFXParameterName(_BSEObjectID trackID, _BSEIndex slot, _BSEIndex parameterIndex) {
    constexpr auto errorResult = "";
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterName","slot is out of range",slot});
            return errorResult;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterName","no active project"});
            return errorResult;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterName","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterName","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.getFXParameterName","failed to get effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "slot: " + std::to_string(slot)
            });
            return errorResult;
        }
        // 설정이름 가져오기.
        auto& name = effect->getParameterName(parameterIndex);
        logManager.add({Log::debug,"BSEAudioTrack.getFXParameterName","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "parameter name: " + name
        });
        return name.c_str();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.getFXParameterName","catch exception"});
        return errorResult;
    }
}
void _BSEAudioTrack_activeFX(_BSEObjectID trackID, _BSEIndex slot) {
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.activeFX","slot is out of range",slot});
            return;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.activeFX","no active project"});
            return;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.activeFX","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.activeFX","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.activeFX","failed to get effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "slot: " + std::to_string(slot)
            });
            return;
        }
        // 이펙트 상태 변경.
        effect->setState(Effect::State::on);
        logManager.add({Log::debug,"BSEAudioTrack.activeFX","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "state: " + (effect->getState() == Effect::State::on ? "on" :
                         effect->getState() == Effect::State::bypass ? "bypass" : "off")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.activeFX","catch exception"});
    }
}
void _BSEAudioTrack_bypassFX(_BSEObjectID trackID, _BSEIndex slot) {
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.bypassFX","slot is out of range",slot});
            return;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.bypassFX","no active project"});
            return;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.bypassFX","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.bypassFX","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.bypassFX","failed to get effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "slot: " + std::to_string(slot)
            });
            return;
        }
        // 이펙트 상태 변경.
        effect->setState(Effect::State::bypass);
        logManager.add({Log::debug,"BSEAudioTrack.bypassFX","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "state: " + (effect->getState() == Effect::State::on ? "on" :
                         effect->getState() == Effect::State::bypass ? "bypass" : "off")
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.bypassFX","catch exception"});
    }
}
_BSEBool _BSEAudioTrack_isActivatedFX(_BSEObjectID trackID, _BSEIndex slot) {
    constexpr auto errorResult = false;
    try {
        // 슬롯 확인
        if (slot < 0) {
            logManager.add({Log::error,"BSEAudioTrack.isActivatedFX","slot is out of range",slot});
            return errorResult;
        }
        // 프로젝트 가져오기.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isActivatedFX","no active project"});
            return errorResult;
        }
        // 트랙 가져오기.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isActivatedFX","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 믹스체널 가져오기.
        auto channel = track->getMixChannel();
        if (not channel.isValid()) {
            logManager.add({Log::error,"BSEAudioTrack.isActivatedFX","failed to get mix channel",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 이펙트 가져오기.
        auto effect = channel->getInsertFX(slot);
        if (not effect) {
            logManager.add({Log::error,"BSEAudioTrack.isActivatedFX","failed to get effect",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "slot: " + std::to_string(slot)
            });
            return errorResult;
        }
        // 이펙트 상태 가져오기.
        auto state = effect->getState();
        logManager.add({Log::debug,"BSEAudioTrack.isActivatedFX","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(track->getObjectID().value) + " " +
            "slot: " + std::to_string(slot) + " " +
            "state: " + (state == Effect::State::on ? "on" :
                         state == Effect::State::bypass ? "bypass" : "off")
        });
        return state == Effect::on;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioTrack.isActivatedFX","catch exception"});
        return errorResult;
    }
}
