/*
 * Baund Studio Engine
 *
 * Module-RBJFilter.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Effect_Module_RBJFilter_hpp
#define BaundStudioEngine_Sources_Effect_Module_RBJFilter_hpp

#include "Sources/BaseLibrary/BaseLibrary.hpp"
#include "Unit-Biquad1.hpp"

namespace Baund::StudioEngine::Module {
    class RBJFilter final: public BaseObject {
    public:
        using SampleValue = AudioBuffer::SampleValue;
        using Frequency   = double;
        using Event       = double;
        using Decibel     = double;
        enum class Filter: int { bypass = 0, lowPass, highPass, bandPassCSG, bandPassCZPG, notch, allPass, peaking, lowShelf, highShelf };
        enum class Resonant    { Q, BW };
        struct Args {
            Filter     type;
            Frequency  frequency;
            Event      resValue;
            Decibel    gainDecibel;
            Resonant   resonant;
        };

        ~RBJFilter() override       = default;
        RBJFilter()                 = default;
        RBJFilter(RBJFilter const&) = default;
        RBJFilter(Args      const&);
        
        auto operator=(RBJFilter const&) -> RBJFilter& = default;
        
        void reset	() 	     override;
        bool isValid() const override;
        
        auto setType       (Filter    const&) -> void;
        auto setFrequency  (Frequency const&) -> void;
        auto setResValue   (Event     const&) -> void;
        auto setGainDecibel(Decibel   const&) -> void;
        auto setResonant   (Resonant  const&) -> void;
        auto getType       () const -> Filter;
        auto getFrequency  () const -> Frequency;
        auto getResValue   () const -> Event;
        auto getGainDecibel() const -> Decibel;
        auto getResonant   () const -> Resonant;

        auto processing(SampleValue const&) -> SampleValue;
        auto compute() -> void;

    private:
        struct MemberVariables {
            Unit::Biquad1 biquad;
            Filter        type        = Filter::bypass;
            Frequency     frequency   = 1000.0;
            Event         resValue    = 1.0;
            Decibel       gainDecibel = 0.0;
            Resonant      resonant    = Resonant::Q;
        } m;
    };
    
    inline RBJFilter::RBJFilter(Args const& in)
    : m({
        .type        = in.type,
        .frequency   = in.frequency,
        .resValue    = in.resValue,
        .gainDecibel = in.gainDecibel,
        .resonant    = in.resonant
    }) {
    }
    inline void RBJFilter::reset() {
        m = MemberVariables();
    }
    inline bool RBJFilter::isValid() const {
        return m.type != Filter::bypass;
    }
    
    inline auto RBJFilter::setType(Filter const& in) -> void {
        m.type = in;
        this->compute();
    }
    inline auto RBJFilter::setFrequency(Frequency const& in) -> void {
        m.frequency = in;
        this->compute();
    }
    inline auto RBJFilter::setResValue(Event const& in) -> void {
        m.resValue = in;
        this->compute();
    }
    inline auto RBJFilter::setGainDecibel(Decibel const& in) -> void {
        m.gainDecibel = in;
        this->compute();
    }
    inline auto RBJFilter::setResonant(Resonant const& in) -> void {
        m.resonant = in;
        this->compute();
    }
    inline auto RBJFilter::getType       () const -> Filter     { return m.type; }
    inline auto RBJFilter::getFrequency  () const -> Frequency  { return m.frequency; }
    inline auto RBJFilter::getResValue   () const -> Event      { return m.resValue; }
    inline auto RBJFilter::getGainDecibel() const -> Decibel    { return m.gainDecibel; }
    inline auto RBJFilter::getResonant   () const -> Resonant   { return m.resonant; }
    inline auto RBJFilter::processing(SampleValue const& in) -> SampleValue {
        if(m.type == Filter::bypass) return in;
        return m.biquad.processing(in);
    }
    inline auto RBJFilter::compute() -> void {
        double alpha, a0, a1, a2, b0, b1, b2;
        if(m.type == Filter::bypass) {
            a0 = 0.0;
            a1 = 0.0;
            a2 = 0.0;
            b0 = 0.0;
            b1 = 0.0;
            b2 = 0.0;
        }
        else if(m.type < Filter::peaking) {
            double const omega = 2.0*M_PI*m.frequency/deviceManager.getAudioDevice().getSampleRate();
            double const tsin  = std::sin(omega);
            double const tcos  = std::cos(omega);

            if(m.resonant == Resonant::BW)
                alpha = tsin*sinh(std::log(2.0)/2.0*m.resValue*omega/tsin);
            else
                alpha = tsin/(2.0*m.resValue);

            switch (m.type) {
                case Filter::lowPass:
                    b0 = (1.0-tcos)/2.0;
                    b1 = 1.0-tcos;
                    b2 = (1.0-tcos)/2.0;
                    a0 = 1.0+alpha;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha;
                    break;
                case Filter::highPass:
                    b0 = (1.0+tcos)/2.0;
                    b1 = -(1.0+tcos);
                    b2 = (1.0+tcos)/2.0;
                    a0 = 1.0+ alpha;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha;
                    break;
                case Filter::bandPassCSG:
                    b0 = tsin/2.0;
                    b1 = 0.0;
                    b2 = -tsin/2;
                    a0 = 1.0+alpha;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha;
                    break;
                case Filter::bandPassCZPG:
                    b0 = alpha;
                    b1 = 0.0;
                    b2 = -alpha;
                    a0 = 1.0+alpha;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha;
                    break;
                case Filter::notch:
                    b0 = 1.0;
                    b1 = -2.0*tcos;
                    b2 = 1.0;
                    a0 = 1.0+alpha;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha;
                    break;
                case Filter::allPass:
                    b0 = 1.0-alpha;
                    b1 = -2.0*tcos;
                    b2 = 1.0+alpha;
                    a0 = 1.0+alpha;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha;
                    break;
                default:
                    b0 = 0.0;
                    b1 = 0.0;
                    b2 = 0.0;
                    a0 = 0.0;
                    a1 = 0.0;
                    a2 = 0.0;
                    break;
            }
        }
        else {
            double const A     = std::pow(10.0,(m.gainDecibel/40.0));
            double const omega = 2.0*M_PI*m.frequency/deviceManager.getAudioDevice().getSampleRate();
            double const tsin  = std::sin(omega);
            double const tcos  = std::cos(omega);

            if(m.resonant == Resonant::BW)
                alpha = tsin*sinh(std::log(2.0)/2.0*m.resValue*omega/tsin);
            else
                alpha = tsin/(2.0*m.resValue);

            double const beta = sqrt(A)/m.resValue;
            
            switch (m.type) {
                case Filter::peaking:
                    b0 = 1.0+alpha*A;
                    b1 = -2.0*tcos;
                    b2 = 1.0-alpha*A;
                    a0 = 1.0+alpha/A;
                    a1 = -2.0*tcos;
                    a2 = 1.0-alpha/A;
                    break;
                case Filter::lowShelf:
                    b0 = A*((A+1.0)-(A-1.0)*tcos+beta*tsin);
                    b1 = 2.0*A*((A-1.0)-(A+1.0)*tcos);
                    b2 = A*((A+1.0)-(A-1.0)*tcos-beta*tsin);
                    a0 = (A+1.0)+(A-1.0)*tcos+beta*tsin;
                    a1 = -2.0*((A-1.0)+(A+1.0)*tcos);
                    a2 = (A+1.0)+(A-1.0)*tcos-beta*tsin;
                    break;
                case Filter::highShelf:
                    b0 = A*((A+1.0)+(A-1.0)*tcos+beta*tsin);
                    b1 = -2.0*A*((A-1.0)+(A+1.0)*tcos);
                    b2 = A*((A+1.0)+(A-1.0)*tcos-beta*tsin);
                    a0 = (A+1.0)-(A-1.0)*tcos+beta*tsin;
                    a1 = 2.0*((A-1.0)-(A+1.0)*tcos);
                    a2 = (A+1.0)-(A-1.0)*tcos-beta*tsin;
                    break;
                default:
                    b0 = 0.0;
                    b1 = 0.0;
                    b2 = 0.0;
                    a0 = 0.0;
                    a1 = 0.0;
                    a2 = 0.0;
                    break;
            }
        }
        m.biquad.setValues({
            .b0 = (SampleValue)b0,
            .b1 = (SampleValue)b1,
            .b2 = (SampleValue)b2,
            .a0 = (SampleValue)a0,
            .a1 = (SampleValue)a1,
            .a2 = (SampleValue)a2
        });
    }
}

#endif//BaundStudioEngine_Sources_Effect_Module_RBJFilter_hpp
