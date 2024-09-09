/*
 * Baund Studio Engine
 *
 * AndroidAudioFileSupport.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Android_AndroidAudioFileSupport_hpp
#define BaundStudioEngine_Sources_Android_AndroidAudioFileSupport_hpp
#ifdef __ANDROID__

#include <fstream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaMuxer.h>
#include <thread>

namespace Baund::StudioEngine::Support {
    static constexpr auto waitTimeToCodecStop     = 100; // ms
    static constexpr auto waitTimeToMuxer         = 0; // ms
    static constexpr auto waitTimeToDequeueBuffer = 1000; // us
    static inline auto isValid(std::string filePath) {
        auto file = std::ifstream(filePath);
        auto result = file.is_open();
        file.close();
        return result;
    }
    static inline auto createFile(std::string filePath) {
        auto file = std::ofstream(filePath);
        auto result = file.is_open();
        file.close();
        return result;
    }
    struct FileDescription {
        static constexpr auto inValid = -1;
        int id = inValid;
        int length = 0;
    };
    static inline auto getFileDescToRead(std::string filePath) {
        auto result = (FileDescription){};
        auto id = ::open(filePath.c_str(),O_RDONLY);
        if (id > FileDescription::inValid) {
            result.id = id;
            result.length = lseek(id,0,SEEK_END);
        }
        return result;
    }
    static inline auto getFileDescToWrite(std::string filePath) {
        auto result = (FileDescription){};
        if (not createFile(filePath)) return result;
        auto id = ::open(filePath.c_str(),O_WRONLY|O_CREAT,O_TRUNC);
        if (id > FileDescription::inValid) {
            result.id = id;
            result.length = 0;
        }
        return result;
    }
    static inline auto isValid(FileDescription fileDesc) {
        return fileDesc.id > FileDescription::inValid;
    }
    static inline auto close(FileDescription& fileDesc) {
        if (isValid(fileDesc)) ::close(fileDesc.id);
        fileDesc = (FileDescription){};
    }
    struct Extractor {
        AMediaExtractor* ptr = nullptr;
        AMediaFormat* format = nullptr;
        char const* mime = nullptr;
        int index = -1;
        int32_t sampleRate = 0;
        int32_t channelSize = 0;
        int64_t frameSize = 0;
    };
    static inline auto createExtractor(FileDescription fileDesc) {
        constexpr auto errorResult = (Extractor){};
        if(not isValid(fileDesc)) return errorResult;
        auto extractor = AMediaExtractor_new();
        if(not extractor) return errorResult;
        auto aMediaError = AMediaExtractor_setDataSourceFd(extractor, fileDesc.id, 0, fileDesc.length);
        if(aMediaError) return errorResult;
        auto trackFormat = AMediaFormat_new();
        auto trackMime = (char const*) nullptr;
        auto trackIndex = -1;
        for(auto i=0; i<AMediaExtractor_getTrackCount(extractor); i++) {
            auto mime = (char const*) nullptr;
            auto format = AMediaExtractor_getTrackFormat(extractor, i);
            AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime);
            if (strncmp(mime, "audio/", 6) == 0) {
                AMediaExtractor_selectTrack(extractor,i);
                AMediaFormat_copy(trackFormat, format);
                trackMime = mime;
                trackIndex = i;
                break;
            }
            AMediaFormat_delete(format);
        }
        if(trackIndex < 0) return errorResult;
        if(std::string(trackMime) == "audio/raw") {

        } else if (std::string(trackMime) == "audio/mpeg") {

        } else if (std::string(trackMime) == "audio/mp4a-latm") {
            AMediaFormat_setInt32(trackFormat,AMEDIAFORMAT_KEY_MAX_INPUT_SIZE,8192);
        } else return errorResult;

        auto trackSampleRate = (int32_t)0;
        auto trackChannelSize = (int32_t)0;
        auto trackDuration = (int64_t)0;
        AMediaFormat_getInt32(trackFormat,AMEDIAFORMAT_KEY_SAMPLE_RATE,&trackSampleRate);
        AMediaFormat_getInt32(trackFormat,AMEDIAFORMAT_KEY_CHANNEL_COUNT,&trackChannelSize);
        AMediaFormat_getInt64(trackFormat,AMEDIAFORMAT_KEY_DURATION,&trackDuration);

        return Extractor {
                .ptr = extractor,
                .format = trackFormat,
                .mime = trackMime,
                .index = trackIndex,
                .sampleRate = trackSampleRate,
                .channelSize = trackChannelSize,
                .frameSize = trackDuration*trackSampleRate/1'000'000
        };
    }
    static inline auto isValid(Extractor extractor) {
        return bool {
                extractor.ptr
                and extractor.format
                and extractor.index >= 0
                and extractor.sampleRate > 0
                and (extractor.channelSize == 1 or extractor.channelSize == 2)
                and (extractor.mime == std::string("audio/raw")
                     or extractor.mime == std::string("audio/mpeg")
                     or extractor.mime == std::string("audio/mp4a-latm"))
        };
    }
    static inline auto close(Extractor& extractor) {
        if(extractor.ptr) AMediaExtractor_delete(extractor.ptr);
        if(extractor.format) AMediaFormat_delete(extractor.format);
        extractor = (Extractor){};
    }
    static inline auto readPCMFile(Extractor& extractor) {
        auto resultBuffer = std::vector<int16_t>();
        for(auto eof = false; not eof; ) {
            auto sampleSize = AMediaExtractor_getSampleSize(extractor.ptr);
            auto byteSample = std::vector<uint8_t>(sampleSize);
            auto readSize = AMediaExtractor_readSampleData(extractor.ptr,byteSample.data(),sampleSize);
            auto readBuffer = reinterpret_cast<int16_t*>(byteSample.data());
            for(auto i=0; i<readSize/sizeof(int16_t); i++) {
                resultBuffer.push_back(readBuffer[i]);
            }
            if(not AMediaExtractor_advance(extractor.ptr)) {
                eof = true;
            }
        }
        return resultBuffer;
    }
    static inline auto convertToFloatAudio(std::vector<int16_t> const& input) {
        auto resultBuffer = std::vector<float>(input.size());
        for(auto i=0; i<input.size(); i++) {
            if(input[i] == 0) resultBuffer[i] = static_cast<float>(input[i]);
            else if(input[i] > 0) resultBuffer[i] = +(static_cast<float>(input[i])/std::numeric_limits<int16_t>::max());
            else if(input[i] < 0) resultBuffer[i] = -(static_cast<float>(input[i])/std::numeric_limits<int16_t>::min());
            else resultBuffer[i] = 0.f;
        }
        return resultBuffer;
    }
    template< typename _T>
    static inline auto convertToStereo(std::vector<_T> const& input) {
        auto resultBuffer = std::vector<_T>(input.size()*2);
        for(auto i=0; i<input.size(); i++) {
            resultBuffer[i*2  ] = input[i];
            resultBuffer[i*2+1] = input[i];
        }
        return resultBuffer;
    }
    static inline auto resampleAudio(std::vector<float> const& input, int numChannels, double iSampleRate, double oSampleRate) {
        auto ratio = oSampleRate / iSampleRate;
        auto iFrames = (int)(input.size()/numChannels);
        auto oFrames = (int)(iFrames*ratio+0.5);

        auto resultBuffer = std::vector<float>(oFrames*numChannels);
        for(auto channel=0; channel<numChannels;channel++) {
            for(auto i=0;i<oFrames;i++) {
                double indexInput = static_cast<double>(i) * iSampleRate / oSampleRate;
                auto index1 = static_cast<int>(indexInput);
                auto index2 = std::min(index1+1,iFrames-1);
                double fraction = indexInput - index1;
                resultBuffer[i*numChannels+channel] = input[index1*numChannels+channel]*(1.0-fraction)+input[index2*numChannels+channel]*fraction;
            }
        }
        return resultBuffer;
    }
    static inline auto readCodecFile(Extractor& extractor) {
        auto resultBuffer = std::vector<int16_t>();
        auto codec = AMediaCodec_createDecoderByType(extractor.mime);
        AMediaCodec_configure(codec, extractor.format, nullptr, nullptr, 0);
        AMediaCodec_start(codec);
        for(auto sawInputEOF = false, sawOutputEOF = false; not sawOutputEOF; ) {
            if(not sawInputEOF) {
                auto bufferIndex = AMediaCodec_dequeueInputBuffer(codec, 10000);
                if(bufferIndex >= 0) {
                    auto bufferSize = (size_t)0;
                    auto writeSize = (size_t)0;
                    auto time = AMediaExtractor_getSampleTime(extractor.ptr);
                    auto flags = AMediaExtractor_getSampleFlags(extractor.ptr);
                    auto inputBuffer = AMediaCodec_getInputBuffer(codec, bufferIndex, &bufferSize);
                    for(auto fillSize = 0; fillSize <= bufferSize; ) {
                        auto sampleSize = AMediaExtractor_getSampleSize(extractor.ptr);
                        auto remaningSize = bufferSize - fillSize;
                        if(sampleSize <= remaningSize) {
                            fillSize += AMediaExtractor_readSampleData(extractor.ptr, inputBuffer+fillSize, remaningSize);
                            if(not AMediaExtractor_advance(extractor.ptr)) {
                                writeSize = fillSize;
                                flags |= AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
                                sawInputEOF = true;
                                break;
                            }
                        } else {
                            writeSize = fillSize;
                            break;
                        }
                    }
                    AMediaCodec_queueInputBuffer(codec, bufferIndex, 0, writeSize, time, flags);
                }
            }
            if(not sawOutputEOF) {
                auto bufferInfo = (AMediaCodecBufferInfo){};
                auto bufferIndex = AMediaCodec_dequeueOutputBuffer(codec, &bufferInfo, 0);
                if(bufferIndex >= 0) {
                    auto bufferSize = (size_t)0;
                    auto outputBuffer = AMediaCodec_getOutputBuffer(codec, bufferIndex, &bufferSize);
                    auto readBuffer = reinterpret_cast<int16_t*>(outputBuffer);
                    for(auto i=0; i<bufferInfo.size/sizeof(int16_t); i++) {
                        resultBuffer.push_back(readBuffer[i]);
                    }
                    AMediaCodec_releaseOutputBuffer(codec, bufferIndex, false);
                }
                if(bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                    sawOutputEOF = true;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop));
        AMediaCodec_stop(codec);
        return resultBuffer;
    }
    static inline auto readAudioFile(std::string filePath, int sampleRate) {
        auto fileDesc = (FileDescription){};
        auto extractor = (Extractor){};
        auto readBuffer = std::vector<int16_t>();
        auto resultBuffer = std::vector<float>();
        if(not isValid(filePath)) {
            goto cleanup;
        }
        if(not isValid(fileDesc = getFileDescToRead(filePath))) {
            goto cleanup;
        }
        if(not isValid(extractor = createExtractor(fileDesc))) {
            goto cleanup;
        }
        if(extractor.mime == std::string("audio/raw")) {
            readBuffer = readPCMFile(extractor);
        } else {
            readBuffer = readCodecFile(extractor);
        }
        resultBuffer = convertToFloatAudio(readBuffer);
        if(extractor.channelSize == 1) {
            resultBuffer = convertToStereo(resultBuffer);
        }
        if(sampleRate != extractor.sampleRate) {
            resultBuffer = resampleAudio(resultBuffer,2,extractor.sampleRate,sampleRate);
        }

    cleanup:
        close(extractor);
        close(fileDesc);

        return resultBuffer;
    }
    static inline auto convertTo16BitAudio(std::vector<float> const& input) {
        auto resultBuffer = std::vector<int16_t>(input.size());
        for(auto i=0; i<input.size(); i++) {
            if(input[i] == 0.f or input[i] == -0.f) resultBuffer[i] = 0;
            else if(input[i] >  0.f) resultBuffer[i] = +static_cast<int16_t>(input[i]*std::numeric_limits<int16_t>::max());
            else if(input[i] < -0.f) resultBuffer[i] = -static_cast<int16_t>(input[i]*std::numeric_limits<int16_t>::min());
            else resultBuffer[i] = 0;
        }
        return resultBuffer;
    }
    static inline auto writeWAVFile(std::string filePath, std::vector<int16_t> const& audioBuffer, int sampleRate, int numChannels) {
        struct WavHeader {
            // RIFF 헤더
            char riff[4] = {'R', 'I', 'F', 'F'};
            uint32_t fileSize;
            char wave[4] = {'W', 'A', 'V', 'E'};
            // 포맷 헤더
            char fmt[4] = {'f', 'm', 't', ' '};
            uint32_t fmtSize = 16;
            uint16_t audioFormat = 1;
            uint16_t numChannels;
            uint32_t sampleRate;
            uint32_t byteRate;
            uint16_t blockAlign;
            uint16_t bitsPerSample;
            // 데이터 헤더
            char data[4] = {'d', 'a', 't', 'a'};
            uint32_t dataSize;
        };
        // WAV 파일 헤더 설정
        WavHeader header;
        header.fileSize = sizeof(WavHeader) - 8 + audioBuffer.size() * sizeof(int16_t);
        header.numChannels = numChannels;
        header.sampleRate = sampleRate;
        header.byteRate = sampleRate * numChannels * sizeof(int16_t);
        header.blockAlign = numChannels * sizeof(int16_t);
        header.bitsPerSample = 16;
        header.dataSize = audioBuffer.size() * sizeof(int16_t);
        // WAV 파일 열기
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        file.write(reinterpret_cast<char*>(&header), sizeof(header));
        file.write(reinterpret_cast<char const*>(audioBuffer.data()), audioBuffer.size() * sizeof(int16_t));
        file.close();
        return true;
    }
    struct Muxer {
        AMediaMuxer* ptr = nullptr;
        AMediaFormat* format = nullptr;
        char const* mime = nullptr;
        int index = -1;
        int32_t sampleRate = 0;
        int32_t channelSize = 0;
    };
    static inline auto createMuxer(FileDescription fileDesc, int sampleRate, int numChannels) {
        auto result = (Muxer){};
        auto muxer = AMediaMuxer_new(fileDesc.id, AMEDIAMUXER_OUTPUT_FORMAT_MPEG_4);
        if(not muxer) return result;
        auto format = AMediaFormat_new();
        AMediaFormat_setString(format,AMEDIAFORMAT_KEY_MIME,"mp4a-latm");
        AMediaFormat_setInt32(format,AMEDIAFORMAT_KEY_AAC_PROFILE,2);
        AMediaFormat_setInt32(format,AMEDIAFORMAT_KEY_SAMPLE_RATE,sampleRate);
        AMediaFormat_setInt32(format,AMEDIAFORMAT_KEY_CHANNEL_COUNT,numChannels);
        AMediaFormat_setInt32(format,AMEDIAFORMAT_KEY_BIT_RATE,128000);
        result.ptr = muxer;
        result.format = format;
        result.mime = "audio/mp4a-latm";
        result.sampleRate = sampleRate;
        result.channelSize = numChannels;
        return result;
    }
    static inline auto isValid(Muxer muxer) {
        return bool {
                muxer.ptr
                and muxer.format
                and muxer.sampleRate > 0
                and (muxer.channelSize == 1 or muxer.channelSize == 2)
                and muxer.mime == std::string("audio/mp4a-latm")
        };
    }
    static inline auto close(Muxer& muxer) {
        if(muxer.ptr) AMediaMuxer_delete(muxer.ptr);
        if(muxer.format) AMediaFormat_delete(muxer.format);
        muxer = (Muxer){};
    }
    static inline auto writeAACFile(Muxer muxer, std::vector<int16_t> const& audioBuffer) {
        auto dataBuffer = reinterpret_cast<uint8_t const*>(audioBuffer.data());
        auto dataSize = audioBuffer.size() * sizeof(int16_t);
        auto codec = AMediaCodec_createEncoderByType(muxer.mime);
        AMediaCodec_configure(codec, muxer.format, nullptr, nullptr,AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
        AMediaCodec_start(codec);
        auto offset = 0;
        for(auto sawInputEOF = false, sawOutputEOF = false; not sawOutputEOF; ) {
            if(not sawInputEOF) {
                auto bufferIndex = AMediaCodec_dequeueInputBuffer(codec,waitTimeToDequeueBuffer);
                if(bufferIndex >= 0) {
                    auto bufferSize = (size_t)0;
                    auto inputbuffer = AMediaCodec_getInputBuffer(codec, bufferIndex, &bufferSize);
                    auto remaningSize = dataSize - offset;
                    auto writeSize = bufferSize < remaningSize ? bufferSize : remaningSize;
                    auto time = (long)offset * 1000000 / (2 * muxer.channelSize * muxer.sampleRate);
                    auto flags = (uint32_t)0;
                    memcpy(inputbuffer, dataBuffer+offset, writeSize);
                    if((offset+=writeSize) >= dataSize) {
                        flags = AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
                        sawInputEOF = true;
                    }
                    AMediaCodec_queueInputBuffer(codec, bufferIndex, 0, writeSize, time, flags);
                }
            }
            if(not sawOutputEOF) {
                auto bufferInfo = (AMediaCodecBufferInfo) {};
                auto bufferIndex = AMediaCodec_dequeueOutputBuffer(codec, &bufferInfo, waitTimeToDequeueBuffer);
                if (bufferIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
                } else if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
                    AMediaFormat_delete(muxer.format);
                    muxer.format = AMediaCodec_getOutputFormat(codec);
                } else if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
                } else if (bufferIndex >= 0) {
                    auto bufferSize = (size_t)0;
                    auto outputBuffer = AMediaCodec_getOutputBuffer(codec, bufferIndex, &bufferSize);
                    if (muxer.index < 0) {
                        muxer.index = AMediaMuxer_addTrack(muxer.ptr, muxer.format);
                        AMediaMuxer_start(muxer.ptr);
                    }
                    AMediaMuxer_writeSampleData(muxer.ptr, muxer.index, outputBuffer,&bufferInfo);
                    AMediaCodec_releaseOutputBuffer(codec, bufferIndex, false);
                    if (bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                        sawOutputEOF = true;
                    }
                }
            }
        }
        AMediaMuxer_stop(muxer.ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop));
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
    }
    static inline auto writeAACFile(std::string filePath, std::vector<int16_t> const& audioBuffer, int sampleRate, int numChannels) {
        auto fileDesc = (FileDescription){};
        auto muxer = (Muxer){};
        if(not isValid(fileDesc = getFileDescToWrite(filePath))) {
            goto cleanup;
        }
        if(not isValid(muxer = createMuxer(fileDesc, sampleRate, numChannels))) {
            goto cleanup;
        }
        writeAACFile(muxer, audioBuffer);

    cleanup:
        close(muxer);
        close(fileDesc);

        return true;
    }
    enum AudioFormat { PCM, AAC };
    static inline auto writeAudioFile(std::string filePath, std::vector<float> const& buffer, int format, int sampleRate, int numChannels) {
        auto int16Audio = convertTo16BitAudio(buffer);
        if(format == PCM) writeWAVFile(filePath, int16Audio, sampleRate, numChannels);
        else if(format == AAC) writeAACFile(filePath, int16Audio, sampleRate, numChannels);
    }
    static inline auto getSampleRate(std::string filePath) {
        auto result = (int32_t)0;
        auto fileDesc = (FileDescription){};
        auto extractor = (Extractor){};
        if(not isValid(fileDesc = getFileDescToRead(filePath))) goto cleanup;
        if(not isValid(extractor = createExtractor(fileDesc))) goto cleanup;
        result = extractor.sampleRate;

    cleanup:
        close(extractor);
        close(fileDesc);
        return result;
    }
    static inline auto getChannelSize(std::string filePath) {
        auto result = (int32_t)0;
        auto fileDesc = (FileDescription){};
        auto extractor = (Extractor){};
        if(not isValid(fileDesc = getFileDescToRead(filePath))) goto cleanup;
        if(not isValid(extractor = createExtractor(fileDesc))) goto cleanup;
        result = extractor.channelSize;

    cleanup:
        close(extractor);
        close(fileDesc);
        return result;
    }
    static inline auto getFrameSize(std::string filePath) {
        auto result = (int32_t)0;
        auto fileDesc = (FileDescription){};
        auto extractor = (Extractor){};
        if(not isValid(fileDesc = getFileDescToRead(filePath))) goto cleanup;
        if(not isValid(extractor = createExtractor(fileDesc))) goto cleanup;
        result = extractor.frameSize;

    cleanup:
        close(extractor);
        close(fileDesc);
        return result;
    }

    /*struct RTRecorder {
        FileDescription      fileDesc;
        Muxer                muxer;
        AMediaCodec*         codec = nullptr;
        //std::vector<int16_t> buffer;
        std::vector<uint8_t> buffer;
        int                  writeSize  = 0;
        int                  writeCount = 0;
        std::mutex           mutex;
        bool                 isStart = false;
        auto operator=(RTRecorder const& in) -> RTRecorder& {
            this->fileDesc   = in.fileDesc;
            this->muxer      = in.muxer;
            this->codec      = in.codec;
            this->buffer     = in.buffer;
            this->writeSize  = in.writeSize;
            this->writeCount = in.writeCount;
            this->isStart    = in.isStart;
            return *this;
        }
    };*/
    /*static inline auto close(RTRecorder& recorder) {
        auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (recorder.codec) {
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop));
            AMediaCodec_stop  (recorder.codec);
            AMediaCodec_flush (recorder.codec);
            AMediaCodec_delete(recorder.codec);
        }
        close(recorder.muxer);
        close(recorder.fileDesc);
        recorder = RTRecorder();
    }*/
    /*static inline auto ready(RTRecorder& recorder, std::string const& filePath, int sampleRate, int numChannels) -> bool {
        auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (not isValid(recorder.fileDesc = getFileDescToWrite(filePath))) goto error;
        if (not isValid(recorder.muxer = createMuxer(recorder.fileDesc,sampleRate,numChannels))) goto error;
        if (not recorder.codec)recorder.codec = AMediaCodec_createEncoderByType(recorder.muxer.mime);
        AMediaFormat_setInt32(recorder.muxer.format,AMEDIAFORMAT_KEY_MAX_INPUT_SIZE,4096);
        AMediaCodec_configure(recorder.codec,recorder.muxer.format,nullptr,nullptr,AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
        AMediaCodec_start(recorder.codec);
        for (;;) {
            auto bufferIndex = AMediaCodec_dequeueInputBuffer(recorder.codec,waitTimeToDequeueBuffer);
            if (bufferIndex >= 0) {
                auto iBufferSize = (size_t)0;
                AMediaCodec_getInputBuffer(recorder.codec, bufferIndex, &iBufferSize);
                recorder.buffer.resize(iBufferSize/sizeof(int16_t));
                AMediaCodec_queueInputBuffer(recorder.codec,bufferIndex,0,0,0,0);
                break;
            }
        }
        AMediaCodec_stop(recorder.codec);
        AMediaCodec_flush(recorder.codec);
        AMediaCodec_configure(recorder.codec,recorder.muxer.format,nullptr,nullptr,AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    success:
        return true;
    error:
        close(recorder);
        return false;
    }*/
    /*static inline auto start(RTRecorder& recorder) -> bool {
        auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (AMediaCodec_start(recorder.codec) != AMEDIA_OK) return false;
        else {
            recorder.isStart = true;
            return true;
        }
    }*/
    /*static inline auto writeAACFile(RTRecorder& recorder, std::vector<int16_t> const& audioBuffer, bool isEOF = false) {
        auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (not recorder.isStart) return;
        std::copy(audioBuffer.begin(), audioBuffer.end(),recorder.buffer.begin() + recorder.writeSize);
        if (not isEOF) {
            if ((recorder.writeSize += audioBuffer.size()) < recorder.buffer.size()) return;
            else recorder.writeSize = 0;
        }
        auto dataBuffer = reinterpret_cast<uint8_t const *>(recorder.buffer.data());
        auto dataSize = recorder.buffer.size() * sizeof(int16_t);
        auto time = (recorder.writeCount++*dataSize*1000000ull)/(2.0*recorder.muxer.channelSize*recorder.muxer.sampleRate);
        for (;;) {
            auto bufferIndex = AMediaCodec_dequeueInputBuffer(recorder.codec, waitTimeToDequeueBuffer);
            if (bufferIndex >= 0) {
                if (isEOF) {
                    AMediaCodec_queueInputBuffer(recorder.codec, bufferIndex, 0, dataSize, time, AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
                    break;
                }
                if (dataSize > 0) {
                    auto iBufferSize = (size_t)0;
                    auto iBuffer = AMediaCodec_getInputBuffer(recorder.codec, bufferIndex, &iBufferSize);
                    std::copy(dataBuffer, dataBuffer + dataSize, iBuffer);
                    std::fill(recorder.buffer.begin(),recorder.buffer.end(),0.0);
                    recorder.writeSize = 0;
                    AMediaCodec_queueInputBuffer(recorder.codec, bufferIndex, 0, dataSize, time, 0);
                    break;
                } else {
                    AMediaCodec_queueInputBuffer(recorder.codec, bufferIndex, 0, 0, 0, 0);
                    break;
                }
            }
        }
        for (;;) {
            auto bufferInfo  = AMediaCodecBufferInfo();
            auto bufferIndex = AMediaCodec_dequeueOutputBuffer(recorder.codec,&bufferInfo,waitTimeToDequeueBuffer);
            if (bufferIndex >= 0) {
                if (recorder.muxer.index < 0) {
                    recorder.muxer.index = AMediaMuxer_addTrack(recorder.muxer.ptr, recorder.muxer.format);
                    AMediaMuxer_start(recorder.muxer.ptr);
                }
                bufferInfo.presentationTimeUs = time;
                auto oBufferSize = (size_t)0;
                auto oBuffer = AMediaCodec_getOutputBuffer(recorder.codec, bufferIndex, &oBufferSize);
                AMediaMuxer_writeSampleData(recorder.muxer.ptr, recorder.muxer.index, oBuffer,&bufferInfo);
                AMediaCodec_releaseOutputBuffer(recorder.codec, bufferIndex, false);
                std::thread sleep([&] { std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToMuxer)); });
                sleep.join();
                break;
            } else if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
                AMediaFormat_delete(recorder.muxer.format);
                recorder.muxer.format = AMediaCodec_getOutputFormat(recorder.codec);
            } else if (bufferIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
            } else if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
            }
        }
    }*/
    /*static inline auto stop(RTRecorder& recorder) -> bool {
        writeAACFile(recorder,{},true);
        auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        //std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop));
        std::thread sleep([&] {std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop)); });
        sleep.join();
        //AMediaMuxer_stop(recorder.muxer.ptr);
        if (AMediaCodec_stop(recorder.codec) != AMEDIA_OK) {
            AMediaCodec_flush(recorder.codec);
            //AMediaMuxer_stop(recorder.muxer.ptr);
            std::thread sleep([&] {std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop)); });
            sleep.join();
            return false;
        }
        else {
            AMediaCodec_flush(recorder.codec);
            //AMediaMuxer_stop(recorder.muxer.ptr);
            std::thread sleep([&] {std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeToCodecStop)); });
            sleep.join();
            recorder.isStart = false;
            return true;
        }
    }*/
    struct RTRecorder {
        FileDescription      fileDesc;
        Muxer                muxer;
        AMediaCodec*         codec = nullptr;
        std::vector<uint8_t> buffer;
        std::mutex           mutex;
        std::size_t          writeCount = 0;
        int                  isStart = 0;
        auto operator=(RTRecorder const& in) -> RTRecorder& {
            this->fileDesc   = in.fileDesc;
            this->muxer      = in.muxer;
            this->codec      = in.codec;
            this->buffer     = in.buffer;
            this->writeCount = in.writeCount;
            this->isStart    = in.isStart;
            return *this;
        }
    };
    static inline auto close(RTRecorder& recorder) {
        //auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (recorder.codec) {
            AMediaCodec_stop  (recorder.codec);
            AMediaCodec_flush (recorder.codec);
            AMediaCodec_delete(recorder.codec);
        }
        close(recorder.muxer);
        close(recorder.fileDesc);
        recorder = RTRecorder();
    }
    static inline auto ready(RTRecorder& recorder, std::string const& filePath, int sampleRate, int numChannels) -> bool {
        //auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (not isValid(recorder.fileDesc = getFileDescToWrite(filePath))) goto error;
        if (not isValid(recorder.muxer = createMuxer(recorder.fileDesc,sampleRate,numChannels))) goto error;
        if (not recorder.codec)recorder.codec = AMediaCodec_createEncoderByType(recorder.muxer.mime);
        AMediaFormat_setInt32(recorder.muxer.format,AMEDIAFORMAT_KEY_MAX_INPUT_SIZE,4096);
        AMediaCodec_configure(recorder.codec,recorder.muxer.format,nullptr,nullptr,AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    success:
        return true;
    error:
        return false;
    }
    static inline auto start(RTRecorder& recorder) -> bool {
        //auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        if (AMediaCodec_start(recorder.codec) != AMEDIA_OK) return false;
        else return true;
    }
    static inline auto writeAACFile(RTRecorder& recorder, std::vector<int16_t> const& audioBuffer, bool isEOF = false) {
        //auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        auto time = (std::size_t)0;
        for (;;) {
            auto bufferIndex = AMediaCodec_dequeueInputBuffer(recorder.codec, waitTimeToDequeueBuffer);
            if (bufferIndex >= 0) {
                auto dataBuffer = reinterpret_cast<uint8_t const*>(audioBuffer.data());
                auto dataSize   = audioBuffer.size() * sizeof(int16_t);
                auto iBufferSize = (size_t)0;
                auto iBuffer     = AMediaCodec_getInputBuffer(recorder.codec,bufferIndex,&iBufferSize);
                auto fullBuffer = std::vector<uint8_t>();
                for (auto i=0; i<dataSize; i++) {
                    recorder.buffer.push_back(dataBuffer[i]);
                }
                if (recorder.buffer.size() < iBufferSize and not isEOF) {
                    AMediaCodec_queueInputBuffer(recorder.codec,bufferIndex,0,0,0,0);
                    return;
                } else if (recorder.buffer.size() == iBufferSize or isEOF) {
                    fullBuffer = std::move(recorder.buffer);
                } else {
                    for (auto i=0; i<iBufferSize; i++) {
                        fullBuffer.push_back(recorder.buffer[i]);
                    }
                    recorder.buffer.erase(recorder.buffer.begin(),recorder.buffer.begin()+iBufferSize);
                }
                std::memcpy(iBuffer,fullBuffer.data(),fullBuffer.size());
                auto flags = isEOF ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM : 0;
                time = (recorder.writeCount++*fullBuffer.size()*1000000ull)/(2.0*recorder.muxer.channelSize*recorder.muxer.sampleRate);
                AMediaCodec_queueInputBuffer(recorder.codec,bufferIndex,0,fullBuffer.size(),time,flags);
                break;
            }
        }
        for (;;) {
            auto bufferInfo = AMediaCodecBufferInfo();
            auto bufferIndex = AMediaCodec_dequeueOutputBuffer(recorder.codec,&bufferInfo,waitTimeToDequeueBuffer);
            if (bufferIndex >= 0) {
                auto bufferSize = (size_t)0;
                auto outputBuffer = AMediaCodec_getOutputBuffer(recorder.codec,bufferIndex,&bufferSize);
                if (recorder.muxer.index < 0) {
                    recorder.muxer.index = AMediaMuxer_addTrack(recorder.muxer.ptr,recorder.muxer.format);
                    AMediaMuxer_start(recorder.muxer.ptr);
                }
                //bufferInfo.presentationTimeUs = time;
                //if (recorder.isStart < 3 and bufferInfo.size > 0) {
                //    recorder.isStart += 1;
                //    recorder.writeCount = 0;
                //} else {
                    if (bufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM and recorder.buffer.size() > 0) {
                        bufferInfo.flags = 0; 
                    }
                    AMediaMuxer_writeSampleData(recorder.muxer.ptr, recorder.muxer.index,
                                                outputBuffer, &bufferInfo);
                //}
                AMediaCodec_releaseOutputBuffer(recorder.codec,bufferIndex,false);
                break;
            } else if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
                AMediaFormat_delete(recorder.muxer.format);
                recorder.muxer.format = AMediaCodec_getOutputFormat(recorder.codec);
            } else if (bufferIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
            } else if (bufferIndex == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
            }
        }
    }
    static inline auto stop(RTRecorder& recorder) -> bool {
        for (;;) {
            writeAACFile(recorder, {}, true);
            if (recorder.buffer.size() == 0) break;
        }
        //auto lock = std::unique_lock<std::mutex>(recorder.mutex);
        AMediaCodec_stop (recorder.codec);
        AMediaMuxer_stop (recorder.muxer.ptr);
        return true;
    }
}

#endif//__ANDROID__
#endif//BaundStudioEngine_Sources_Android_AndroidAudioFileSupport_hpp
