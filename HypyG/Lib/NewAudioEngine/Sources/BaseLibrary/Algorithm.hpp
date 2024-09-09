/*
 * Baund Studio Engine
 *
 * Algorithm.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_Algorithm_hpp
#define BaundStudioEngine_Sources_BaseLibrary_Algorithm_hpp

#include "Definitions.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include <thread>

namespace Baund::StudioEngine::Algorithm {
    template< typename _T >
    auto extractMaxAbsValues(_T const* arr, Size size, int frameUnit) {
        if (not arr) throw Error::NullPointAccess();
        auto maxValues = std::vector<_T>();
        
        for (auto currentFrame = 0; currentFrame < size; ) {
            auto remaning = size - currentFrame;
            auto times    = frameUnit < remaning ? frameUnit : remaning;
            auto currentMax = (_T)0;
            for(auto i=0; i<times; i++) {
                auto value = std::abs(arr[currentFrame]);
                if(value > currentMax) currentMax = value;
                currentFrame += 1;
            }
            maxValues.push_back(currentMax);
        }
        return maxValues;
    }
    template< typename _T >
    auto extractMaxAbsValues(std::vector<_T> const& arr, int frameUnit) {
        return extractMaxAbsValues(arr.data(),arr.size(),frameUnit);
    }
    inline auto convertAudioFramesToSeconds(long frames, double sampleRate) -> double {
        if(frames == 0) return 0.0;
        else return frames / sampleRate;
    }
    inline auto convertSecondsToAudioFrames(double time, double sampleRate) -> long {
        return std::ceil(time * sampleRate);
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_Algorithm_hpp
