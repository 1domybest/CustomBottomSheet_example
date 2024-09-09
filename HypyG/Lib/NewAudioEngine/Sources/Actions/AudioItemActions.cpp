/*
 * Baund Studio Engine
 *
 * AudioItemActions.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "AudioItemActions.hpp"
#include "Sources/Project/ProjectManager.hpp"

using namespace Baund::StudioEngine;

_BSEObjectID _BSEAudioItem_create(_BSEObjectID trackID,_BSESeconds position,_BSEPath filePath) {
    constexpr auto errorResult = PublicObject::none.value;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.create","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.create","no active project"});
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack({trackID});
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.create","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(trackID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.create","failed to get sample rate"});
            return errorResult;
        }
        // 아이템을 생성합니다.
        auto audioItem = project->getItemManager().add(AudioItem());
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.create","failed to add audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value)
            });
            return errorResult;
        }
        // 오디오파일을 추가합니다.
        auto audioFile = project->getMediaFileManager().add(AudioFile(filePath));
        if (not audioFile.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.create","invalid audio file",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(track->getObjectID().value) + " " +
                "file path: " + filePath
            });
            return errorResult;
        }
        // 아이템을 설정합니다.
        auto frame = Algorithm::convertSecondsToAudioFrames(position, sampleRate);
        audioItem->setAudioTrack({trackID});
        audioItem->setPosition(frame);
        audioItem->setLength(audioFile->buffer.getFrameSize());
        audioItem->setAudioFile(audioFile);
        track->add(audioItem);
        track->sortItemData();
        auto itemPos = Algorithm::convertAudioFramesToSeconds(audioItem->getPosition(), sampleRate);
        auto itemLen = Algorithm::convertAudioFramesToSeconds(audioItem->getLength()  , sampleRate);
        auto itemOff = Algorithm::convertAudioFramesToSeconds(audioItem->getOffset()  , sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.create","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "file path: " + audioItem->getAudioFile()->audioFile.getPath() + " " +
            "position: " + std::to_string(itemPos) + "s " +
            "length: " + std::to_string(itemLen) + "s " +
            "offset: " + std::to_string(itemOff) + "s "
        });
        return audioItem->getObjectID().value;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.create","catch exception"});
        return errorResult;
    }
}
void _BSEAudioItem_remove(_BSEObjectID itemID) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.remove","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.remove","invalid audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 트랙을 가져옵니다.
        auto audioTrack = project->getTrackManager().getAudioTrack(audioItem->getAudioTrack());
        if (not audioTrack.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.remove","invalid audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 더이상 사용되지 않는 오디오파일의 버퍼를 비웁니다.
        auto audioFile = audioItem->getAudioFile();
        audioTrack->sub(audioItem);
        if (audioFile.isValid()) {
            auto find = 0;//false;
            for (auto accessor : project->getItemManager().getAudioItemList()) {
                if (accessor->getAudioFile()->audioFile == audioFile->audioFile) {
                    find += 1;//true;
                    logManager.add({Log::debug,"BSEAudioItem.remove","audio file is being used by another item",accessor->getObjectID().value});
                    if (find > 1) break;
                }
            }
            if (find <= 1) { // (not find) {
                logManager.add({Log::debug,"BSEAudioItem.remove","remove audio file buffer",audioFile->audioFile.getPath()});
                project->getMediaFileManager().sub(audioFile);
            }
        }
        // 제거된 아이템을 로그에 기록합니다.
        logManager.add({Log::debug,"BSEAudioItem.remove","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(itemID)
        });
        // 아이템을 제거합니다.
        project->getItemManager().sub(audioItem);
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.remove","catch exception"});
    }
}
void _BSEAudioItem_move(_BSEObjectID itemID,_BSESeconds position,_BSEObjectID trackID) {
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.move","audio device not ready"});
            return;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.move","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.move","invalid audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 트랙을 가져옵니다.
        auto audioTrack = project->getTrackManager().getAudioTrack(audioItem->getAudioTrack());
        if (not audioTrack.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.move","invalid audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioTrack.move","failed to get sample rate"});
            return;
        }
        // 아이템의 포지션을 변경합니다.
        auto frame = Algorithm::convertSecondsToAudioFrames(position, sampleRate);
        if (audioItem->getAudioTrack().value != trackID) {
            auto from = audioTrack;
            auto dest = project->getTrackManager().getAudioTrack({trackID});
            if (not dest.isValid()) {
                logManager.add({Log::error,"BSEAudioItem.move","invalid audio track",
                    "project ID: " + std::to_string(project->getObjectID().value) + " " +
                    "track ID: " + std::to_string(dest->getObjectID().value)
                });
                return;
            }
            audioItem->setPosition(frame);
            from->sub(audioItem);
            dest->add(audioItem);
            audioItem->setAudioTrack({trackID});
            from->sortItemData();
            dest->sortItemData();
        } else {
            audioItem->setPosition(frame);
            audioTrack->sortItemData();
        }
        auto itemPos = Algorithm::convertAudioFramesToSeconds(audioItem->getPosition(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.move","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "position: " + std::to_string(itemPos) + "s"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.move","catch exception"});
    }
}
_BSEObjectID _BSEAudioItem_cut(_BSEObjectID itemID,_BSESeconds position) {
    constexpr auto errorResult = PublicObject::none.value;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.cut","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.cut","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.cut","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 트랙을 가져옵니다.
        auto track = project->getTrackManager().getAudioTrack(audioItem->getAudioTrack());
        if (not track.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.cut","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.cut","failed to get sample rate"});
            return errorResult;
        }
        auto itemPos    = audioItem->getPosition();
        auto itemEnd    = itemPos + audioItem->getLength();
        auto cutPos     = Algorithm::convertSecondsToAudioFrames(position, sampleRate);
        if (itemPos >= cutPos or cutPos >= itemEnd) {
            logManager.add({Log::error,"BSEAudioItem.cut","position is out of range",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
                "item ID: " + std::to_string(itemID) + " " +
                "pos: " + std::to_string(Algorithm::convertAudioFramesToSeconds(itemPos, sampleRate)) + "s " +
                "end: " + std::to_string(Algorithm::convertAudioFramesToSeconds(itemEnd, sampleRate)) + "s " +
                "cut: " + std::to_string(Algorithm::convertAudioFramesToSeconds(cutPos , sampleRate)) + "s "
            });
            return errorResult;
        }
        // 새로운 아이템 생성.
        auto addedItem = project->getItemManager().add(AudioItem());
        addedItem->setAudioTrack(audioItem->getAudioTrack());
        addedItem->setAudioFile(audioItem->getAudioFile());
        addedItem->setPosition(cutPos);
        addedItem->setLength(itemEnd-cutPos);
        addedItem->setOffset(itemPos - cutPos + audioItem->getOffset());
        // 기존 아이템 길이조정.
        audioItem->setLength(cutPos-itemPos);
        // 트랙에 추가.
        track->add(addedItem);
        track->sortItemData();
        logManager.add({Log::debug,"BSEAudioItem.cut","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(addedItem->getAudioTrack().value) + " " +
            "item1 ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "item1 len: " + std::to_string(Algorithm::convertAudioFramesToSeconds(audioItem->getLength(),sampleRate)) + "s " +
            "item2 ID: " + std::to_string(addedItem->getObjectID().value) + " " +
            "item2 pos: " + std::to_string(Algorithm::convertAudioFramesToSeconds(addedItem->getPosition(),sampleRate)) + "s " +
            "item2 len: " + std::to_string(Algorithm::convertAudioFramesToSeconds(addedItem->getLength(),sampleRate)) + "s " +
            "item2 off: " + std::to_string(Algorithm::convertAudioFramesToSeconds(addedItem->getOffset(),sampleRate)) + "s " +
            "item2 file: " + addedItem->getAudioFile()->audioFile.getPath()
        });
        return addedItem->getObjectID().value;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.cut","catch exception"});
        return errorResult;
    }
}
_BSEBool _BSEAudioItem_isValid(_BSEObjectID itemID) {
    constexpr auto errorResult = false;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.isValid","no active project"});
            return errorResult;
        }
        // 아이템의 유효성을 반환합니다.
        auto item = project->getItemManager().getAudioItem({itemID});
        auto isValid = item.isValid();
        logManager.add({Log::debug,"BSEAudioItem.isValid","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "item ID: " + std::to_string(itemID) + " " +
            "state: " + (isValid ? "true" : "false")
        });
        return isValid;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.isValid","catch exception"});
        return errorResult;
    }
}
_BSESeconds _BSEAudioItem_getPosition(_BSEObjectID itemID) {
    constexpr auto errorResult = 0.0;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getPosition","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getPosition","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getPosition","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.getPosition","failed to get sample rate"});
            return errorResult;
        }
        // 아이템의 포지션을 반환합니다.
        auto position = Algorithm::convertAudioFramesToSeconds(audioItem->getPosition(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.getPosition","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "position: " + std::to_string(position) + "s"
        });
        return position;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getPosition","catch exception"});
        return errorResult;
    }
}
_BSESeconds _BSEAudioItem_getLength(_BSEObjectID itemID) {
    constexpr auto errorResult = 0.0;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getLength","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getLength","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getLength","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.getLength","failed to get sample rate"});
            return errorResult;
        }
        // 아이템의 길이를 반환합니다.
        auto length = Algorithm::convertAudioFramesToSeconds(audioItem->getLength(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.getLength","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "length: " + std::to_string(length) + "s"
        });
        return length;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getLength","catch exception"});
        return errorResult;
    }
}
_BSEPath _BSEAudioItem_getAudioFile(_BSEObjectID itemID) {
    constexpr auto errorResult = "";
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getAudioFile","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getAudioFile","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        auto audioFile = audioItem->getAudioFile();
        if (not audioFile.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getAudioFile","failed to get audio file",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(audioItem->getObjectID().value)
            });
            return errorResult;
        }
        auto& path = audioFile->audioFile.getPath();
        logManager.add({Log::debug,"BSEAudioItem.getAudioFile","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "file path: " + path
        });
        return path.c_str();
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getAudioFile","catch exception"});
        return errorResult;
    }
}
_BSESeconds _BSEAudioItem_getOffset(_BSEObjectID itemID) {
    constexpr auto errorResult = 0.0;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getOffset","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getOffset","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getOffset","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.getOffset","failed to get sample rate"});
            return errorResult;
        }
        // 아이템의 오프셋을 반환합니다.
        auto offset = Algorithm::convertAudioFramesToSeconds(audioItem->getOffset(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.getOffset","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "offset: " + std::to_string(offset) + "s"
        });
        return offset;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getOffset","catch exception"});
        return errorResult;
    }
}
void _BSEAudioItem_setOffset(_BSEObjectID itemID, _BSESeconds time) {
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setOffset","audio device not ready"});
            return;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setOffset","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setOffset","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.setOffset","failed to get sample rate"});
            return;
        }
        // 오프셋을 설정합니다.
        auto offset = Algorithm::convertSecondsToAudioFrames(time, sampleRate);
        audioItem->setOffset(offset);
        auto itemOff = Algorithm::convertAudioFramesToSeconds(audioItem->getOffset(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.setOffset","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "offset: " + std::to_string(itemOff) + "s"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.setOffset","catch exception"});
    }
}
void _BSEAudioItem_resizeLeftSide(_BSEObjectID itemID, _BSESeconds position) {
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeLeftSide","audio device not ready"});
            return;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeLeftSide","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeLeftSide","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 트랙을 가져옵니다.
        auto audioTrack = project->getTrackManager().getAudioTrack(audioItem->getAudioTrack());
        if (not audioTrack.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeLeftSide","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
                "item ID: " + std::to_string(audioItem->getObjectID().value)
            });
            return;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.resizeLeftSide","failed to get sample rate"});
            return;
        }
        // 아이템 설정변경.
        auto movePosition = Algorithm::convertSecondsToAudioFrames(position, sampleRate);
        auto prevPosition = audioItem->getPosition();
        audioItem->setPosition(movePosition);
        auto offset = prevPosition - movePosition;
        auto length = audioItem->getLength() + offset;
        if(length < 0) length = 0;
        audioItem->setLength(length);
        auto prevOffset = audioItem->getOffset();
        audioItem->setOffset(prevOffset + offset);
        audioTrack->sortItemData();
        auto itemPos = Algorithm::convertAudioFramesToSeconds(audioItem->getPosition(), sampleRate);
        auto itemLen = Algorithm::convertAudioFramesToSeconds(audioItem->getLength  (), sampleRate);
        auto itemOff = Algorithm::convertAudioFramesToSeconds(audioItem->getOffset  (), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.resizeLeftSide","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "position: " + std::to_string(itemPos) + "s " +
            "length: " + std::to_string(itemLen) + "s " +
            "offset: " + std::to_string(itemOff) + "s"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.resizeLeftSize","catch exception"});
    }
}
void _BSEAudioItem_resizeRightSide(_BSEObjectID itemID, _BSESeconds position) {
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeRightSide","audio device not ready"});
            return;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeRightSide","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeRightSide","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 트랙을 가져옵니다.
        auto audioTrack = project->getTrackManager().getAudioTrack(audioItem->getAudioTrack());
        if (not audioTrack.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.resizeRightSide","failed to get audio track",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
                "item ID: " + std::to_string(audioItem->getObjectID().value)
            });
            return;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.resizeRightSide","failed to get sample rate"});
            return;
        }
        // 아이템 설정을 변경합니다.
        auto movePosition = Algorithm::convertSecondsToAudioFrames(position, sampleRate);
        auto prevPosition = audioItem->getPosition();
        auto length       = movePosition - prevPosition;
        if(length < 0) length = 0;
        audioItem->setLength(length);
        auto itemLen = Algorithm::convertAudioFramesToSeconds(audioItem->getLength(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.resizeRightSide","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "length: " + std::to_string(itemLen) + "s"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.resizeRightSide","catch exception"});
    }
}
void _BSEAudioItem_setGainValue(_BSEObjectID itemID, _BSEPercent gainValue) {
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setGainValue","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setGainValue","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 값을 설정합니다.
        audioItem->setGainValue(gainValue/100.0);
        auto value = audioItem->getGainValue() * 100.0;
        logManager.add({Log::debug,"BSEAudioItem.setGainValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "value: " + std::to_string(value) + "%"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.setGainValue","catch exception"});
    }
}
_BSEPercent _BSEAudioItem_getGainValue(_BSEObjectID itemID) {
    constexpr auto errorResult = 0.0;
    try {
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getGainValue","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getGainValue","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 값을 반환합니다.
        auto value = audioItem->getGainValue()*100.0;
        logManager.add({Log::debug,"BSEAudioItem.getGainValue","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "value: " + std::to_string(value) + "%"
        });
        return value;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getGainValue","catch exception"});
        return errorResult;
    }
}
void _BSEAudioItem_setFadeIn(_BSEObjectID itemID, _BSESeconds time) {
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setFadeIn","audio device not ready"});
            return;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setFadeIn","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setFadeIn","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.setFadeIn","failed to get sample rate"});
            return;
        }
        // 값을 설정합니다.
        auto frames = Algorithm::convertSecondsToAudioFrames(time, sampleRate);
        audioItem->setFadeIn(frames);
        auto fadeIn = Algorithm::convertAudioFramesToSeconds(audioItem->getFadeIn(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.setFadeIn","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "time: " + std::to_string(fadeIn) + "s"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.setFadeIn","catch exception"});
    }
}
void _BSEAudioItem_setFadeOut(_BSEObjectID itemID, _BSESeconds time) {
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setFadeOut","audio device not ready"});
            return;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setFadeOut","no active project"});
            return;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.setFadeOut","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.setFadeOut","failed to get sample rate"});
            return;
        }
        // 값을 설정합니다.
        auto frames = Algorithm::convertSecondsToAudioFrames(time, sampleRate);
        audioItem->setFadeOut(frames);
        auto fadeOut = Algorithm::convertAudioFramesToSeconds(audioItem->getFadeOut(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.setFadeOut","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "time: " + std::to_string(fadeOut) + "s"
        });
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.setFadeOut","catch exception"});
    }
}
_BSESeconds _BSEAudioItem_getFadeIn(_BSEObjectID itemID) {
    constexpr auto errorResult = 0.0;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getFadeIn","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getFadeIn","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getFadeIn","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.getFadeIn","failed to get sample rate"});
            return errorResult;
        }
        // 값을 반환합니다.
        auto time = Algorithm::convertAudioFramesToSeconds(audioItem->getFadeIn(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.getFadeIn","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "time: " + std::to_string(time) + "s"
        });
        return time;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getFadeIn","catch exception"});
        return errorResult;
    }
}
_BSESeconds _BSEAudioItem_getFadeOut(_BSEObjectID itemID) {
    constexpr auto errorResult = 0.0;
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getFadeOut","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getFadeOut","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getFadeOut","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 샘플레이트를 가져옵니다.
        auto sampleRate = device.getSampleRate();
        if (sampleRate == 0.0) {
            logManager.add({Log::error,"BSEAudioItem.getFadeOut","failed to get sample rate"});
            return errorResult;
        }
        // 값을 반환합니다.
        auto time = Algorithm::convertAudioFramesToSeconds(audioItem->getFadeOut(), sampleRate);
        logManager.add({Log::debug,"BSEAudioItem.getFadeOut","",
            "project ID: " + std::to_string(project->getObjectID().value) + " " +
            "track ID: " + std::to_string(audioItem->getAudioTrack().value) + " " +
            "item ID: " + std::to_string(audioItem->getObjectID().value) + " " +
            "time: " + std::to_string(time) + "s"
        });
        return time;
    } catch (...) {
        logManager.add({Log::error,"BSEAudioItem.getFadeOut","catch exception"});
        return errorResult;
    }
}
_BSESampleValueArray _BSEAudioItem_getPCMSamples(_BSEObjectID itemID, _BSESeconds timeUnit) {
    constexpr auto errorResult = (_BSESampleValueArray){ .data = nullptr, .size = 0 };
    try {
        // 오디오기기를 가져옵니다.
        auto& device = deviceManager.getAudioDevice();
        if (not device.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getPCMSamples","audio device not ready"});
            return errorResult;
        }
        // 프로젝트를 가져옵니다.
        auto project = projectManager.getActivated();
        if (not project.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getPCMSamples","no active project"});
            return errorResult;
        }
        // 아이템을 가져옵니다.
        auto audioItem = project->getItemManager().getAudioItem({itemID});
        if (not audioItem.isValid()) {
            logManager.add({Log::error,"BSEAudioItem.getPCMSamples","failed to get audio item",
                "project ID: " + std::to_string(project->getObjectID().value) + " " +
                "item ID: " + std::to_string(itemID)
            });
            return errorResult;
        }
        // 버퍼의 샘플을 반환합니다.
        auto buffer = audioItem->extractPCMSamples(Algorithm::convertSecondsToAudioFrames(timeUnit, device.getSampleRate()));
        auto size = buffer.size();
        auto resultData = new _BSESampleValue[size];
        memcpy(resultData,buffer.data(),size*sizeof(_BSESampleValue));
        logManager.add({Log::debug,"BSEAudioItem.getPCMSamples","sample size",size});
        return {
            .data = resultData,
            .size = (_BSESize)size
        };
    } catch (...) {
        return errorResult;
    }
}
