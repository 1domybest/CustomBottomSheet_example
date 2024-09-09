/*
 * Baund Studio Engine
 *
 * AudioDeviceActions.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Actions_AudioDeviceActions_hpp
#define BaundStudioEngine_Sources_Actions_AudioDeviceActions_hpp

#include "BSETypes.hpp"

#ifdef __cplusplus
extern "C" {
#endif
    
    void _BSEAudioDevice_ready(void);
    void _BSEAudioDevice_start(void);
    void _BSEAudioDevice_stop (void);
    void _BSEAudioDevice_close(void);
    _BSEBool _BSEAudioDevice_isReady(void);
    _BSEBool _BSEAudioDevice_isStart(void);
    _BSESeconds _BSEAudioDevice_getLatency(void);

#ifdef __ANDROID__
    void _BSEAudioDevice_setID(int,int);
#endif//__ANDROID__
#ifdef __cplusplus
}
#endif
#endif//BaundStudioEngine_Sources_Actions_AudioDeviceActions_hpp
