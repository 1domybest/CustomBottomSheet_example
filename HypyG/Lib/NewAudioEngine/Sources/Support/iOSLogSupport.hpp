/*
 * Baund Studio Engine
 *
 * iOSLogSupport.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Support_iOSLogSupport_hpp
#define BaundStudioEngine_Sources_Support_iOSLogSupport_hpp
#ifdef __APPLE__

#include <iostream>
#include <string>

namespace Baund::StudioEngine::Support {
    static inline auto printLog(char        const& level,
                                std::string const& tag,
                                std::string const& comment,
                                std::string const& value = {}
                                ) -> void {
        // L/BSE Tag: comment
        std::cout << level << "/BSE "<< tag << ": " << comment;
        // L/BSE Tag: comment: value
        if (value.length() > 0) std::cout << ": " << value;
        std::cout << std::endl;
    }
}

#endif//__APPLE__
#endif//BaundStudioEngine_Sources_Support_iOSLogSupport_hpp
