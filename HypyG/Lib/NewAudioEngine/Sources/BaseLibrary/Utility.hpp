/*
 * Baund Studio Engine
 *
 * Utility.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_Utility_hpp
#define BaundStudioEngine_Sources_BaseLibrary_Utility_hpp

#include "Sources/BaseLibrary/Definitions.hpp"
#include <chrono>
#include <functional>
#include <thread>

namespace Baund::StudioEngine::Utility {
    static constexpr auto operator""_sec(unsigned long long in) {
        return std::chrono::seconds(in);
    }
    static constexpr auto operator""_sec(long double in) {
        return std::chrono::duration<double>(in);
    }
    static constexpr auto operator""_msec(unsigned long long in) {
        return std::chrono::milliseconds(in);
    }
    static constexpr auto operator""_msec(long double in) {
        return std::chrono::duration<double>(in/1000.0);
    }
    static constexpr auto operator""_usec(unsigned long long in) {
        return std::chrono::microseconds(in);
    }
    static constexpr auto operator""_usec(long double in) {
        return std::chrono::duration<double>(in/1000.0/1000.0);
    }
    template< typename _T, typename _U >
    static inline auto sleep(std::chrono::duration<_T,_U> in) {
        std::thread thread([in] { std::this_thread::sleep_for(in); });
        thread.join();
    }
    static inline auto getProcessTime(std::function<void()> func) {
        auto before = std::chrono::system_clock::now();
        func();
        auto after = std::chrono::system_clock::now();
        auto time  = std::chrono::duration<double>(after - before);
        return time;
    }
    static inline auto getNewFileNameWithTime(std::string const& name, std::string const& ext) {
        auto now  = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto path = name + "_" + std::to_string(time) + "." + ext;
        return path;
    }
}
namespace Baund::StudioEngine {
    namespace Util = Utility;
    using Util::operator""_sec;
    using Util::operator""_msec;
    using Util::operator""_usec;
}

#endif//BaundStudioEngine_Sources_BaseLibrary_Utility_hpp
