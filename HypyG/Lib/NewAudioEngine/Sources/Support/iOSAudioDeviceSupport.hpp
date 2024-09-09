/*
 * Baund Studio Engine
 *
 * iOSAudioDeviceSupport.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Support_iOSAudioDeviceSupport_hpp
#define BaundStudioEngine_Sources_Support_iOSAudioDeviceSupport_hpp
#ifdef __APPLE__

#include "Sources/BaseLibrary/Definitions.hpp"
#include <AudioToolbox/AudioToolbox.h>
#include <thread>

namespace Baund::StudioEngine::Support {
    /* 프리콜백 */
    static OSStatus preCallback(void*                       userData,
                                AudioUnitRenderActionFlags* ioActionFlags,
                                const AudioTimeStamp*,
                                UInt32,
                                UInt32                      inNumberFrames,
                                AudioBufferList*) {
        // 버퍼프레임의 수 확인.
        auto numFrames   = static_cast<UInt32*>(userData);
        (*numFrames)     = inNumberFrames;
        (*ioActionFlags) = kAudioUnitRenderAction_OutputIsSilence;
        return noErr;
    }
    /* 오디오유닛 닫기 */
    static inline auto close(AudioUnit& audioUnit) {
        // 오디오유닛 닫기.
        if (audioUnit != nullptr) {
            if(AudioComponentInstanceDispose(audioUnit) != noErr) goto error;
            audioUnit = nullptr;
        }
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 오디오기기 열기 */
#if TARGET_OS_IPHONE
    constexpr auto componentSubType = kAudioUnitSubType_RemoteIO;
#else
    constexpr auto componentSubType = kAudioUnitSubType_HALOutput;
#endif//TARGET_OS_IPHONE
    static inline auto open(AudioUnit& audioUnit) {
        // 오디오기기 설명.
        constexpr auto desc = AudioComponentDescription {
            .componentType         = kAudioUnitType_Output,
            .componentSubType      = componentSubType,
            .componentManufacturer = kAudioUnitManufacturer_Apple,
            .componentFlags        = 0,
            .componentFlagsMask    = 0
        };
        // 오디오기기 열기.
        auto component = AudioComponentFindNext(NULL, &desc);
        if (AudioComponentInstanceNew(component, &audioUnit) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        audioUnit = nullptr;
        return false;
    }
    /* 입출력기기 사용설정 */
    static inline auto enableIO(AudioUnit audioUnit) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 입출력기기 사용설정.
        constexpr auto inputBus    = 1;
        constexpr auto outputBus   = 0;
        UInt32 flag = 1; // Enable : true
        if (AudioUnitSetProperty(audioUnit,
                                 kAudioOutputUnitProperty_EnableIO,
                                 kAudioUnitScope_Input,
                                 inputBus,
                                 &flag,
                                 sizeof(flag)) != noErr) goto error;
        if (AudioUnitSetProperty(audioUnit,
                                 kAudioOutputUnitProperty_EnableIO,
                                 kAudioUnitScope_Output,
                                 outputBus,
                                 &flag,
                                 sizeof(flag)) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 오디오유닛 초기화 */
    static inline auto init(AudioUnit audioUnit) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 오디오유닛 초기화.
        if (AudioUnitInitialize(audioUnit) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 오디오기기의 샘플레이트 가져오기 */
    static inline auto getDeviceSampleRate(AudioUnit audioUnit, Float64& sampleRate) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 샘플레이트 가져오기.
        auto sampleRateSize = (UInt32) sizeof(sampleRate);
        if (AudioUnitGetProperty(audioUnit,
                                 kAudioUnitProperty_SampleRate,
                                 kAudioUnitScope_Output,
                                 0, // output bus
                                 &sampleRate,
                                 &sampleRateSize) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        sampleRate = 0.0;
        return false;
    }
    /* 스트림포맷 설정 */
    static inline auto setStreamFormat(AudioUnit audioUnit, Float64 const& sampleRate) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 포맷 설명.
        constexpr auto streamChannelSize = 2;
        auto format = (AudioStreamBasicDescription) {
            .mFormatID         = kAudioFormatLinearPCM,
            .mFormatFlags      = kAudioFormatFlagIsFloat,
            .mSampleRate       = sampleRate,
            .mBytesPerPacket   = sizeof(float) * streamChannelSize,
            .mFramesPerPacket  = 1,
            .mBytesPerFrame    = sizeof(float) * streamChannelSize,
            .mChannelsPerFrame = streamChannelSize,
            .mBitsPerChannel   = sizeof(float) * 8,
            .mReserved         = 0
        };
        // 입력스트림포맷 설정.
        if (AudioUnitSetProperty(audioUnit,
                                 kAudioUnitProperty_StreamFormat,
                                 kAudioUnitScope_Output,
                                 1, // input bus
                                 &format,
                                 sizeof(format)) != noErr) goto error;
        // 출력스트림포맷 설정.
        if (AudioUnitSetProperty(audioUnit,
                                 kAudioUnitProperty_StreamFormat,
                                 kAudioUnitScope_Input,
                                 0, // output bus
                                 &format,
                                 sizeof(format)) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 오디오기기의 버퍼크기 설정 */
#if TARGET_OS_IPHONE
    auto setBufferSize(UInt32 bufferSize) -> bool;
#else
#ifndef BSE_DEBUG_MODE
#define BSE_DEBUG_MODE false
#endif//BSE_DEBUG_MODE
    auto setBufferSize(UInt32 bufferSize) -> bool {
        constexpr auto debugMode = BSE_DEBUG_MODE;
        // 지역변수 선언.
        auto propSize      = UInt32();
        auto iAudioObject  = AudioObjectID();
        auto oAudioObject  = AudioObjectID();
        auto iBufferFrames = UInt32();
        auto oBufferFrames = UInt32();
        auto address      = AudioObjectPropertyAddress {
            .mSelector = NULL,
            .mScope    = kAudioObjectPropertyScopeGlobal,
            .mElement  = kAudioObjectPropertyElementMain
        };
        // 입력장치 가져오기.
        propSize = sizeof(iAudioObject);
        address.mSelector = kAudioHardwarePropertyDefaultInputDevice;
        if (AudioObjectGetPropertyData(kAudioObjectSystemObject,&address,0,NULL,&propSize,&iAudioObject) != noErr) goto error;
        // 입력버퍼크기 가져오기.
        address.mSelector = kAudioDevicePropertyBufferFrameSize;
        if constexpr (debugMode) {
            propSize = (UInt32) sizeof(iBufferFrames);
            if (AudioObjectGetPropertyData(iAudioObject,&address,0,NULL,&propSize,&iBufferFrames) != noErr) goto error;
        }
        // 입력버퍼크기 쓰기.
        if (AudioObjectSetPropertyData(iAudioObject,&address,0,NULL,sizeof(bufferSize),&bufferSize) != noErr) goto error;
        // 출력장치 가져오기.
        propSize = sizeof(iAudioObject);
        address.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
        if (AudioObjectGetPropertyData(kAudioObjectSystemObject,&address,0,NULL,&propSize,&oAudioObject) != noErr) goto error;
        // 출력버퍼크기 가져오기.
        address.mSelector = kAudioDevicePropertyBufferFrameSize;
        if constexpr (debugMode) {
            propSize = sizeof(oBufferFrames);
            if (AudioObjectGetPropertyData(oAudioObject,&address,0,NULL,&propSize,&oBufferFrames) != noErr) goto error;
        }
        // 출력버퍼크기 쓰기.
        if (AudioObjectSetPropertyData(oAudioObject,&address,0,NULL,sizeof(bufferSize),&bufferSize) != noErr) goto error;
    success:
        return true;
    error:
        return false;
    }
#endif//TARGET_OS_IPHONE
    /* 사전콜백 실행 */
    static inline auto runPreCallback(AudioUnit audioUnit, UInt32& numFrames) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 콜백 설정.
        auto callbackStruct = (AURenderCallbackStruct) {};
        callbackStruct.inputProc       = preCallback;
        callbackStruct.inputProcRefCon = &numFrames;
        if (AudioUnitSetProperty(audioUnit,
                                 kAudioUnitProperty_SetRenderCallback,
                                 kAudioUnitScope_Global,
                                 0,
                                 &callbackStruct,
                                 sizeof(callbackStruct)) != noErr) goto error;
        // 콜백 실행.
        if (AudioOutputUnitStart(audioUnit) != noErr) goto error;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (AudioOutputUnitStop(audioUnit) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 입출력콜백 설정 */
    static inline auto setIOCallback(AudioUnit audioUnit, AURenderCallback callback, void* userData) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 콜백 설정.
        auto callbackStruct = (AURenderCallbackStruct) {};
        callbackStruct.inputProc       = callback;
        callbackStruct.inputProcRefCon = userData;
        if (AudioUnitSetProperty(audioUnit,
                                 kAudioUnitProperty_SetRenderCallback,
                                 kAudioUnitScope_Global,
                                 0,
                                 &callbackStruct,
                                 sizeof(callbackStruct)) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 에러콜백 설정 */
    static inline auto setErrorCallback(AudioUnit audioUnit, AudioUnitPropertyListenerProc callback, void* userData) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 콜백 설정.
        if (AudioUnitAddPropertyListener(audioUnit,
                                         kAudioUnitProperty_StreamFormat,
                                         callback,
                                         userData) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 작동상태 반환 */
    static inline auto isRunning(AudioUnit audioUnit) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 작동상태 가져오기.
        auto running  = (UInt32) 0;
        auto propSize = (UInt32) sizeof(running);
        if (AudioUnitGetProperty(audioUnit,
                                 kAudioOutputUnitProperty_IsRunning,
                                 kAudioUnitScope_Global,
                                 0,
                                 &running,
                                 &propSize) != noErr) goto error;
        // 결과 반환.
    success:
        return running != 0;
    error:
        return false;
    }
    /* 오디오유닛 시작 */
    static inline auto start(AudioUnit audioUnit) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 오디오유닛 시작.
        if (AudioOutputUnitStart(audioUnit) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 오디오유닛 정지 */
    static inline auto stop(AudioUnit audioUnit) {
        if (not audioUnit) throw Error::NullPointAccess();
        // 오디오유닛 정지.
        if (AudioOutputUnitStop(audioUnit) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
}

#endif//__APPLE__
#endif//BaundStudioEngine_Sources_Support_iOSAudioDeviceSupport_hpp
