/*
 * Baund Studio Engine
 *
 * AndroidLogSupport.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Support_AndroidLogSupport_hpp
#define BaundStudioEngine_Sources_Support_AndroidLogSupport_hpp
#ifdef __ANDROID__

#include <android/log.h>
#include <string>
#include <sstream>

namespace Baund::StudioEngine::Support {
    static inline auto printLog(char        const& level,
                                std::string const& tag,
                                std::string const& comment,
                                std::string const& value = { }
                                ) -> void {
        // BSE Tag
        auto tag_str = "BSE " + tag + " ";
        // comment(: value)
        auto cmt_str = comment + (value.size() > 0 ? ": " + value : "");
        if(level == 'D') {
            __android_log_print(ANDROID_LOG_DEBUG,tag_str.c_str(),"%s", cmt_str.c_str());
        } else if(level == 'E') {
            __android_log_print(ANDROID_LOG_ERROR,tag_str.c_str(),"%s", cmt_str.c_str());
        } else {
            __android_log_print(ANDROID_LOG_UNKNOWN,tag_str.c_str(),"%s", cmt_str.c_str());
        }
    }
}

#endif//__ANDROID__
#endif//BaundStudioEngine_Sources_Support_AndroidLogSupport_hpp
