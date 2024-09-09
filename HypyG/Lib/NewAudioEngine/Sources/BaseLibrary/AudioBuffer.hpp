/*
 * Baund Studio Engine
 *
 * AudioBuffer.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_AudioBuffer_hpp
#define BaundStudioEngine_Sources_BaseLibrary_AudioBuffer_hpp

#include "Definitions.hpp"
#include "Algorithm.hpp"
#include "LogManager.hpp"

namespace Baund::StudioEngine {
    class AudioBuffer final: public virtual BaseObject {
    public:
        using SampleRate   = double;
        using Frames       = long;
        using Channels     = long;
        using SampleValue  = float;
        using Data         = SampleValue*;
        using FrameSamples = SampleValue[];
        struct Args {
            SampleRate sampleRate;
            Frames     frameSize;
            Channels   channelSize;
        };
        enum Channel { L = 0, R = 1 };
        enum Format  { other = 0, mono = 1, stereo = 2 };
        
        ~AudioBuffer() override;
        AudioBuffer() = default;
        AudioBuffer(AudioBuffer const&);
        AudioBuffer(AudioBuffer     &&);
        AudioBuffer(Args        const&);

        auto operator=(AudioBuffer const&) -> AudioBuffer&;
        auto operator=(AudioBuffer     &&) -> AudioBuffer&;

        void reset  ()       override;
        bool isValid() const override;

        auto operator[](Index const&) const -> FrameSamples&;

        auto getSampleRate () const -> SampleRate const&;
        auto getFrameSize  () const -> Frames     const&;
        auto getChannelSize() const -> Channels   const&;
        auto getDataSize   () const -> Size;
        auto getData       () const -> Data       const&;

        enum class Select { l, r, mix, max };
        template< Select select >
        auto toMono  () const -> AudioBuffer;
        auto toStereo() const -> AudioBuffer;
        auto silence () const -> void;

        auto operator==(AudioBuffer const&) const -> bool;
        auto operator!=(AudioBuffer const&) const -> bool;
        auto operator>=(AudioBuffer const&) const -> bool;
        auto operator<=(AudioBuffer const&) const -> bool;
        auto operator> (AudioBuffer const&) const -> bool;
        auto operator< (AudioBuffer const&) const -> bool;

    private:
        struct MemberVariables {
            SampleRate sampleRate  = 0.0;
            Frames     frameSize   = 0;
            Channels   channelSize = 0;
            Data       data        = nullptr;
        } m;
    };
    
    inline AudioBuffer::~AudioBuffer() {
        if (m.data) delete [] m.data;
    }
    inline AudioBuffer::AudioBuffer(AudioBuffer const& in)
    : m({
        .sampleRate  = in.m.sampleRate,
        .frameSize   = in.m.frameSize,
        .channelSize = in.m.channelSize,
        .data        = [&in] {
            auto dataSize = in.m.frameSize * in.m.channelSize;
            auto buffer   = (dataSize > 0) ? new SampleValue[dataSize] : nullptr;
            if (buffer != nullptr and in.m.data != nullptr) {
                auto begin = in.m.data;
                auto end   = begin + dataSize;
                std::copy(begin, end, buffer);
            }
            return buffer;
        }()
    }) {
    }
    inline AudioBuffer::AudioBuffer(AudioBuffer&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline AudioBuffer::AudioBuffer(Args const& in)
    : m({
        .sampleRate  = in.sampleRate,
        .frameSize   = in.frameSize,
        .channelSize = in.channelSize,
        .data        = [&in] {
            auto dataSize = in.frameSize * in.channelSize;
            return dataSize > 0 ? new SampleValue[dataSize] : nullptr;
        }()
    }) {
    }
    inline auto AudioBuffer::operator=(AudioBuffer const& in) -> AudioBuffer& {
        if (this->m.data != in.m.data) {
            if (in.m.data != nullptr) {
                // 데이터 크기 확인 및 재할당.
                auto dataSize = [this,&in] {
                    auto tDataSize = this->m.frameSize * this->m.channelSize;
                    auto iDataSize = in   .m.frameSize * in   .m.channelSize;
                    if (tDataSize != iDataSize) {
                        if (this->m.data != nullptr) delete [] this->m.data;
                        this->m.data = new SampleValue[iDataSize];
                    }
                    return iDataSize;
                }();
                // 데이터 복사.
                [this,&in,dataSize] {
                    auto begin = in.m.data;
                    auto end   = begin + dataSize;
                    std::copy(begin, end, this->m.data);
                }();
            } else if (this->m.data != nullptr) {
                delete [] this->m.data;
                this->m.data = nullptr;
            }
            this->m.sampleRate  = in.m.sampleRate;
            this->m.frameSize   = in.m.frameSize;
            this->m.channelSize = in.m.channelSize;
        }
        return *this;
    }
    inline auto AudioBuffer::operator=(AudioBuffer&& in) -> AudioBuffer& {
        if (this->m.data != in.m.data) {
            if (this->m.data != nullptr) delete [] this->m.data;
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void AudioBuffer::reset() {
        if (m.data != nullptr) delete [] m.data;
        m = MemberVariables();
    }
    inline bool AudioBuffer::isValid() const {
        return m.data != nullptr;
    }
    inline auto AudioBuffer::operator[](Index const& in) const -> FrameSamples& {
        void* indexPtr = m.data + (in * m.channelSize);
        return *static_cast<FrameSamples*>(indexPtr);
    }
    inline auto AudioBuffer::getSampleRate () const -> SampleRate const& { return m.sampleRate; }
    inline auto AudioBuffer::getFrameSize  () const -> Frames     const& { return m.frameSize; }
    inline auto AudioBuffer::getChannelSize() const -> Channels   const& { return m.channelSize; }
    inline auto AudioBuffer::getDataSize   () const -> Size              { return m.frameSize * m.channelSize; }
    inline auto AudioBuffer::getData       () const -> Data       const& { return m.data; }
    template< AudioBuffer::Select select >
    inline auto AudioBuffer::toMono() const -> AudioBuffer {
        if (m.channelSize == stereo) {
            auto resultBuffer = AudioBuffer({
                .sampleRate  = m.sampleRate,
                .frameSize   = m.frameSize,
                .channelSize = mono
            });
            if constexpr (select == Select::l) {
                for(auto i=0; i<m.frameSize; i++) {
                    resultBuffer[i][0] = (*this)[i][L];
                }
            } else if constexpr (select == Select::r) {
                for(auto i=0; i<m.frameSize; i++) {
                    resultBuffer[i][0] = (*this)[i][R];
                }
            } else if constexpr (select == Select::mix) {
                for(auto i=0; i<m.frameSize; i++) {
                    resultBuffer[i][0] = ((*this)[i][L] + (*this)[i][R]) / 2;
                }
            } else if constexpr (select == Select::max) {
                for(auto i=0; i<m.frameSize; i++) {
                    auto lValue = (*this)[i][L];
                    auto rValue = (*this)[i][R];
                    resultBuffer[i][0] = std::abs(lValue) > std::abs(rValue) ? lValue : rValue;
                }
            }
            return resultBuffer;
        } else if (m.channelSize == mono) {
            return *this;
        } else {
            logManager.add({Log::error,"AudioBuffer","failed to convert mono, channel is not mono or stereo",m.channelSize});
            return { };
        }
    }
    inline auto AudioBuffer::toStereo() const -> AudioBuffer {
        if (m.channelSize == mono) {
            auto resultBuffer = AudioBuffer({
                .sampleRate  = m.sampleRate,
                .frameSize   = m.frameSize,
                .channelSize = stereo
            });
            for (auto i=0; i<m.frameSize; i++) {
                resultBuffer[i][L] = (*this)[i][0];
                resultBuffer[i][R] = (*this)[i][0];
            }
            return resultBuffer;
        } else if (m.channelSize == stereo) {
            return *this;
        } else {
            logManager.add({Log::error,"AudioBuffer","failed to convert stereo, channel is not mono or stereo",m.channelSize});
            return { };
        }
    }
    inline auto AudioBuffer::silence() const -> void {
        std::fill(m.data, m.data+getDataSize(), 0.0f);
    }
    inline auto AudioBuffer::operator==(AudioBuffer const& in) const -> bool {
        return this->m.frameSize == in.m.frameSize;
    }
    inline auto AudioBuffer::operator!=(AudioBuffer const& in) const -> bool {
        return this->m.frameSize != in.m.frameSize;
    }
    inline auto AudioBuffer::operator>=(AudioBuffer const& in) const -> bool {
        return this->m.frameSize >= in.m.frameSize;
    }
    inline auto AudioBuffer::operator<=(AudioBuffer const& in) const -> bool {
        return this->m.frameSize <= in.m.frameSize;
    }
    inline auto AudioBuffer::operator> (AudioBuffer const& in) const -> bool {
        return this->m.frameSize >  in.m.frameSize;
    }
    inline auto AudioBuffer::operator< (AudioBuffer const& in) const -> bool {
        return this->m.frameSize <  in.m.frameSize;
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_AudioBuffer_hpp
