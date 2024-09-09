/*
 * Baund Studio Engine
 *
 * ItemManager.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Project_ItemManager_hpp
#define BaundStudioEngine_Sources_Project_ItemManager_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "MediaFileManager.hpp"

namespace Baund::StudioEngine {
    class Item: public PublicObject {
    public:
        using Frames = AudioBuffer::Frames;

        virtual ~Item() override = default;
        Item()                   = default;
        Item(Item const&)        = default;
        Item(Item     &&);

        auto operator=(Item const&) -> Item& = default;
        auto operator=(Item     &&) -> Item&;

        virtual void reset  ()       override = 0;
        virtual bool isValid() const override = 0;

        auto setPosition(Frames const&) -> void;
        auto setLength  (Frames const&) -> void;
        auto setOffset  (Frames const&) -> void;
        auto getPosition() const -> Frames;
        auto getLength  () const -> Frames;
        auto getOffset  () const -> Frames;

        auto operator==(Item const&) const -> bool;
        auto operator!=(Item const&) const -> bool;
        auto operator>=(Item const&) const -> bool;
        auto operator<=(Item const&) const -> bool;
        auto operator> (Item const&) const -> bool;
        auto operator< (Item const&) const -> bool;

    private:
        struct MemberVariables {
            Frames position = 0;
            Frames length   = 0;
            Frames offset   = 0;
        } m;
    };
    class AudioItem final: public Item {
    public:
        using FileAccessor = MediaFileManager::AudioFileAccessor;
        using GainValue    = double;
        using PCMSamples   = std::vector<AudioBuffer::SampleValue>;

        ~AudioItem() override       = default;
        AudioItem()                 = default;
        AudioItem(AudioItem const&) = default;
        AudioItem(AudioItem     &&);

        auto operator=(AudioItem const&) -> AudioItem& = default;
        auto operator=(AudioItem     &&) -> AudioItem&;
        
        void reset  ()       override;
        bool isValid() const override;
        
        auto setAudioFile (FileAccessor const&) -> void;
        auto setGainValue (GainValue    const&) -> void;
        auto setFadeIn    (Frames       const&) -> void;
        auto setFadeOut   (Frames       const&) -> void;
        auto setAudioTrack(PublicObject::ID const&) -> void;
        auto getAudioFile () const -> FileAccessor;
        auto getGainValue () const -> GainValue;
        auto getFadeIn    () const -> Frames;
        auto getFadeOut   () const -> Frames;
        auto getAudioTrack() const -> PublicObject::ID;

        auto readSampleFrame  (Frames const&) -> AudioBuffer const&;
        auto extractPCMSamples(Frames const&) -> PCMSamples;

    private:
        struct MemberVariables {
            FileAccessor     audioFile;
            GainValue        gainValue   = 1.0;
            Frames           fadeIn      = 64;
            Frames           fadeOut     = 64;
            AudioBuffer      audioBuffer;
            PublicObject::ID trackID = PublicObject::none;
        } m;
    };
    class ItemManager final: public virtual BaseObject {
    public:
        using AudioItemList     = List<AudioItem>;
        using AudioItemAccessor = AudioItemList::Accessor;
    
        ~ItemManager() override         = default;
        ItemManager()                   = default;
        ItemManager(ItemManager const&) = default;
        ItemManager(ItemManager     &&);
        
        auto operator=(ItemManager const&) -> ItemManager& = default;
        auto operator=(ItemManager     &&) -> ItemManager&;
        
        void reset  ()       override;
        bool isValid() const override;

        auto getAudioItemList()       -> AudioItemList&;
        auto getAudioItemList() const -> AudioItemList const&;
        auto getAudioItem(PublicObject::ID const&) const -> AudioItemAccessor;

        auto add(AudioItem         const&) -> AudioItemAccessor;
        auto add(AudioItem             &&) -> AudioItemAccessor;
        auto sub(AudioItemAccessor const&) -> void;
        
    private:
        struct MemberVariables {
            AudioItemList audioItemList;
        } m;
    };
    
    inline Item::Item(Item&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto Item::operator=(Item&& in) -> Item& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void Item::reset() {
        m = MemberVariables();
    }
    inline bool Item::isValid() const {
        return m.length > 0;
    }
    inline auto Item::setPosition(Frames const& in) -> void { m.position = in; }
    inline auto Item::setLength  (Frames const& in) -> void { m.length   = (in > 0) ? in : 0; }
    inline auto Item::setOffset  (Frames const& in) -> void { m.offset   = in; }
    inline auto Item::getPosition() const -> Frames         { return m.position; }
    inline auto Item::getLength  () const -> Frames         { return m.length; }
    inline auto Item::getOffset  () const -> Frames         { return m.offset; }
    inline auto Item::operator==(Item const& in) const -> bool {
        return this->m.position == in.m.position;
    }
    inline auto Item::operator!=(Item const& in) const -> bool {
        return this->m.position != in.m.position;
    }
    inline auto Item::operator>=(Item const& in) const -> bool {
        return this->m.position >= in.m.position;
    }
    inline auto Item::operator<=(Item const& in) const -> bool {
        return this->m.position <= in.m.position;
    }
    inline auto Item::operator> (Item const& in) const -> bool {
        return this->m.position >  in.m.position;
    }
    inline auto Item::operator< (Item const& in) const -> bool {
        return this->m.position <  in.m.position;
    }
    
    inline AudioItem::AudioItem(AudioItem&& in)
    : Item(std::move(in))
    , m   (std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto AudioItem::operator=(AudioItem&& in) -> AudioItem& {
        if (this != &in) {
            this->Item::operator=(std::move(in));
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void AudioItem::reset() {
        m = MemberVariables();
        Item::reset();
    }
    inline bool AudioItem::isValid() const {
        return Item::isValid() and m.audioFile.isValid();
    }
    inline auto AudioItem::setAudioFile(FileAccessor const& in) -> void {
        m.audioFile   = in;
        m.audioBuffer = AudioBuffer({
            .sampleRate  = in->audioFile.getSampleRate(),
            .frameSize   = 1,
            .channelSize = in->audioFile.getChannelSize()
        });
    }
    inline auto AudioItem::setGainValue(GainValue const& in) -> void {
        auto value = in;
        if (value < 0.0) value = 0.0;
        m.gainValue = in;
    }
    inline auto AudioItem::setFadeIn(Frames const& in) -> void {
        m.fadeIn = in > 0 ? in : 0;
        if (m.fadeIn >= getLength()) {
            m.fadeIn  = getLength() - 1;
        }
        if (m.fadeIn + m.fadeOut >= getLength()) {
            m.fadeOut = getLength() - 1 - m.fadeIn;
        }
    }
    inline auto AudioItem::setFadeOut(Frames const& in) -> void {
        m.fadeOut = in > 0 ? in : 0;
        if (m.fadeOut >= getLength()) {
            m.fadeOut = getLength() - 1;
        }
        if (m.fadeIn + m.fadeOut >= getLength()) {
            m.fadeIn = getLength() - 1 - m.fadeOut;
        }
    }
    inline auto AudioItem::setAudioTrack(PublicObject::ID const& in) -> void {
        m.trackID = in;
    }
    inline auto AudioItem::getAudioFile () const -> FileAccessor     { return m.audioFile; }
    inline auto AudioItem::getGainValue () const -> GainValue        { return m.gainValue; }
    inline auto AudioItem::getFadeIn    () const -> Frames           { return m.fadeIn; }
    inline auto AudioItem::getFadeOut   () const -> Frames           { return m.fadeOut; }
    inline auto AudioItem::getAudioTrack() const -> PublicObject::ID { return m.trackID; }
    inline auto AudioItem::readSampleFrame(Frames const& frame) -> AudioBuffer const& {
        // 아이템버퍼 초기화.
        m.audioBuffer.silence();
        if (not m.audioFile.isValid()) return m.audioBuffer;
        if (m.audioFile->buffer.getSampleRate()  != m.audioBuffer.getSampleRate() or
            m.audioFile->buffer.getChannelSize() != m.audioBuffer.getChannelSize()) {
            m.audioBuffer = AudioBuffer({
                .sampleRate  = m.audioFile->buffer.getSampleRate(),
                .frameSize   = 1,
                .channelSize = m.audioFile->buffer.getChannelSize()
            });
        }
        // 읽기 위치 확인.
        if (frame < 0 or frame >= this->getLength()) return m.audioBuffer;
        auto readFrame = frame - this->getOffset();
        if (readFrame < 0 or readFrame >= m.audioFile->buffer.getFrameSize()) {
            return m.audioBuffer;
        }
        // 페이드인 적용.
        auto fade = 1.0;
        if (frame <= this->getFadeIn()) {
            if (this->getFadeIn() > 0) {
                fade = 1.0 / this->getFadeIn() * frame;
            }
        }
        // 페이드아웃 적용.
        else if (frame >= this->getLength() - this->getFadeOut()) {
            if (this->getFadeOut() > 0) {
                fade = 1.0 / this->getFadeOut() * (this->getLength() - frame - 1);
            }
        }
        // 게인적용 및 버퍼에 쓰기.
        for (auto c=0; c<m.audioBuffer.getChannelSize(); c++) {
            m.audioBuffer[0][c] = m.audioFile->buffer[readFrame][c] * fade * this->getGainValue();
        }
        // 아이템버퍼 반환.
        return m.audioBuffer;
    }
    inline auto AudioItem::extractPCMSamples(Frames const& frameUnit) -> PCMSamples {
        auto buffer = std::vector<AudioBuffer::SampleValue>(this->getLength());
        for (auto i=0; i<this->getLength(); i++) {
            auto read = readSampleFrame(i);
            auto mono = read.toMono<AudioBuffer::Select::max>();
            buffer[i] = mono[0][0];
        }
        return Algorithm::extractMaxAbsValues(buffer, (int)frameUnit);
    }
    
    inline ItemManager::ItemManager(ItemManager&& in)
    : m(std::move(in.m)) {
        in.m = MemberVariables();
    }
    inline auto ItemManager::operator=(ItemManager&& in) -> ItemManager& {
        if (this != &in) {
            this->m = std::move(in.m);
            in   .m = MemberVariables();
        }
        return *this;
    }
    inline void ItemManager::reset() {
        m = MemberVariables();
    }
    inline bool ItemManager::isValid() const {
        return m.audioItemList.count() > 0;
    }
    inline auto ItemManager::getAudioItemList() -> AudioItemList& {
        return m.audioItemList;
    }
    inline auto ItemManager::getAudioItemList() const -> AudioItemList const& {
        return m.audioItemList;
    }
    inline auto ItemManager::getAudioItem(PublicObject::ID const& in) const -> AudioItemAccessor {
        auto lock = this->getLock();
        auto const errorResult = (AudioItemAccessor) {};
        if (in.value == PublicObject::none.value) {
            goto error;
        }
        for (auto accessor : m.audioItemList) {
            if (accessor->getObjectID().value == in.value) return accessor;
        }
    error:
        return errorResult;
    }
    inline auto ItemManager::add(AudioItem const& in) -> AudioItemAccessor {
        auto item = in;
        return add(std::move(item));
    }
    inline auto ItemManager::add(AudioItem&& in) -> AudioItemAccessor {
        auto lock = this->getLock();
        return m.audioItemList.add(std::move(in));
    }
    inline auto ItemManager::sub(AudioItemAccessor const& in) -> void {
        auto lock = this->getLock();
        if (in.isValid()) {
            in->reset();
            m.audioItemList.sub(in);
        }
    }
}

#endif//BaundStudioEngine_Sources_Project_ItemManager_hpp
