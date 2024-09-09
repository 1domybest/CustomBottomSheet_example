/*
 * Baund Studio Engine
 *
 * Unit-CombFilter.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Unit_CombFilter_hpp
#define BaundStudioEngine_Sources_Effect_Unit_CombFilter_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"

namespace Baund::StudioEngine::Unit {
    class CombFilter final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Frames      = AudioBuffer::Frames;
        using Feedback    = double;
        using Buffer      = std::vector<SampleValue>;
        
        ~CombFilter() override        = default;
        CombFilter()                  = default;
        CombFilter(CombFilter const&);
        auto operator=(CombFilter const&) -> CombFilter&;
        
        void reset  ()       override;
        bool isValid() const override;

        auto processing(SampleValue const&) -> SampleValue;
        
        auto setDelay   (Frames   const&) -> void;
        auto setFeedback(Feedback const&) -> void;
        auto getDelay   () const -> Frames;
        auto getFeedback() const -> Feedback;
        
    private:
        struct NonCopyables {
            std::mutex mutex;
        } n;
        struct MemberVariables {
            Buffer   buffer;
            Frames   position = 0;
            Frames   delay    = 0;
            Feedback feedback = 0.0;
            bool     active   = false;
        } m;
    };
    
    inline CombFilter::CombFilter(CombFilter const& in)
    : m(in.m) {
    }
    inline auto CombFilter::operator=(CombFilter const& in) -> CombFilter& {
        this->m = in.m;
        return *this;
    }
    inline void CombFilter::reset() {
        m = MemberVariables();
    }
    inline bool CombFilter::isValid() const {
        return true;
    }
    inline auto CombFilter::setDelay(Frames const& in) -> void {
        std::unique_lock<std::mutex> lock(n.mutex);
        m.active = false;
        m.position = 0;
        m.delay = in;
        m.buffer.resize(in);
        std::fill(m.buffer.begin(),m.buffer.end(),0.0);
        m.active = true;
    }
    inline auto CombFilter::setFeedback(Feedback const& in) -> void {
        m.feedback = in;
    }
    inline auto CombFilter::getDelay() const -> Frames {
        return m.delay;
    }
    inline auto CombFilter::getFeedback() const -> Feedback {
        return m.feedback;
    }
    inline auto CombFilter::processing(SampleValue const& in) -> SampleValue {
        std::unique_lock<std::mutex> lock(n.mutex);
        if (m.active) {
            if (std::isnan(m.buffer[m.position])) m.buffer[m.position] = 0.0;
            auto out = m.buffer[m.position];
            m.buffer[m.position] = ((in + m.buffer[m.position]) * m.feedback);
            m.position += 1;
            (m.position >= m.delay) ? m.position = 0 : NULL;
            //auto inPos  = m.position;
            //auto outPos = (m.position >= m.delay) ? 0 : m.position + 1;
            //m.position  = outPos;
            //m.buffer[inPos] = m.buffer[inPos]*m.feedback;//in + m.buffer[inPos]*m.feedback;
            //auto out = m.buffer[outPos];
            return out;
        } else return in;
    }
}

#endif//BaundStudioEngine_Sources_Effect_Unit_CombFilter_hpp
