/*
 * Baund Studio Engine
 *
 * iOSAudioFileSupport.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Support_iOSAudioFileSupport_hpp
#define BaundStudioEngine_Sources_Support_iOSAudioFileSupport_hpp
#ifdef __APPLE__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <string>

namespace Baund::StudioEngine::Support {
    // 오디오파일 열기.
    static inline auto open(ExtAudioFileRef& fileRef, std::string const& inputPath) {
        // 경로 가져오기.
        auto filePath = CFStringCreateWithCString(kCFAllocatorDefault,inputPath.c_str(),kCFStringEncodingUTF8);
        auto fileURL  = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,filePath,kCFURLPOSIXPathStyle,false);
        // 오디오파일 열기.
        auto result   = true;
        if (ExtAudioFileOpenURL(fileURL,&fileRef) != noErr) result = false;
        // 경로 해제.
        if (filePath) CFRelease(filePath);
        if (fileURL)  CFRelease(fileURL);
        // 결과 반환.
        if (result == false) goto error;
    success:
        return true;
    error:
        fileRef = nullptr;
        return false;
    }
    // 오디오파일 해제.
    static inline auto close(ExtAudioFileRef& fileRef) {
        // 오디오파일 해제.
        if (fileRef != nullptr) {
            if (ExtAudioFileDispose(fileRef) != noErr) goto error;
        }
        // 결과 반환.
    success:
        fileRef = nullptr;
        return true;
    error:
        return false;
    }
    // 오디오포맷 가져오기.
    static inline auto get(ExtAudioFileRef fileRef, AudioStreamBasicDescription& desc) {
        // 오디오포맷 가져오기.
        auto propSize = (UInt32) sizeof(AudioStreamBasicDescription);
        if (ExtAudioFileGetProperty(fileRef,kExtAudioFileProperty_FileDataFormat,&propSize,&desc) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        desc = { };
        return false;
    }
    // 오디오포맷 쓰기.
    static inline auto set(ExtAudioFileRef fileRef, AudioStreamBasicDescription desc) {
        // 오디오포맷 쓰기.
        if (ExtAudioFileSetProperty(fileRef,kExtAudioFileProperty_ClientDataFormat,sizeof(desc),&desc) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    struct SampleRate  { Float64 value = 0.0; };
    struct NumChannels { UInt32  value = 0; };
    struct NumFrames   { SInt64  value = 0; };
    // 샘플레이트 가져오기.
    static inline auto get(ExtAudioFileRef fileRef, SampleRate& sampleRate) {
        // 오디오포맷 가져오기.
        auto desc = (AudioStreamBasicDescription) {};
        if (not get(fileRef,desc)) goto error;
        // 결과 반환.
    success:
        sampleRate.value = desc.mSampleRate;
        return true;
    error:
        sampleRate.value = 0.0;
        return false;
    }
    // 체널의 수 가져오기.
    static inline auto get(ExtAudioFileRef fileRef, NumChannels& numChannels) {
        // 오디오포맷 가져오기.
        auto desc = (AudioStreamBasicDescription) {};
        if (not get(fileRef,desc)) goto error;
        // 결과 반환.
    success:
        numChannels.value = desc.mChannelsPerFrame;
        return true;
    error:
        numChannels.value = 0;
        return false;
    }
    // 프레임의 수 가져오기.
    static inline auto get(ExtAudioFileRef fileRef, NumFrames& length) {
        // 프레임의 수 가져오기.
        auto propSize = (UInt32) sizeof(SInt64);
        if (ExtAudioFileGetProperty(fileRef,kExtAudioFileProperty_FileLengthFrames,&propSize,&length.value) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        length.value = 0;
        return false;
    }
    // 오디오파일 읽기.
    template< typename SampleValueType >
    static inline auto read(ExtAudioFileRef  fileRef,
                            SampleRate       sampleRate,
                            NumChannels      numChannels,
                            NumFrames        numFrames,
                            SampleValueType* data) {
        // 지역변수 선언.
        auto clientDesc = (AudioStreamBasicDescription) {};
        auto bufferList = (AudioBufferList) {};
        auto frames     = (UInt32) numFrames.value;
        // 출력포맷 설명.
        clientDesc.mSampleRate       = sampleRate.value;
        clientDesc.mFormatID         = kAudioFormatLinearPCM;
        clientDesc.mFormatFlags      = kAudioFormatFlagsNativeFloatPacked;
        clientDesc.mChannelsPerFrame = numChannels.value;
        clientDesc.mBytesPerFrame    = clientDesc.mChannelsPerFrame * sizeof(SampleValueType);
        clientDesc.mFramesPerPacket  = 1;
        clientDesc.mBytesPerPacket   = clientDesc.mBytesPerFrame * clientDesc.mFramesPerPacket;
        clientDesc.mBitsPerChannel   = sizeof(SampleValueType)*8;
        clientDesc.mReserved         = 0;
        // 오디오버퍼 설정.
        bufferList.mNumberBuffers = 1;
        bufferList.mBuffers[0].mDataByteSize   = (UInt32)(clientDesc.mBytesPerFrame * numFrames.value);
        bufferList.mBuffers[0].mNumberChannels = numChannels.value;
        bufferList.mBuffers[0].mData           = data;
        // 출력포맷 쓰기.
        if (not set(fileRef,clientDesc)) goto error;
        // 오디오데이터 읽기.
        if (ExtAudioFileRead(fileRef,&frames,&bufferList) != noErr) goto error;
        // 결과 반환.
    success:
        return true;
    error:
        return false;
    }
    // 오디오파일 생성.
    static inline auto create(ExtAudioFileRef&            fileRef,
                              AudioFileTypeID             fileType,
                              AudioStreamBasicDescription desc,
                              std::string const&          path) {
        // 경로 가져오기.
        auto filePath = CFStringCreateWithCString(kCFAllocatorDefault,path.c_str(),kCFStringEncodingUTF8);
        auto fileURL  = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,filePath,kCFURLPOSIXPathStyle,false);
        // 오디오파일 생성.
        auto result   = true;
        if (ExtAudioFileCreateWithURL(fileURL,fileType,&desc,NULL,kAudioFileFlags_EraseFile,&fileRef) != noErr) result = false;
        // 경로 해제.
        if (filePath) CFRelease(filePath);
        if (fileURL ) CFRelease(fileURL);
        // 결과 반환.
        if (result == false) goto error;
    success:
        return true;
    error:
        fileRef = nullptr;
        return false;
    }
    // 오디오파일 쓰기.
    template< typename SampleValueType >
    static inline auto write(std::string const&   path,
                             AudioFileTypeID  fileType,
                             SampleRate       sampleRate,
                             NumChannels      numChannels,
                             NumFrames        numFrames,
                             SampleValueType* data) {
        // 지역변수 선언.
        auto fileRef    = (ExtAudioFileRef) nullptr;
        auto fileDesc   = (AudioStreamBasicDescription) {};
        auto clientDesc = (AudioStreamBasicDescription) {};
        auto isM4A      = fileType == kAudioFileM4AType;
        auto bufferList = (AudioBufferList) {};
        // 출력포맷 설명.
        fileDesc.mSampleRate       = sampleRate.value;
        fileDesc.mFormatID         = isM4A ? kAudioFormatMPEG4AAC : kAudioFormatLinearPCM;
        fileDesc.mFormatFlags      = isM4A ? kMPEG4Object_AAC_Main : (kAudioFormatFlagIsSignedInteger|kAudioFormatFlagIsPacked);
        fileDesc.mChannelsPerFrame = numChannels.value;
        fileDesc.mBytesPerFrame    = isM4A ? 0 : fileDesc.mChannelsPerFrame * 16/8;
        fileDesc.mFramesPerPacket  = isM4A ? 1024 : 1;
        fileDesc.mBytesPerPacket   = isM4A ? 0 : fileDesc.mBytesPerFrame * fileDesc.mFramesPerPacket;
        fileDesc.mBitsPerChannel   = isM4A ? 0 : 16;
        fileDesc.mReserved         = 0;
        // 입력포맷 설명.
        clientDesc.mSampleRate       = sampleRate.value;
        clientDesc.mFormatID         = kAudioFormatLinearPCM;
        clientDesc.mFormatFlags      = kAudioFormatFlagsNativeFloatPacked;
        clientDesc.mChannelsPerFrame = numChannels.value;
        clientDesc.mBytesPerFrame    = clientDesc.mChannelsPerFrame * sizeof(SampleValueType);
        clientDesc.mFramesPerPacket  = 1;
        clientDesc.mBytesPerPacket   = clientDesc.mBytesPerFrame * clientDesc.mFramesPerPacket;
        clientDesc.mBitsPerChannel   = sizeof(SampleValueType) * 8;
        clientDesc.mReserved         = 0;
        // 오디오버퍼 설정.
        bufferList.mNumberBuffers = 1;
        bufferList.mBuffers[0].mDataByteSize   = (UInt32)(clientDesc.mBytesPerFrame * numFrames.value);
        bufferList.mBuffers[0].mNumberChannels = numChannels.value;
        bufferList.mBuffers[0].mData           = data;
        // 오디오파일 생성.
        if (not create(fileRef, fileType, fileDesc, path)) goto error;
        // 입력포맷 설정.
        if (not set(fileRef, clientDesc)) goto error;
        // 오디오파일 쓰기.
        if (ExtAudioFileWrite(fileRef,(UInt32)numFrames.value,&bufferList) != noErr) goto error;
        // 결과 반환.
    success:
        if (fileRef != nullptr) ExtAudioFileDispose(fileRef);
        return true;
    error:
        if (fileRef != nullptr) ExtAudioFileDispose(fileRef);
        return false;
    }
    // 오디오트랙 추출.
    auto extractM4A(std::string const& iPath, std::string const& oPath) -> bool;
    // .mp4형식여부 확인.
    auto isMP4Video(std::string const& path) -> bool;
    // 실시간 레코더.
    struct RTRecorder {
        ExtAudioFileRef audioFile = nullptr;
        UInt32          position  = 0;
    };
    static inline auto close(RTRecorder& recorder) {
        if (recorder.audioFile) {
            close(recorder.audioFile);
        }
        recorder = RTRecorder();
    }
    template< typename SampleValueType >
    static inline auto ready(RTRecorder& recoder, std::string const& filePath, double sampleRate, long numChannels) -> bool {
        auto fileDesc   = (AudioStreamBasicDescription) {};
        auto clientDesc = (AudioStreamBasicDescription) {};
        fileDesc.mSampleRate       = sampleRate;
        fileDesc.mFormatID         = kAudioFormatMPEG4AAC;
        fileDesc.mFormatFlags      = kMPEG4Object_AAC_Main;
        fileDesc.mChannelsPerFrame = (UInt32)numChannels;
        fileDesc.mBytesPerFrame    = 0;
        fileDesc.mFramesPerPacket  = 1024;
        fileDesc.mBytesPerPacket   = 0;
        fileDesc.mBitsPerChannel   = 0;
        fileDesc.mReserved         = 0;
        clientDesc.mSampleRate       = sampleRate;
        clientDesc.mFormatID         = kAudioFormatLinearPCM;
        clientDesc.mFormatFlags      = kAudioFormatFlagsNativeFloatPacked;
        clientDesc.mChannelsPerFrame = (UInt32)numChannels;
        clientDesc.mBytesPerFrame    = clientDesc.mChannelsPerFrame * sizeof(SampleValueType);
        clientDesc.mFramesPerPacket  = 1;
        clientDesc.mBytesPerPacket   = clientDesc.mBytesPerFrame * clientDesc.mFramesPerPacket;
        clientDesc.mBitsPerChannel   = sizeof(SampleValueType) * 8;
        clientDesc.mReserved         = 0;

        if (not create(recoder.audioFile,kAudioFileM4AType,fileDesc,filePath)) goto error;
        if (not set(recoder.audioFile, clientDesc)) goto error;
    success:
        return true;
    error:
        close(recoder.audioFile);
        return false;
    }
    static inline auto start(RTRecorder& recorder) -> bool {
        return true;
    }
    static inline auto stop(RTRecorder& recorder) -> bool {
        return true;
    }
    template< typename SampleValueType >
    static inline auto writeAACFile(RTRecorder& recorder, SampleValueType* buffer, long numFrames, long numChannels) -> bool {
        auto bufferList = (AudioBufferList){};
        bufferList.mNumberBuffers = 1;
        bufferList.mBuffers[0].mDataByteSize   = (UInt32)(numChannels*numFrames*sizeof(SampleValueType));
        bufferList.mBuffers[0].mNumberChannels = (UInt32)numChannels;
        bufferList.mBuffers[0].mData           = buffer;
        if (ExtAudioFileWrite(recorder.audioFile,(UInt32)(numFrames),&bufferList) != noErr) goto error;
        
    success:
        return true;
    error:
        return false;
    }
}

#endif//__APPLE__
#endif//BaundStudioEngine_Sources_Support_iOSAudioFileSupport_hpp
