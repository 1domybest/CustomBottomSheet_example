/*
 * Baund Studio Engine
 *
 * AndroidAudioDeviceSupport.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Support_AndroidAudioDeviceSupport_hpp
#define BaundStudioEngine_Sources_Support_AndroidAudioDeviceSupport_hpp
#ifdef __ANDROID__

#include "Sources/BaseLibrary/Definitions.hpp"
#include <aaudio/AAudio.h>
#include <thread>

namespace Baund::StudioEngine::Support {
    /* 프리콜백 */
    static aaudio_data_callback_result_t preCallback(AAudioStream* stream,
                                                     void*         userData,
                                                     void*         audioData,
                                                     int32_t       numFrames) {
        // 버퍼프레임의 수 확인.
        auto frames = static_cast<uint32_t*>(userData);
        (*frames)   = numFrames;
        //auto buffer = reinterpret_cast<float*>(audioData);
        //std::fill(buffer,buffer+numFrames*2,0.0);
        return AAUDIO_CALLBACK_RESULT_STOP;
    }
    /* 스트림빌더 닫기 */
    static inline auto close(AAudioStreamBuilder*& builder) {
        // 스트림빌더 닫기.
        if(builder != nullptr) {
            if(AAudioStreamBuilder_delete(builder) != AAUDIO_OK) goto error;
            builder = nullptr;
        }
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    /* 스트림빌더 열기 */
    static inline auto open(AAudioStreamBuilder*& builder, int numFrames) {
        // 스트림빌더 생성.
        if (AAudio_createStreamBuilder(&builder) != AAUDIO_OK) goto error;
        // 스트림빌더 포맷 설명.
        AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
        AAudioStreamBuilder_setSampleRate(builder, 44100);
        AAudioStreamBuilder_setChannelCount(builder, 2);
        AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_FLOAT);
        AAudioStreamBuilder_setBufferCapacityInFrames(builder, numFrames * 2);
        AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
        AAudioStreamBuilder_setFramesPerDataCallback(builder, numFrames);
        // 결과 반환.
    success:
        return true;
    error:
        builder = nullptr;
        return false;
    }
    /* 사전콜백 실행 */
    static inline auto runPreCallback(AAudioStreamBuilder* builder, uint32_t& numFrames) {
        if (not builder) throw Error::NullPointAccess();
        // 지역변수 선언.
        auto stream = (AAudioStream*) nullptr;
        // 출력스트림 열기.
        AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
        AAudioStreamBuilder_setDataCallback(builder,preCallback,&numFrames);
        if(AAudioStreamBuilder_openStream(builder,&stream) != AAUDIO_OK) {
            stream = nullptr;
            goto error;
        }
        // 콜백 실행.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(AAudioStream_requestStart(stream) != AAUDIO_OK) goto error;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(AAudioStream_requestStop(stream) != AAUDIO_OK) goto error;
        AAudioStream_requestFlush(stream);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // 결과 반환.
    success:
        if(stream) AAudioStream_close(stream);
        return true;
    error:
        if(stream) AAudioStream_close(stream);
        return false;
    }
    /* 입출력콜백 설정 */
    static inline auto setIOCallback(AAudioStreamBuilder* builder,AAudioStream_dataCallback callback, void* userData) {
        if (not builder) throw Error::NullPointAccess();
        AAudioStreamBuilder_setDataCallback(builder,callback,userData);
        return true;
    }
    /* 에러콜백 설정 */
    static inline auto setErrorCallback(AAudioStreamBuilder* builder,AAudioStream_errorCallback callback, void* userData) {
        if (not builder) throw Error::NullPointAccess();
        AAudioStreamBuilder_setErrorCallback(builder,callback,userData);
        return true;
    }
    /* 오디오스트림 닫기 */
    static inline auto close(AAudioStream*& stream) {
        if (not stream) throw Error::NullPointAccess();
        // 오디오스트림 닫기.
        if(stream != nullptr)
            if(AAudioStream_close(stream) != AAUDIO_OK) goto error;
        // 결과 반환.
    success:
        stream = nullptr;
        return true;
    error:
        return false;
    }
    /* 오디오스트림 열기 */
    static inline auto open(AAudioStreamBuilder* builder,AAudioStream*& stream,aaudio_direction_t direction) {
        if (not builder) throw Error::NullPointAccess();
        // 샘플레이트리스트 작성.
        int sampleRateList[7] = { 48000, 44100, 32000, 22050, 16000, 11025, 8000 };
        // 오디오스트림 열기.
        AAudioStreamBuilder_setDirection(builder,direction);
        if (AAudioStreamBuilder_openStream(builder, &stream) != AAUDIO_OK) {
            for(auto sampleRate : sampleRateList) {
                //AAudioStream_requestStop(stream);
                if (stream) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    AAudioStream_requestFlush(stream);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    AAudioStream_close(stream);
                }
                AAudioStreamBuilder_setSampleRate(builder, sampleRate);
                if (AAudioStreamBuilder_openStream(builder, &stream) == AAUDIO_OK) goto success;
            }
            goto error;
        }
        // 결과 반환.
    success:
        return true;
    error:
        stream = nullptr;
        return false;
    }
    /* 기기샘플레이트 가져오기 */
    static inline auto getSampleRate(AAudioStream* stream, double& sampleRate) {
        if (not stream) throw Error::NullPointAccess();
        sampleRate = AAudioStream_getSampleRate(stream);
        return true;
    }
    /* 작동상태 확인 */
    static inline auto isRunning(AAudioStream* stream) {
        if (not stream) throw Error::NullPointAccess();
        auto state = AAudioStream_getState(stream);
        return state == AAUDIO_STREAM_STATE_STARTED or state == AAUDIO_STREAM_STATE_STARTING;
    }
    /* 오디오스트림 시작 */
    static inline auto start(AAudioStream* stream) {
        if (not stream) throw Error::NullPointAccess();
        if((AAudioStream_requestStart(stream)) != AAUDIO_OK) goto error;
    success:
        return true;
    error:
        return false;
    }
    /* 오디오스트림 정지 */
    static inline auto stop(AAudioStream* stream) {
        if (not stream) throw Error::NullPointAccess();
        if(AAudioStream_requestStop(stream) != AAUDIO_OK) goto error;
        AAudioStream_requestFlush(stream);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    success:
        return true;
    error:
        return false;
    }
    static inline auto setID(AAudioStreamBuilder* builder, int id) {
        if (not builder) throw Error::NullPointAccess();
        if (id < 0) return false;
        AAudioStreamBuilder_setDeviceId(builder,id);
        return true;
    }
}

#endif//__ANDROID__
#endif//BaundStudioEngine_Sources_Support_AndroidAudioDeviceSupport_hpp
